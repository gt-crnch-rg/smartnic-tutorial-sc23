#!/bin/bash -l
#SBATCH -p thor
#SBATCH -w thor031
#SBATCH -o task-%j.out

# the following line is for task c
if [ -n "$LIB_" ]; then
	echo "lib path: $LIB_"
	echo "the path includes following libs...:"
	ls $LIB_/*.so
	export LD_LIBRARY_PATH=$LIB_:$LD_LIBRARY_PATH
fi

module load doca

COMMAND_=$@

echo "cmd: $COMMAND_"

$COMMAND_

