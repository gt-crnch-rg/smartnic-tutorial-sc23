## Code Sample 3 - Algorithmic Restructuring with MiniMD


This code sample builds on the MiniMD codebase, which is a proxy application for LAMMPS. In this example, there are two variants of the code: 1) a standard MiniMD implementation which runs on the host and 2) a "mixed" or heterogenoeous version that performs the force calculations on a BlueField DPU. You can compile and run both variants with different OpenMP and MPI parameters to compare the overall performance.  

This example builds on the related work by Maw Hawkins to do a simple "heterogeneous" BF3 example in this repo: https://github.com/max-Hawkins/heterog-bluefield-dev. The same instructions for compiling the hello world example should also work here.

### Compilation of this Example:

1) First you need to request an interactive node to compile the example code and run it. We highly recommend that you open two terminals or use a screen/tmux session to allow logging into the host and BF3 nodes.

```
[user@login02]$ cd code/03-algorithm-restructuring-minimd
#Note you should select a number between 001 and 032 for node number
$ ./scripts/request_bf3_heterogenous_slurm_job.sh 008
``` 

Once you have a session and two terminal sessions, you need to compile on the host and BF3 separately.

Terminal 1 (Host):
```
[user@login02]$ ssh thor008
[user@thor008]$ cd <minimd-sample-folder>/ref
#Load the right version of HPC-X and associated modules for MPI
[user@thor008]$ scripts/hpcai_cluster_set_env.sh && hpcx_load
[user@thor008]$ make openmpi && mv miniMD_openmpi_ minimd-host.exe

```

Terminal 2 (BF3):
```
[user@login02]$ ssh thorbf3a008
[user@thorbf3a008]$ cd <minimd-sample-folder>/ref
#Load the right version of HPC-X and associated modules for MPI
[user@thorbf3a008]$ scripts/hpcai_cluster_set_env.sh && hpcx_load
[user@thorbf3a008]$ make openmpi && mv miniMD_openmpi_ minimd-bf.exe
```

### Running the Sample: 

To run this sample, you need to launch the heterogeneous MPI job from the BlueField device:


```
#This simple example uses the LJ input with one thread per host and BF
[user@thorbf3a008]$ mpirun -np 1 -H thor008 minimd-host.exe -app in.lj.miniMD : -np 1 -H thorbf3a008 minimd-bf.exe -app in.lj.miniMD
```

### Learn More
This example was developed by Sara Karamati, Max Hawkins, and Jeff Young of Georgia Tech. For more information on the evaluation of this application please see the following paper:

Sara Karamati, et al., "Smarter" NICs for faster molecular dynamics: a case study, IPDPS 2023.
