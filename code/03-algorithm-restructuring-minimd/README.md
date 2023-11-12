## Code Sample 3 - Algorithmic Restructuring with MiniMD


This code sample builds on the MiniMD codebase, which is a proxy application for LAMMPS. In this example, there are two variants of the code: 1) a standard MiniMD implementation which runs on the host and 2) a "mixed" or heterogenoeous version that performs the force calculations on a BlueField DPU. You can compile and run both variants with different OpenMP and MPI parameters to compare the overall performance.  

### Compilation of this Example:

### Running the Examples: 

### Sample Output:


Instructions:

1. `cd smartnic-tutorial-sc23/code/02-code-transformations-p3dfft`
2. `vi run_p3dfft.sh` (replace node sequence [001-008] with something available, always use eight consecutive nodes in sections, ie., 001-008, 009-016, 017-024, 025-032)
3. `sbatch run_p3dfft.sh`
4. wait for the job to complete (should finish within a few minutes after start running)
5. `diff -y -W150 p3dfft_nonblocking-thor-bf3_ib-ompi-sep23-8-32-1-0-0-0.log p3dfft_nonblocking-thor-bf3_ib-ucc-sep23-8-32-1-4-4-4.log`
```
Transform time (avg/min/max): 7.202699 6.308231 7.718879                  |     Transform time (avg/min/max): 5.616796 5.486121 5.724293
Sum total:    65.3297    56.7619    74.6215                               |     Sum total:    56.0948    51.3249    60.7244
```

### Learn More
This example was developed by Sara Karamati, Max Hawkins, and Jeff Young of Georgia Tech. For more information on the evaluation of this application please see the following paper:

