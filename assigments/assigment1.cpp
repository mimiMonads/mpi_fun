// BSCH - Distributed Systems
// Assignment 1

#include <iostream>
#include <fstream>
//#include <mpi.h>
#include <string>
#include <cstdlib>
#include "../dotenv.hpp"

// Global variables
const int arraySize = 120;
int world_size, world_rank; 
int numberArray[arraySize];

std::string yourStudentNumber;
std::string yourFullName;

int main(int argc, char** argv) {

    // Load env
    dotenv::load();
    // Read env
    const char* sn_c = std::getenv("STUDENT_NUMBER");
    const char* fn_c = std::getenv("FULL_NAME");

    yourStudentNumber = sn_c ? sn_c : "";
    yourFullName      = fn_c ? fn_c : "";

    
    std::cout << yourFullName << '\n';
    std::cout << yourStudentNumber << '\n';

    return 0;
}