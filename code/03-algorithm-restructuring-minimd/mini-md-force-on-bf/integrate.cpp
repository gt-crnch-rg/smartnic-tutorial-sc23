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
//#define PRINTDEBUG(a) a
#define PRINTDEBUG(a)
#include "stdio.h"
#include "integrate.h"
#include "openmp.h"
#include "math.h"
#include "bf_comm.h"
#include <algorithm>

void comm_neigh_to_bf(Atom &atom, Neighbor &neighbor, Comm &comm, int n);
void comm_atom_to_bf(Atom &atom, int n);
void comm_force_to_host(Atom &atom);


Integrate::Integrate() {sort_every=20;}
Integrate::~Integrate() {}

void Integrate::setup()
{
  dtforce = 0.5 * dt;
}

void Integrate::initialIntegrate(int nb)
{
    OMPFORSCHEDULE
    for(MMD_int i = 0; i < nlocal; i++){
        v[i * PAD + 0] += dtforce * f[i * PAD + 0];
        v[i * PAD + 1] += dtforce * f[i * PAD + 1];
        v[i * PAD + 2] += dtforce * f[i * PAD + 2];
        x[i * PAD + 0] += dt * v[i * PAD + 0];
        x[i * PAD + 1] += dt * v[i * PAD + 1];
        x[i * PAD + 2] += dt * v[i * PAD + 2];
    }
    if(nb){
      MPI_Barrier(nic_host_communicator);
    }
     
}

void Integrate::finalIntegrate()
{
  OMPFORSCHEDULE
  for(MMD_int i = 0; i < nlocal; i++) {
    v[i * PAD + 0] += dtforce * f[i * PAD + 0];
    v[i * PAD + 1] += dtforce * f[i * PAD + 1];
    v[i * PAD + 2] += dtforce * f[i * PAD + 2];
  }

}

void Integrate::run(Atom &atom, Force* force, Neighbor &neighbor,
                    Comm &comm, Thermo &thermo, Timer &timer)
{
  int i, n;

  comm.timer = &timer;
  timer.array[TIME_TEST] = 0.0;

  int check_safeexchange = comm.check_safeexchange;

  mass = atom.mass;
  dtforce = dtforce / mass;
  //Use OpenMP threads only within the following loop containing the main loop.
  //Do not use OpenMP for setup and postprocessing.
  
  #pragma omp parallel private(i,n)
  {
    int next_sort = sort_every>0?sort_every:ntimes+1;
    
    for(n = 0; n < ntimes; n++) {
      
      if(isHost)
      {
        #pragma omp barrier

        x = atom.x;
        v = atom.v;
        f = atom.f;
        xold = atom.xold;
        nlocal = atom.nlocal;
        
        #ifdef BF
        if((n + 1) % neighbor.every) {
        #endif
          initialIntegrate(0);
        #ifdef BF
        }else{
          initialIntegrate(1);  
        }
        #endif
        
        #pragma omp master
        timer.stamp();

        if((n + 1) % neighbor.every) {
          
          comm.communicate(atom);
          #pragma omp master
          timer.stamp(TIME_COMM);
          
        } else {
          
          //these routines are not yet ported to OpenMP
          {
            if(check_safeexchange) {
              #pragma omp master
              {
                double d_max = 0;

                for(i = 0; i < atom.nlocal; i++) {
                  double dx = (x[i * PAD + 0] - xold[i * PAD + 0]);

                  if(dx > atom.box.xprd) dx -= atom.box.xprd;

                  if(dx < -atom.box.xprd) dx += atom.box.xprd;

                  double dy = (x[i * PAD + 1] - xold[i * PAD + 1]);

                  if(dy > atom.box.yprd) dy -= atom.box.yprd;

                  if(dy < -atom.box.yprd) dy += atom.box.yprd;

                  double dz = (x[i * PAD + 2] - xold[i * PAD + 2]);

                  if(dz > atom.box.zprd) dz -= atom.box.zprd;

                  if(dz < -atom.box.zprd) dz += atom.box.zprd;

                  double d = dx * dx + dy * dy + dz * dz;

                  if(d > d_max) d_max = d;
                }

                d_max = sqrt(d_max);

                if((d_max > atom.box.xhi - atom.box.xlo) || (d_max > atom.box.yhi - atom.box.ylo) || (d_max > atom.box.zhi - atom.box.zlo))
                  printf("Warning: Atoms move further than your subdomain size, which will eventually cause lost atoms.\n"
                  "Increase reneighboring frequency or choose a different processor grid\n"
                  "Maximum move distance: %lf; Subdomain dimensions: %lf %lf %lf\n",
                  d_max, atom.box.xhi - atom.box.xlo, atom.box.yhi - atom.box.ylo, atom.box.zhi - atom.box.zlo);

              }

            }
            
            #ifdef BF
              MPI_Barrier(nic_host_communicator);
              
            #endif
            
            #pragma omp master
            timer.stamp_extra_start();
            
            comm.exchange(atom);
            
            
            if(n+1>=next_sort) {
              atom.sort(neighbor);
              
              #ifdef BF
              MPI_Aint sorted_index_address;
              MPI_Get_address(atom.sorted_index, &sorted_index_address); 
              MPI_Send(&sorted_index_address, 1, MPI_AINT, 1, 0, nic_host_communicator);
              #endif
              next_sort +=  sort_every;
            }
            
            
            comm.borders(atom);
            #ifdef BF
            
            MPI_Get_address(atom.f, &f_address); 
            MPI_Send(&f_address, 1, MPI_AINT, 1, 0, nic_host_communicator);
            #endif
            
            #pragma omp master
            {
              timer.stamp_extra_stop(TIME_TEST);
              timer.stamp(TIME_COMM);
            }

            if(check_safeexchange)
              for(int i = 0; i < PAD * atom.nlocal; i++) xold[i] = x[i];
          }
          
          #pragma omp barrier

          neighbor.build(atom);

          #ifdef BF
            comm_neigh_to_bf(atom, neighbor,comm, n);
          #endif
          
          // #pragma omp barrier
          
          
          
          
          #pragma omp master
          timer.stamp(TIME_NEIGH);
        }
        
        force->evflag = (n + 1) % thermo.nstat == 0;
        
        #ifdef BF
        if((n + 1) % neighbor.every) {
        #endif
          force->compute(atom, neighbor, comm, comm.me, 1);
          
          #pragma omp master
          timer.stamp(TIME_FORCE);
        
          if(neighbor.halfneigh && neighbor.ghost_newton) {
            comm.reverse_communicate(atom);
          
            #pragma omp master
            timer.stamp(TIME_COMM);
           
          }
        #ifdef BF
        }else{
          MPI_Barrier(nic_host_communicator);
        }
        #endif
        v = atom.v;
        f = atom.f;
        nlocal = atom.nlocal;
        
        #pragma omp barrier

        finalIntegrate();
        
        if(thermo.nstat){
          #ifdef BF
          if(((n + 1) % neighbor.every) == 0) {
            MMD_float force_thermo[2]={force->eng_vdwl, force->virial};
            MPI_Recv(force_thermo, 2, MPI_DOUBLE, 1, 0, nic_host_communicator, MPI_STATUS_IGNORE);
            force->eng_vdwl=force_thermo[0]; 
            force->virial=force_thermo[1];
          } 
          #endif
           thermo.compute(n + 1, atom, neighbor, force, timer, comm);
        }
        
        
        
      } else{
          #ifdef BF
            if(((n + 1) % neighbor.every) == 0) {
              
              MPI_Barrier(nic_host_communicator);
              comm_atom_to_bf(atom, n);
              MPI_Barrier(nic_host_communicator);
              
              comm.communicate(atom);
              force->evflag = (n + 1) % thermo.nstat == 0;
              force->compute(atom, neighbor, comm, comm.me, 0);
              
              if(neighbor.halfneigh && neighbor.ghost_newton) {
                comm.reverse_communicate(atom);
              }
              
              comm.exchange_bf(atom);
              
              if(n+1>=next_sort) {
                
                MPI_Aint sorted_index_address;
                MPI_Recv(&sorted_index_address, 1, MPI_AINT, 0, 0, nic_host_communicator, MPI_STATUS_IGNORE);
                MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win_sorted_index);
                MPI_Get(atom.sorted_index,atom.nlocal, MPI_INT, 0, sorted_index_address, atom.nlocal, MPI_INT, win_sorted_index);
                MPI_Win_flush_local(0, win_sorted_index);
                MPI_Win_unlock(0, win_sorted_index);
                
                atom.sort_bf();
                
                next_sort +=  sort_every;
              }
              
              comm_force_to_host(atom);
              MPI_Barrier(nic_host_communicator);
              if(thermo.nstat){
                        MMD_float force_thermo[2]={force->eng_vdwl, force->virial};
                        MPI_Send(force_thermo, 2, MPI_DOUBLE, 0, 0, nic_host_communicator);
              }
              
              comm_neigh_to_bf(atom, neighbor, comm, n);  
              
            }
          #endif
          

        }
      }
    } //end OpenMP parallel
    
    /*if(isHost){
      double sum=0.0;
      for(int i=0; i<(atom.nlocal+atom.nghost)*PAD;i++){
  	    sum+=atom.x[i];
      }
      printf("final x location: %f\n", sum);
    }*/
}
