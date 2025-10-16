#include "search_drive.h"

SearchDrive::SearchDrive() {
    cfg = JsonParser::instance().json_cfg();
    snames = cfg.files_to_search;
};

SearchDrive::SearchDrive(std::string root) {
    SearchDrive();

    if (root_dir.empty()) {
        root_dir = "C:/";
    } else {
        root_dir = root;
    }
};

std::map<std::string, std::string> SearchDrive::search(std::vector<std::string> snames) {

    return respond;
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