#!/bin/bash

# Source this script to set your environment to run the heterogeneous MiniMD example
# Authors: Max Hawkins with minor edits by Jeff Young

# Query Architecture
arch=$(uname -i)
# Create unique symlink name
FILE=/tmp/hpcx-$USER

# Unlink existing symlink if it exists
if test -h $FILE; then
	unlink $FILE
fi

# If on a traditional x86 host
if [ "$arch" == 'x86_64' ]; then
	ln -s /global/software/centos-8.x86_64/modules/gcc/8.3.1/hpcx/2.15.0/ $FILE
	export PATH=/global/software/centos-8.x86_64/modules/gcc/8.3.1/hpcx/2.14.0/ucx/bin:$PATH
	export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64:/global/software/centos-8.x86_64/modules/gcc/8.3.1/hpcx/2.14.0/ucx/lib:/global/software/centos-8.x86_64/modules/gcc/8.3.1/hpcx/2.14.0/ompi/lib/:$LD_LIBRARY_PATH
# Else if on a BlueField node
elif [ "$arch" == 'aarch64' ]; then
	ln -s /global/software/rocky-9.aarch64/modules/gcc/11/hpcx/2.15 $FILE
	export PATH=/global/software/rocky-9.aarch64/modules/gcc/11/hpcx/2.15/ucx/bin:$FILE/ompi/tests/osu-micro-benchmarks-5.6.2:$PATH
	export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64:/global/software/rocky-9.aarch64/modules/gcc/11/hpcx/2.15/ucx/lib:/global/software/rocky-9.aarch64/modules/gcc/11/hpcx/2.15.0/ompi/lib/:$LD_LIBRARY_PATH
fi

source $FILE/hpcx-init.sh
hpcx_load
