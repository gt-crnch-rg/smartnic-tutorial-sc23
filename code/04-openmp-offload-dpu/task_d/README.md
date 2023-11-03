# Instructions

1. `vim run_service.sh` (select the right node for server)
2. `sbatch --export=NONE run_service.sh` run the service on DPU for server
3. `vim run_service.sh` (select the right node for client)
4. `sbatch --export=NONE run_service.sh` run the service on DPU for client
5. `vim run_task.sh` (select the right node for server)
6. `sbatch --export=NONE run_task.sh ./task_d/net_accel` run the server task on host
7. `vim run_task.sh` (select the right node for server)
8. `sbatch --export=NONE run_task.sh ./task_d/net_accel c 192.168.3.2XX` (where `XX` is the number in the name of the node) run the client task on host
9. Observe the output as follwing or refer to the `out` folder:
### server task file
```
cmd : ./task_d/net_accel
```
### client task file
```
cmd : ./task_d/net_accel c 192.168.3.230
```
### server service file
```
connection established.
to   client > 01
to   client > 03
to   client > 05
to   client > 07
to   client > 09
to   client > 11
to   client > 13
to   client > 15
to   client > 17
to   client > 19
```
### client service file
```
connection established.
to   server > 00
to   server > 02
to   server > 04
to   server > 06
to   server > 08
to   server > 10
to   server > 12
to   server > 14
to   server > 16
to   server > 18
```

