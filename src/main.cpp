#include "search_drive.h"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

void printAnimatedWelcome() {
    std::string welcome = R"(
*===============================*
|  File search drive (console)  |
|  Version: 1.0                 |
|  Made by: OG_Michael          |
*===============================*


)";

    for (char c : welcome) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    std::cout << std::endl;
}

void search_exec(SearchDrive &drive) {
    std::vector<std::string> files_to_search;
    std::string input;

    std::cout << "Selected directory: " << drive.get_root_dir() << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  - Enter file names to search" << std::endl;
    std::cout << "  - Type 'DIR' to change source directory" << std::endl;
    std::cout << "  - Type 'DONE' to start search" << std::endl;
    std::cout << "  - Type 'EXIT' to quit" << std::endl << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "DONE") {
            if (files_to_search.empty()) {
                std::cout << "No files specified for search. Please enter at least one file name." << std::endl;
                continue;
            }
            break;
        }
        else if (input == "DIR") {
            std::cout << "Enter new directory path: ";
            std::getline(std::cin, input);
            drive.set_root_dir(input);
            std::cout << "Directory changed to: " << drive.get_root_dir() << std::endl;
        }
        else if (input == "EXIT") {
            std::cout << "Goodbye!" << std::endl;
            exit(0);
        }
        else if (!input.empty()) {
            files_to_search.push_back(input);
            std::cout << "Added to search: '" << input << "'" << std::endl;
            std::cout << "Total files to search: " << files_to_search.size() << std::endl;
        }
    }

    std::cout << std::endl << "Starting search for " << files_to_search.size() << " files..." << std::endl;
    std::cout << "This may take a while..." << std::endl << std::endl;

    auto start_time = std::chrono::steady_clock::now();

    std::map<std::string, std::string> res = drive.search(files_to_search);

    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << std::endl << "=== SEARCH RESULTS ===" << std::endl;
    std::cout << "Search completed in " << duration.count() << " ms" << std::endl;
    std::cout << "Found " << res.size() << " files:" << std::endl << std::endl;

    if (res.empty()) {
        std::cout << "No files found." << std::endl;
    } else {
        for (const auto& found_file : res) {
            std::cout << "✓ " << found_file.first << " -> " << found_file.second << std::endl;
        }
    }

    std::cout << std::endl << "Press Enter to continue...";
    std::cin.get();
}

int main(int argc, char** argv) {
    // Проверка конфигурационного файла
    std::ifstream cfg("config.json");
    if (!cfg.is_open()) {
        std::cerr << "Warning: config.json not found. Using default settings." << std::endl;
    } else {
        cfg.close();
    }

    // Очистка экрана
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    printAnimatedWelcome();

    try {
        SearchDrive drive;

        if (argc > 1) {
            drive.set_root_dir(argv[1]);
            std::cout << "Using command line directory: " << drive.get_root_dir() << std::endl;
        }

        while (true) {
            search_exec(drive);

            std::cout << std::endl << "Start new search? (y/n): ";
            std::string choice;
            std::getline(std::cin, choice);

            if (choice != "y" && choice != "Y") {
                std::cout << "Thank you for using File Search Drive!" << std::endl;
                break;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}