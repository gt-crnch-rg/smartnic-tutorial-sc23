#!/bin/bash
#This script just shows how to allocate two nodes in a heterogeneous fashion to get a paired host and BF3 

#Read in the node number from command line
NODE=$1

check_cmd_params()
{

if [ -z $NODE ];
then 
	printf "Pass a node number from 001 to 032\n"
	printf "Ex: ./request_bf3_interactive_node.sh 015"
	exit
fi
}

check_cmd_params

#If the user specified a parameter, then request two nodes for 10 minutes in an exclusive fashion
# -w specifies node names, -p specifies the queue, and --exclusive specifies that no other jobs will share
#these nodes
salloc --nodes=2 -w thor$NODE,thorbf3a$NODE -p thor --time=0:10:00 --exclusive
