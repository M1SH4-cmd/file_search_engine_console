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
    std::cout << "Enter file names to search (one per line). Type 'DONE' to start search:" << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "DONE") {
            if (files_to_search.empty()) {
                std::cout << "No files specified. Please enter file names." << std::endl;
                continue;
            }
            break;
        }
        else if (!input.empty()) {
            files_to_search.push_back(input);
            std::cout << "Added: '" << input << "'" << std::endl;
        }
    }

    std::cout << std::endl << "Starting search..." << std::endl;

    auto start_time = std::chrono::steady_clock::now();

    std::map<std::string, std::string> res = drive.search(files_to_search);

    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << std::endl << "=== SEARCH RESULTS ===" << std::endl;
    std::cout << "Time: " << duration.count() << " ms" << std::endl;
    std::cout << "Found: " << res.size() << " files" << std::endl << std::endl;

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
            std::cout << "Using directory: " << drive.get_root_dir() << std::endl;
        }

        // Тестовый поиск для отладки
        std::cout << "=== TEST MODE ===" << std::endl;
        std::vector<std::string> test_files = {"notepad.exe", "cmd.exe", "explorer.exe"};
        std::cout << "Testing search for common Windows files..." << std::endl;

        auto result = drive.search(test_files);
        std::cout << "Test found: " << result.size() << " files" << std::endl;

        for (const auto& file : result) {
            std::cout << "FOUND: " << file.first << " -> " << file.second << std::endl;
        }

        std::cout << "=== END TEST ===" << std::endl << std::endl;

        // Основной поиск
        search_exec(drive);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}