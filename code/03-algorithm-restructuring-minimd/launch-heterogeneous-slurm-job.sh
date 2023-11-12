#!/bin/bash

# This script shows how to request a paired host and BlueField node

# Request two nodes on the Thor cluster
salloc -p thor --nodes=2 --ntasks-per-node=1 --time=00:10:00 -w thor014,thorbf3a014
