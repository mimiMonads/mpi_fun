// BSCH - Distributed Systems
// Assignment 1

#include <iostream>
#include <fstream>
#include <mpi.h>
#include <string>
#include <cstdlib>
#include <random>
#include <vector>
#include "../dotenv.hpp"

// Global variables
const int arraySize = 120;
int world_size, world_rank; 
int numberArray[arraySize];

int studentNumber;
std::string fullName;

int main(int argc, char** argv) {

    
    // Load env
    MPI_Init(&argc, &argv);
    dotenv::load();
    // Read env
    const char* sn_c    = std::getenv("STUDENT_NUMBER");
    const char* fn_c    = std::getenv("FULL_NAME");

    fullName        = fn_c ? fn_c : "";

    int studentNumber = 0;


    if (sn_c && *sn_c) {
        try { studentNumber     = std::stoi(sn_c); }
        catch (...) { std::cerr << "Invalid STUDENT_NUMBER\n"; studentNumber = 12345678; }
    } else {
        studentNumber = 12345678;
    }

    int worldRank   = 0;
    int worldSize   = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);


    const bool dividesEvenly = (arraySize % worldSize) == 0;

    // Task A -> Fails if it can dividsed evenly
    if (!dividesEvenly) {
        if (worldRank == 0) {
            std::cerr << "Error: world size " << worldSize
                      << " does not evenly divide array size " << arraySize << std::endl;
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    const int elementsPerRank = arraySize / worldSize;
    std::vector<int> localNumbers(elementsPerRank);

    if (worldRank == 0) {

        std::cout << "Name: " << fullName
            << " | Student Number: " << studentNumber << std::endl;
        std::mt19937 rng(static_cast<unsigned int>(studentNumber));
        std::uniform_int_distribution<int> dist(1, 1000);
        for (int i = 0; i < arraySize; ++i) {
            numberArray[i] = dist(rng);
        }
    }

    MPI_Scatter(numberArray, elementsPerRank, MPI_INT,
                localNumbers.data(), elementsPerRank, MPI_INT,
                0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    std::cout << "Rank " << worldRank << " subset:";
    for (int value : localNumbers) {
        std::cout << ' ' << value;
    }
    std::cout << std::endl;

    MPI_Finalize();
    return 0;
}
