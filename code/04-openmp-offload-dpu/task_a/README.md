# Instructions

1. `vim run_service.sh` (select the right node)
2. `vim run_task.sh` (select the right node)
3. `sbatch --export=NONE run_service.sh` run the service on DPU
4. `sbatch --export=NONE run_task.sh ./task_a/hello` run the task on host
5. Observe the output as follwing or refer to the `out` folder:
### task file
```
cmd: ./task_a/hello
```
### service file
```
Hi Folks!
```
