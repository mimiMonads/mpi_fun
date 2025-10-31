
#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {

    // Give args to mpi
    MPI_Init(&argc, &argv);
    int world_size = 0;
    int world_rank = 0;

    // can be change with the flag -np $number
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len = 0;

    // Gets the world size, 
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // Gets our rank
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    // Gets the process name 
    MPI_Get_processor_name(processor_name, &name_len);


    std::cout << "world size is: " << world_size << "\n";
    std::cout << "world rank is: " << world_rank
              << " on " << processor_name << "\n";

    // rank 0 is Main
    if (world_rank == 0) {

        for (int i = 1; i < world_size; ++i)
            MPI_Send(
                &world_rank, 
                1, 
                MPI_INT, 
                i, 
                0, 
                MPI_COMM_WORLD);
    } else {
        
        int data = -1;
        MPI_Recv(
            &data, 
            1, 
            MPI_INT, 
            0, 
            0, 
            MPI_COMM_WORLD, 
            MPI_STATUS_IGNORE
        );
        std::cout << "rank " << world_rank
                  << " received data from rank " << data << "\n";
    }

    MPI_Finalize();
    return 0;
}
