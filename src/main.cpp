#include "search_drive.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

void printAnimatedWelcome() {
    std::string welcome = "*===============================*\n"
                          "|  File search drive (console)  |\n"
                          "|  Version: 0.1                 |\n"
                          "|  Made by: OG_Michael          |\n"
                          "*===============================*\n";

    for (char c : welcome) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    std::fstream cfg("config.json");
    if (!cfg.is_open()) {
        std::cerr << "Error opening file config.json" << std::endl;
        exit(-1);
    };

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printAnimatedWelcome();
//    std::string path;
//    if (argc > 1) {
//        path = argv[1];
//    }
    return 0;
}
