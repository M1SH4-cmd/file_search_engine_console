#ifndef FILE_SEARCH_DRIVE_CONSOLE_V_SEARCH_DRIVE_CPP
#define FILE_SEARCH_DRIVE_CONSOLE_V_SEARCH_DRIVE_H

#include "json_parser.h"
#include <nlohmann\json.hpp>
#include <map>
#include <vector>
#include <thread>
#include <ctime>

using json = nlohmann::json;
class SearchDrive {

public:

    SearchDrive ();
    SearchDrive (std::string root);

    std::map<std::string, std::string> search(std::vector<std::string> snames);

    bool update_data(CFG &cfg);

    void set_root_dir(std::string root);
    std::string get_root_dir();

private:
    CFG cfg;
    std::vector<std::string> snames;
    std::string root_dir;
    std::map<std::string, std::string> result;

};

#endif //FILE_SEARCH_DRIVE_CONSOLE_V_SEARCH_DRIVE_CPP