#include "json_parser.h"

std::mutex JsonParser::mtx;

JsonParser &JsonParser::instance() {
    static JsonParser parser;
    return parser;
}

CFG &JsonParser::json_cfg() {
    return cfg;
}

bool JsonParser::load_data(CFG &cfg) {
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream file("config.json");
    if (!file.is_open()) {
        cfg = CFG();
        return false;
    };

    json j;
    try {
        file >> j;
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    }
    file.close();

    if (j.contains("config")) {
        const auto& config = j["config"];

        cfg.app_name = config["name"];
        cfg.version = config["version"];

        if (config.contains("files_to_search")) {
            try {
                json files_to_search = json::array();
                files_to_search = config["files_to_search"];

                for (auto f : files_to_search) {
                    cfg.files_to_search.push_back(f);
                }
            } catch (const json::exception& e) {
                std::cerr << "JSON parse error: " << e.what() << std::endl;
                return false;
            }
        } else {
            std::cerr << "No files to search given" << std::endl;
            return false;
        }
    } else {
        std::cerr << "JSON parsing error!" << std::endl;
        return false;
    }
    return true;
}

void JsonParser::upload_data(CFG &cfg) {
    json j;
    std::lock_guard<std::mutex> lock(mtx);

    json resp = json::array();

    for (auto r : cfg.respond) {
        std::string name = "";
        std::string path = "";
        resp.push_back({name, path});
    }

    j["config"] = {
            {"respond", resp}
    };

    std::ofstream file("config.json");
    file << j.dump(4);

    if (!file.is_open()) {
        cfg = CFG();
        return;
    };
    file.close();
}