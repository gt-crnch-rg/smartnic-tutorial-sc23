Instructions:

1. `cd smartnic-tutorial-sc23/code/01-mpi-collectives`
2. `vi run_osu.sh` (replace node sequence [001-004] with something available, always use four consecutive nodes in sections, ie., 001-004, 005-008, 009-012, 013-016, 017-020, 021-024, 025-028, 029-032)
3. `sbatch run_osu.sh`
4. wait for the job to complete (should finish within a few minutes after started running)
5. `diff -y -W150 osu_allgatherv-thor-bf2_ib-ompi-sep23-4-32-1-0-0-0.log osu_allgatherv-thor-bf2_ib-ucc-sep23-4-32-1-4-4-4.log`
```
# OSU MPI Allgatherv Latency Test v5.8                                  # OSU MPI Allgatherv Latency Test v5.8
# Size       Avg Latency(us)   Min Latency(us)   Max Latency(us)  Iter          # Size       Avg Latency(us)   Min Latency(us)   Max Latency(us)  Iter
256                    75.00             58.08             87.41          |     256                  1835.82           1826.81           1849.30
512                   135.99            107.54            158.90          |     512                  1772.42           1762.96           1784.71
1024                  216.41            208.07            230.68          |     1024                 1771.06           1761.99           1785.55
2048                  272.53            262.82            285.09          |     2048                 1778.51           1768.03           1792.72
4096                  344.07            335.84            357.57          |     4096                 1779.49           1766.15           1794.37
8192                  481.87            476.41            491.85          |     8192                 1786.26           1771.69           1802.33
16384                 973.74            963.33            990.42          |     16384                1784.57           1762.39           1806.51
32768                2276.58           2257.96           2298.80          |     32768                1806.08           1789.20           1825.49
65536                4461.15           4168.15           4723.16          |     65536                2953.64           2910.42           3033.82
131072               9034.13           8452.73           9538.94          |     131072               6005.69           5947.82           6158.57
262144              12139.21          11927.74          12362.73          |     262144              12200.98          12092.07          12409.39
524288              24773.29          24263.16          25525.17          |     524288              24845.62          24649.39          25131.77
1048576             53654.94          53332.47          53960.20          |     1048576             51377.30          50658.26          52396.19
```
6. `vi run_osu.sh` (replace allgatherv with iallgatherv)
7. repeat step 3-5
8. can also experiment by replacing "allgatherv" with "alltoall" or "alltoallv"
9. Why are we seeing what we are seeing?
