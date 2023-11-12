# Leveraging SmartNICs for HPC and Data Center Applications

Tutorial on SmartNICs for Supercomputing 2023


**Held on**: November 12th, 1:30 PM - 5:00 PM US Mountain Standard Time 

[Event website and Program](https://sc23.supercomputing.org/presentation/?id=tut141&sess=sess214)

![BlueField 3 SmartNIC](https://github.com/gt-crnch-rg/smartnic-tutorial-hot-interconnects/blob/41db05cd929c19a445c6afae5dcb226bfb64ac79/fig/smartnic_bf3.jpg)

## Mentimeter Audience Survey

Please take our survey using [this Mentimeter link!](https://www.menti.com/aljr5bmzi3wc).

## Presenters:
* Richard Graham [(NVIDIA)](nvidia.com)
* Antonio Peña [(Barcelona Supercomputing Center)](https://www.bsc.es/pena-antonio)
* Yong Qin [(NVIDIA)](nvidia.com)
* Jeffrey Young [(Georgia Institute of Technology)](https://crnch-rg.cc.gatech.edu/)
* Richard Vuduc [(Georgia Institute of Technology)](https://crnch-rg.cc.gatech.edu/)

Supporting Contributors:
* Muhammad Usman [(Barcelona Supercomputing Center)](https://www.bsc.es/usman-muhammad)
* Oscar Hernandez [(Oak Ridge National Laboratory)](ornl.gov)
* Dmitry Pekurovsky [(NVIDIA)](nvidia.com)


**Abstract:** The past few years have witnessed a surge in the number of advanced network adapters, known as "SmartNICs", that offer additional functionalities beyond standard packet processing capabilities. These devices often feature programmable lightweight processing cores, FPGAs, and even CPU- and GPU-based platforms capable of running separate operating systems. Though primarily aimed at data center operations, such as infrastructure management, packet filtering, and I/O acceleration, SmartNICs are increasingly being explored for high-performance computing (HPC) application acceleration.

This tutorial offers an in-depth exploration of the state-of-the-art for SmartNICs and the emerging software ecosystems supporting them. Attendees will engage in hands-on exercises to better understand how to use SmartNICs for HPC application acceleration, including MPI collective operation offloading, OpenMP offload, and algorithmic modifications to maximize on-board processing power. Participants will have the opportunity to execute these exercises using cutting-edge SmartNICs like NVIDIA's BlueField-3 Data Processing Unit (DPU). The tutorial presenters will discuss additional techniques for optimizing applications to harness SmartNICs as communication accelerators in HPC systems.

Please note that you must register for the SC23 tutorial track and attend this tutorial in person.

[Register here](https://sc23.supercomputing.org/attend/registration/)

## Tentative Agenda (Note this may change!)


| Time          | Topic                                   | Presenters   | 
| ------------- | --------------------------------------- | -------------| 
| 1:30 - 1:40    | Introduction and setting up system access | Jeff /Yong | 
| 1:40 - 2:10   | SmartNIC Introduction and Overview      | Rich G       | 
| 2:10 - 2:20   | Programming approaches for HPC with SmartNICs     | Jeff        |
| 2:20- 2:30   | Transforming applications with MPI and SHMEM                      |       Rich G      |
| 2:30 - 2:40   | Heterogeneous MPI Applications: Host + SmartNIC ranks |  Jeff       |
| 2:40 - 3:00   | Using the SmartNIC as an Communication Accelerator      |  Yong       |
| 3:00 - 3:30   | **BREAK**                                |             |
| 3:30 - 4:00   | OpenMP offload to the SmartNIC |  Antonio       |
| 4:00 - 5:00   | Hands-on with BlueField 3 DPUs |         |


## Hands-on Examples

Please see the examples under the `/code` folder. The tutorial slides will have additional details on running and interacting with each example.

## Slides

Slides will be shared with tutorial attendees and shared more broadly after SC23.
