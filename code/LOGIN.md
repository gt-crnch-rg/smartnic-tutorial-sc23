Instructions:

1. `ssh rdmaworkshop##@35.233.188.131` (replace `rdmaworkshop##` with the actual account you are given).
2. If authentication fails with `Permission denied (publickey,password).` error you have a default authentication method set to publickey. In that case change this command to `ssh -o PreferredAuthentications=password rdmaworkshop##@35.233.188.131`.
3. Once logged in to the jumpbox, `ssh gw.hpcadvisorycouncil.com`.
4. Check jobs and resources with `squeue -p thor` `sinfo -p thor`.
5. When submitting jobs make sure to use the `sc23` reservation, e.g., `sbatch --res=sc23 ...`, `salloc --res=sc23 ...`.
