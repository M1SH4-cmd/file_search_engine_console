#ifndef FILE_SEARCH_DRIVE_CONSOLE_V_JSON_PARSER_H
#define FILE_SEARCH_DRIVE_CONSOLE_V_JSON_PARSER_H
#include <nlohmann/json.hpp>
#include <mutex>
#include <fstream>
#include <iostream>
#include <thread>
#include <map>
#include <vector>

using json = nlohmann::json;

struct CFG {
    CFG(){};
    std::vector<std::string> files_to_search;
    std::map<std::string, std::string> respond;
    std::string version;
    std::string app_name;
};

class JsonParser {
public:
    static JsonParser &instance();
    //bool parse_arrays();
    CFG &json_cfg();
    bool load_data(CFG& cfg);
    void upload_data (CFG& cfg);

private:
    CFG cfg;
    static std::mutex mtx;
    JsonParser() = default;
    JsonParser(const JsonParser& parser) = delete;
    JsonParser& operator=(const JsonParser&) = delete;
};


#endif //FILE_SEARCH_DRIVE_CONSOLE_V_JSON_PARSER_H
