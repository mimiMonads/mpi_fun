// Distributed Systems - 2026
// Assignment 2 - template
// TODO: Student Name - Student Number

#include <iostream>
#include <fstream>
#include <mpi.h>
#include <string>

//Global variables
const int arraySize = 550;
int world_size, world_rank;
int key;
char totalArray[arraySize];

//createData helper method - Do not modify
void createData(int rank) {
    /* DATA METHOD
    This method populates an array with the text read from file.
    */

    // File must be saved in the same directory as the executable of this program
    std::string nodeFileName = "ciphertext.txt"; //read your corresponding .txt file
    std::ifstream MyReadFile(nodeFileName);
    // Only 1 node need read the file, output to console
    std::cout << "Node " << rank << " reading file: " << nodeFileName << std::endl;
    //check if file is read correctly
    if (!MyReadFile.is_open()) {
        std::cerr << "Unable to open file: " << nodeFileName << std::endl;
        std::cerr << "Make sure ciphertext.txt is in the same directory as your executable." << std::endl;
        return;
    }
    else{
        MyReadFile.getline(totalArray, arraySize);
        std::cout << "File read successfully." << std::endl;
    }
    // Close the file
    MyReadFile.close();
}
//decryptText helper method - Do not modify
char* decryptText(int key, char* arr, int size) {
    int length, i;
    char c;
    //loop through each string in array
    for (int j = 0; j < size; j++) {
        c = arr[j];
        //if character is in alphabet, convert to uppercase
        if (isalpha(c)) {
            c = toupper(c);
            //decrypt using formula (c - key % 26)
            c = 'A' + (((c - 'A') - key + 26) % 26);
            arr[j] = c;
        }
    }
    return arr;
}
//searchText helper method - Do not modify
int searchText(char* arr, int size, const char* needle) {
    int hits = 0;
    char* found = arr;
    //search given array for the needle
    while ((found = strstr(found, needle)) != NULL && found - arr < size) {
        hits++;
        found++;
    }
    return hits;
}
//exportText helper method - Do not modify
void exportText(char* arr, int size, const char* filename) {
    std::ofstream outfile(filename); //create new file
    outfile.write(arr, size); //write array to file
    outfile.close(); //close the file
}

int main(int argc, char** argv) {
    // TODO: Work to be done here

   

}