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
                          "*===============================*\n\n\n";

    for (char c : welcome) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    std::cout << std::endl;
}

void search_exec(SearchDrive &drive) {
    std::string input;
    std::vector<std::string> files_to_search;
    std::cout << "Selected dir: " << drive.get_root_dir() <<
              "\nInput file names to search or type 'DIR' to change source dir\nType 'DONE' when finished:"
              << std::endl;
    std::string input1;
    std::cin >> input1;

    if (input1 == "DIR") {
        std::cout << "Type in new dir:" << std::endl;
        std::cin >> input1;
        drive.set_root_dir(input1);
    } else  if (input1 == "DONE"){
        return;
    } else {
        files_to_search.push_back(input1);
    }

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "DONE") {
            break;
        }

        if (input.empty()) {
            continue;
        }

        files_to_search.push_back(input);

        std::cout << "< Файл добавлен в поиск >: " << input << "\n";
    }
    drive.
    std::map<std::string, std::string> res = drive.search(files_to_search);

    for(auto found_file : res) {
        std::cout << "\n> " << found_file.first << "\t" << found_file.second;
    }
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

    if (argc > 1) {
        SearchDrive drive(argv[1]);
        //search_exec(drive);
    } else {
        SearchDrive drive;
        //search_exec(drive);
    }

        return 0;
    }
