#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

enum MessageType { DO_WORK = 1, SEND_RESULT, SHUTDOWN };

void master(int world_size) {
    for (int i = 1; i < world_size; ++i)
        MPI_Send(nullptr, 0, MPI_INT, i, DO_WORK, MPI_COMM_WORLD);

    std::vector<double> results(world_size - 1);
    for (int i = 1; i < world_size; ++i)
        MPI_Recv(&results[i - 1], 1, MPI_DOUBLE, i, SEND_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    double total = 0.0;
    for (double v : results) total += v;
    std::cout << "Overall average: " << (total / (world_size - 1)) << "\n";

    for (int i = 1; i < world_size; ++i)
        MPI_Send(nullptr, 0, MPI_INT, i, SHUTDOWN, MPI_COMM_WORLD);
}

void slave(int rank) {
    std::srand(static_cast<unsigned>(time(nullptr)) + rank);
    while (true) {
        MPI_Status status;
        MPI_Recv(nullptr, 0, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == SHUTDOWN) break;
        if (status.MPI_TAG == DO_WORK) {
            double sum = 0.0;
            for (int i = 0; i < 100; ++i) sum += std::rand() % 100;
            double avg = sum / 100.0;
            MPI_Send(&avg, 1, MPI_DOUBLE, 0, SEND_RESULT, MPI_COMM_WORLD);
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int size, rank; MPI_Comm_size(MPI_COMM_WORLD, &size); MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) master(size); else slave(rank);
    MPI_Finalize();
    return 0;
}
