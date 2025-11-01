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

    const int partitionSize = arraySize / worldSize;
    std::vector<int> partition(partitionSize);

    if (worldRank == 0) {

        std::cout << "Name: " << fullName
            << " | Student Number: " << studentNumber << std::endl;

        // Using the Mersenne Twister 19937 generator as generator
        // This ensures that given a seed it will always return the same sequence
        // In this case the range of possible values are all `unsigned int`
        std::mt19937 rng(static_cast<unsigned int>(studentNumber));

        // In order to adhere to a range , we need a distribution, in this case from 1 to 1000
        // Otherwise `numberArray` will contain any range of uint
        std::uniform_int_distribution<int> dist(1, 1000);
        for (int i = 0; i < arraySize; ++i) {
            numberArray[i] = dist(rng);
        }
    }

    // Scatter the root's full array so every rank receives its partition
    MPI_Scatter(
        numberArray,           // Provides the full array
        partitionSize,         // Number of integers
        MPI_INT,               // Datatype
        partition.data(),      // Receive buffer
        partitionSize, MPI_INT,// How many integers each rank receives and their type
        0,                     // Marks ownership for root
        MPI_COMM_WORLD);       // Communicator 

    const int nodeNumber = partition.empty() 
        ? 0 
        : partition.front();
        
    std::vector<int> gatheredNumbers;

    if (worldRank == 0) {
        gatheredNumbers.resize(worldSize);
    }

    // Gather one number from each rank back to root for aggregation
    MPI_Gather(&nodeNumber,              // Address of this rank s value to send
               1, MPI_INT,               // Send a single integer from each process
               // Root receives all values; others pass nullptr
               worldRank == 0 
                ? gatheredNumbers.data() 
                : nullptr,
               1, MPI_INT,               // Each received value is a single integer
               0,                        // Marks ownership for root
               MPI_COMM_WORLD);          // Communicator 

    if (worldRank == 0) {

        std::cout << "Collected node numbers:";

        for (int value : gatheredNumbers) {
            std::cout << ' ' << value;
        }

        std::cout << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    std::cout << "Rank " << worldRank << " subset:";
    for (int value : partition) {
        std::cout << ' ' << value;
    }
    std::cout << std::endl;

    MPI_Finalize();
    return EXIT_SUCCESS;
}
