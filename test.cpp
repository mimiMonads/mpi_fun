#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "dotenv.hpp"

int main() {
    dotenv::load();

    const char* api_key = std::getenv("SANITY_API_KEY");
    const char* project = std::getenv("SANITY_PROJECT_ID");

    if (api_key && project) {
        std::cout << "uwu" << project << std::endl;
        std::cout << "Using API key: " << api_key << std::endl;
    } else {
        std::cout << "Oh no!" << std::endl;
    }
}
