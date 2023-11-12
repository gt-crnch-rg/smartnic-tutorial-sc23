#include <mpi.h>

extern char name[MPI_MAX_PROCESSOR_NAME];
extern MPI_Comm hosts_communicator;
extern MPI_Comm nic_host_communicator;
extern int isHost;
extern int x_is_ready;

extern MPI_Win win_numneigh;
extern MPI_Win win_neighbors;
extern MPI_Win win_x;
extern MPI_Win win_type;
extern MPI_Win win_f;
extern MPI_Win win_sorted_index;

extern MPI_Aint f_address;



//#define BF


