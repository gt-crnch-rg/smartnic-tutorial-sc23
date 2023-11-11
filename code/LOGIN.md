Instructions:

1. `ssh rdmaworkshop##@gw.hpcadvisorycouncil.com` (replace `rdmaworkshop##` with the actual account you are given).
2. If authentication fails with `Permission denied (publickey,password).` error you have a default authentication method set to publickey. In that case change this command to `ssh -o PreferredAuthentications=password rdmaworkshop##@gw.hpcadvisorycouncil.com`.
3. Check jobs and resources `squeue -p thor` `sinfo -p thor`.
