#include "search_drive.h"
#include <filesystem>
#include <thread>
#include <future>
#include <atomic>
#include <mutex>
#include <iostream>
#include <windows.h>

namespace fs = std::filesystem;

SearchDrive::SearchDrive() {
    cfg = JsonParser::instance().json_cfg();
    snames = cfg.files_to_search;
    root_dir = "C:/";
}

SearchDrive::SearchDrive(std::string root) {
    cfg = JsonParser::instance().json_cfg();
    snames = cfg.files_to_search;

    if (root.empty()) {
        root_dir = "C:/";
    } else {
        root_dir = root;
    }
}

std::map<std::string, std::string> SearchDrive::search(std::vector<std::string> search_names) {
    std::map<std::string, std::string> result;

    std::cout << "DEBUG: Starting search in directory: " << root_dir << std::endl;
    std::cout << "DEBUG: Files to search: ";
    for (const auto& file : search_names) {
        std::cout << "'" << file << "' ";
    }
    std::cout << std::endl;

    if (search_names.empty()) {
        std::cout << "DEBUG: No files to search" << std::endl;
        return result;
    }

    if (root_dir.empty()) {
        std::cout << "DEBUG: Root directory is empty" << std::endl;
        return result;
    }

    // Проверяем существование корневой директории
    if (!fs::exists(root_dir)) {
        std::cout << "DEBUG: Directory does not exist: " << root_dir << std::endl;
        return result;
    }

    if (!fs::is_directory(root_dir)) {
        std::cout << "DEBUG: Path is not a directory: " << root_dir << std::endl;
        return result;
    }

    std::mutex result_mutex;
    std::atomic<bool> search_completed{false};
    std::atomic<int> files_found{0};
    std::atomic<int> directories_scanned{0};

    // Функция для поиска одного файла с ручной рекурсией
    auto search_file = [&](const std::string& filename) -> std::pair<std::string, std::string> {
        if (search_completed) {
            return {filename, ""};
        }

        std::cout << "DEBUG: Starting search for file: " << filename << std::endl;

        // Используем стек для ручной рекурсии вместо recursive_directory_iterator
        std::vector<fs::path> directories_to_search;
        directories_to_search.push_back(root_dir);

        while (!directories_to_search.empty() && !search_completed) {
            fs::path current_dir = directories_to_search.back();
            directories_to_search.pop_back();

            try {
                for (const auto& entry : fs::directory_iterator(
                        current_dir,
                        fs::directory_options::skip_permission_denied
                )) {
                    if (search_completed) {
                        break;
                    }

                    try {
                        if (entry.is_directory()) {
                            // Проверяем, не является ли это симлинком или точкой монтирования
                            bool is_symlink = entry.is_symlink();
                            bool is_reparse = false;

#ifdef _WIN32
                            // На Windows проверяем атрибуты файла
                            DWORD attrs = GetFileAttributesW(entry.path().c_str());
                            if (attrs != INVALID_FILE_ATTRIBUTES) {
                                is_reparse = (attrs & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
                            }
#endif

                            // Пропускаем симлинки и точки монтирования чтобы избежать циклов
                            if (!is_symlink && !is_reparse) {
                                directories_to_search.push_back(entry.path());
                            }
                            directories_scanned++;
                        }
                        else if (entry.is_regular_file()) {
                            const std::string& current_filename = entry.path().filename().string();

                            // Точное сравнение имен файлов
                            if (current_filename == filename) {
                                std::cout << "DEBUG: FOUND: " << filename << " at " << entry.path().string() << std::endl;
                                files_found++;
                                return {filename, entry.path().string()};
                            }
                        }
                    }
                    catch (const fs::filesystem_error& e) {
                        // Игнорируем ошибки для отдельных файлов/папок
                        continue;
                    }
                }
            }
            catch (const fs::filesystem_error& e) {
                // Пропускаем директории с ошибками доступа
                std::cout << "DEBUG: Cannot access directory: " << current_dir << " - " << e.what() << std::endl;
                continue;
            }
        }

        std::cout << "DEBUG: File not found: " << filename << std::endl;
        return {filename, ""};
    };

    // Запускаем поиск в отдельных потоках
    std::vector<std::future<std::pair<std::string, std::string>>> futures;

    for (const auto& filename : search_names) {
        if (!filename.empty()) {
            futures.push_back(std::async(std::launch::async, search_file, filename));
        }
    }

    // Собираем результаты
    for (auto& future : futures) {
        try {
            auto search_result = future.get();
            auto filename = search_result.first;
            auto filepath = search_result.second;

            if (!filepath.empty()) {
                std::lock_guard<std::mutex> lock(result_mutex);
                result[filename] = filepath;
            }
        }
        catch (const std::exception& e) {
            std::cout << "DEBUG: Error in future: " << e.what() << std::endl;
        }
    }

    search_completed = true;

    std::cout << "DEBUG: Search completed. Found " << files_found << " files." << std::endl;
    return result;
}

void SearchDrive::set_root_dir(std::string root) {
    root_dir = root;
    std::cout << "DEBUG: Root directory set to: " << root_dir << std::endl;
}

std::string SearchDrive::get_root_dir() {
    return root_dir;
}

void SearchDrive::set_files_to_search(std::vector<std::string> search_names) {
    this->snames = search_names;
}