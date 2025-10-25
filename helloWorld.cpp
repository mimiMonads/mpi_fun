#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size = 0;
    int world_rank = 0;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);     // total processes
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);     // my rank
    MPI_Get_processor_name(processor_name, &name_len);

    std::cout << "Hello from rank " << world_rank
              << " of " << world_size
              << " on " << processor_name << "\n";

    MPI_Finalize();
    return 0;
}