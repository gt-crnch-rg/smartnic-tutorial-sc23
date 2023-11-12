/* ----------------------------------------------------------------------
   miniMD is a simple, parallel molecular dynamics (MD) code.   miniMD is
   an MD microapplication in the Mantevo project at Sandia National
   Laboratories ( http://www.mantevo.org ). The primary
   authors of miniMD are Steve Plimpton (sjplimp@sandia.gov) , Paul Crozier
   (pscrozi@sandia.gov) and Christian Trott (crtrott@sandia.gov).

   Copyright (2008) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This library is free software; you
   can redistribute it and/or modify it under the terms of the GNU Lesser
   General Public License as published by the Free Software Foundation;
   either version 3 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this software; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
   USA.  See also: http://www.gnu.org/licenses/lgpl.txt .

   For questions, contact Paul S. Crozier (pscrozi@sandia.gov) or
   Christian Trott (crtrott@sandia.gov).

   Please read the accompanying README and LICENSE files.
---------------------------------------------------------------------- */

#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"
#include "force_lj.h"
#include "integrate.h"
#include "thermo.h"
#include "bf_comm.h"

Thermo::Thermo() {}
Thermo::~Thermo() {}

void Thermo::setup(MMD_float rho_in, Integrate &integrate, Atom &atom, int units)
{
  rho = rho_in;
  ntimes = integrate.ntimes;

  MMD_int maxstat;

  if(nstat == 0) maxstat = 2;
  else maxstat = ntimes / nstat + 2;

  steparr = (MMD_int*) malloc(maxstat * sizeof(MMD_int));
  tmparr = (MMD_float*) malloc(maxstat * sizeof(MMD_float));
  engarr = (MMD_float*) malloc(maxstat * sizeof(MMD_float));
  prsarr = (MMD_float*) malloc(maxstat * sizeof(MMD_float));

  if(units == LJ) {
    mvv2e = 1.0;
    dof_boltz = (atom.natoms * 3 - 3);
    t_scale = mvv2e / dof_boltz;
    p_scale = 1.0 / 3 / atom.box.xprd / atom.box.yprd / atom.box.zprd;
    e_scale = 0.5;
  } else if(units == METAL) {
    mvv2e = 1.036427e-04;
    dof_boltz = (atom.natoms * 3 - 3) * 8.617343e-05;
    t_scale = mvv2e / dof_boltz;
    p_scale = 1.602176e+06 / 3 / atom.box.xprd / atom.box.yprd / atom.box.zprd;
    e_scale = 524287.985533;//16.0;
    integrate.dtforce /= mvv2e;

  }
}

void Thermo::compute(MMD_int iflag, Atom &atom, Neighbor &neighbor, Force* force, Timer &timer, Comm &comm)
{
  MMD_float t, eng, p;

  if(iflag > 0 && iflag % nstat) return;

  if(iflag == -1 && nstat > 0 && ntimes % nstat == 0) return;

  t_act = 0;
  e_act = 0;
  p_act = 0;
  
  #pragma omp barrier
  t = temperature(atom);
  
  #pragma omp master
  {
    eng = energy(atom, neighbor, force);
    
    p = pressure(t, force);
    MMD_int istep = iflag;
    if(iflag == -1) istep = ntimes;

    if(iflag == 0) mstat = 0;
    
    steparr[mstat] = istep;
    tmparr[mstat] = t;
    engarr[mstat] = eng;
    prsarr[mstat] = p;
    
    mstat++;

    double oldtime = timer.array[TIME_TOTAL];
    timer.barrier_stop(TIME_TOTAL);
    
    if(threads->mpi_me == 0) {
      fprintf(stdout, "%i %e %e %e %6.3lf\n", istep, t, eng, p, istep == 0 ? 0.0 : timer.array[TIME_TOTAL]);
    }

    timer.array[TIME_TOTAL] = oldtime;
  }
}

/* reduced potential energy */

MMD_float Thermo::energy(Atom &atom, Neighbor &neighbor, Force* force)
{
  e_act = force->eng_vdwl;

  if(neighbor.halfneigh) {
    e_act *= 2.0;
  }

  e_act *= e_scale;
  MMD_float eng;

  if(sizeof(MMD_float) == 4)
    MPI_Allreduce(&e_act, &eng, 1, MPI_FLOAT, MPI_SUM, hosts_communicator);
  else
    MPI_Allreduce(&e_act, &eng, 1, MPI_DOUBLE, MPI_SUM, hosts_communicator);

  return eng / atom.natoms;
}

/*  reduced temperature */

MMD_float Thermo::temperature(Atom &atom)
{
  MMD_int i;
  MMD_float vx, vy, vz;

  MMD_float t = 0.0;
  t_act = 0;
  #pragma omp barrier

  MMD_float* v = atom.v;

  OMPFORSCHEDULE
  for(i = 0; i < atom.nlocal; i++) {
    vx = v[i * PAD + 0];
    vy = v[i * PAD + 1];
    vz = v[i * PAD + 2];
    t += (vx * vx + vy * vy + vz * vz) * atom.mass;
  }

  #pragma omp atomic
  t_act += t;

  #pragma omp barrier

  MMD_float t1;
  #pragma omp master
  {
    if(sizeof(MMD_float) == 4)
      MPI_Allreduce(&t_act, &t1, 1, MPI_FLOAT, MPI_SUM, hosts_communicator);
    else
      MPI_Allreduce(&t_act, &t1, 1, MPI_DOUBLE, MPI_SUM, hosts_communicator);

  }
  return t1 * t_scale;
}


/* reduced pressure from virial
   virial = Fi dot Ri summed over own and ghost atoms, since PBC info is
   stored correctly in force array before reverse_communicate is performed */

MMD_float Thermo::pressure(MMD_float t, Force* force)
{
  p_act = force->virial;

  MMD_float virial = 0;

  if(sizeof(MMD_float) == 4)
    MPI_Allreduce(&p_act, &virial, 1, MPI_FLOAT, MPI_SUM, hosts_communicator);
  else
    MPI_Allreduce(&p_act, &virial, 1, MPI_DOUBLE, MPI_SUM, hosts_communicator);

  //printf("Pres: %e %e %e %e\n",t,dof_boltz,virial,p_scale);
  return (t * dof_boltz + virial) * p_scale;
}


