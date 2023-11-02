# Instructions

1. `vim run_service.sh` (select the right node)
2. `vim run_task.sh` (select the right node)
3. Export `LIB_` environment variable to the directory containing libaries as `export LIB_=/path/to/lib/task_c/build/`
4. `sbatch --export=NONE run_service.sh` run the service on DPU
5. `sbatch --export=NONE run_task.sh ./task_c/build/shared` run the task on host
6. Observe the output as follwing or refer to the `out` folder:
### task file
```
lib path: /global/home/users/uthmanhere/omp_exp/smartnic-tutorial-sc23/code/04-openmp-offload-dpu/task_c/build/
the path includes following libs:
/global/home/users/uthmanhere/omp_exp/smartnic-tutorial-sc23/code/04-openmp-offload-dpu/task_c/build//liblog_aarch64.so
/global/home/users/uthmanhere/omp_exp/smartnic-tutorial-sc23/code/04-openmp-offload-dpu/task_c/build//liblog_x86.so
cmd: ./task_c/build/shared
```
### service file
```
lib path: /global/home/users/uthmanhere/omp_exp/smartnic-tutorial-sc23/code/04-openmp-offload-dpu/task_c/build/
Hello from the other side (shared object actually..)
```

# Compilation
Just run the command:
```
sh compile.sh
```
It will make a build direcotry and add
shared objects and application binary in it.

