#!/bin/bash
#SBATCH -p thor
#SBATCH -w thor[001-008],thorbf3a[001-008],thorbf2a[001-008]
#SBATCH -t 30:0
#SBATCH --res sc23

sh /global/home/groups/rdmaworkshop/offload_stack/run.sh bf3_ib ompi sep23 p3dfft nonblocking 8 32 1 0 0 0
sh /global/home/groups/rdmaworkshop/offload_stack/run.sh bf3_ib ucc  sep23 p3dfft nonblocking 8 32 1 4 4 4
sh /global/home/groups/rdmaworkshop/offload_stack/run.sh bf2_ib ompi sep23 p3dfft nonblocking 8 32 1 0 0 0
sh /global/home/groups/rdmaworkshop/offload_stack/run.sh bf2_ib ucc  sep23 p3dfft nonblocking 8 32 1 4 4 4
