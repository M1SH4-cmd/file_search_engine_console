#include "search_drive.h"

namespace fs = std::filesystem;

SearchDrive::SearchDrive() {
    cfg = JsonParser::instance().json_cfg();
    snames = cfg.files_to_search;
    root_dir = "C:/";
};

SearchDrive::SearchDrive(std::string root) {
    cfg = JsonParser::instance().json_cfg();
    snames = cfg.files_to_search;

    if (root.empty()) {
        root_dir = "C:/";
    } else {
        root_dir = root;
    }
};

std::map<std::string, std::string> SearchDrive::search(std::vector<std::string> snames) {
    std::map<std::string, std::string> result;

    if (snames.empty() || root_dir.empty()) {
        return result;
    }

    // Создание потокобезопасной структуры
    std::mutex result_mutex;
    std::atomic<bool> search_completed{false};

    // Функция для поиска одного файла
    auto search_file = [&](const std::string& filename) -> std::pair<std::string, std::string> {
        if (search_completed) {
            return {filename, ""};
        }

        try {
            // Рекурсивный обход директории
            for (const auto& entry : fs::recursive_directory_iterator(
                    root_dir,
                    fs::directory_options::skip_permission_denied
            )) {
                if (search_completed) {
                    break;
                }

                if (entry.is_regular_file()) {
                    const std::string& current_filename = entry.path().filename().string();

                    // Точное сравнение имен файлов (регистрозависимое)
                    if (current_filename == filename) {
                        return {filename, entry.path().string()};
                    }

                    // Дополнительно: поиск по частичному совпадению
                    if (current_filename.find(filename) != std::string::npos) {
                        return {filename, entry.path().string()};
                    }

                }
            }
        } catch (const fs::filesystem_error&) {
            // Игнор ошибки доступа
        }

        return {filename, ""};
    };

    // Запуск поиска каждого файла в отдельном потоке
    std::vector<std::future<std::pair<std::string, std::string>>> futures;

    for (const auto& filename : snames) {
        futures.push_back(std::async(std::launch::async, search_file, filename));
    }

    // Сборка результатов
    for (auto& future : futures) {
        auto [filename, filepath] = future.get();

        std::lock_guard<std::mutex> lock(result_mutex);
        if (!filepath.empty()) {
            result[filename] = filepath;
        }
    }

    search_completed = true;
    return result;
}

void SearchDrive::set_root_dir(std::string root) {
    root_dir = root;
}

std::string SearchDrive::get_root_dir() {
    return root_dir;
}

void SearchDrive::set_files_to_search(std::vector<std::string> snames) {
    this->snames = snames;
}

bool SearchDrive::update_data(CFG &cfg) {
    JsonParser::instance().json_cfg() = cfg;
}