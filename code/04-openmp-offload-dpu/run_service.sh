#!/bin/bash -l
#SBATCH -p thor
#SBATCH -w thorbf3a031
#SBATCH -o service-%j.out

# the following lineis for task c
if [ -n "$LIB_" ]; then
	echo "lib path: $LIB_"
	export LD_LIBRARY_PATH=$LIB_:$LD_LIBRARY_PATH
fi
MODULEPATH_=/global/home/groups/rdmaworkshop/doca-omp-service/modulefiles/

module load doca $MODULEPATH_/doca-omp-service.aarch64
doca-omp-service

