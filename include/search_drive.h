#ifndef FILE_SEARCH_DRIVE_CONSOLE_V_SEARCH_DRIVE_CPP
#define FILE_SEARCH_DRIVE_CONSOLE_V_SEARCH_DRIVE_H

#include "json_parser.h"
#include <nlohmann\json.hpp>
#include <map>
#include <vector>

using json = nlohmann::json;
class SearchDrive {

public:

    SearchDrive(){};

    bool parse_data();
    std::map<std::string, std::string> search(std::vector<std::string> snames);
    bool update_data();

private:
    CFG cfg;
    std::vector<std::string> snames;

};






#endif //FILE_SEARCH_DRIVE_CONSOLE_V_SEARCH_DRIVE_CPP
