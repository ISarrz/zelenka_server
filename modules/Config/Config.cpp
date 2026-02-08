#include "Config.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace fs = std::filesystem;

void config::SetConfig(const std::filesystem::path &project_path) {
    config_path = project_path.parent_path().parent_path()/ "data" / "config.json";

    std::cout << "Setting config..." << std::endl;
    json data;
    std::cout << "Yandex key: ";
    std::string value;
    std::cin >> value;
    data["yandex_key"] = value;

    std::cout << "Yandex secret key: ";
    std::cin >> value;
    data["yandex_secret_key"] = value;

    std::cout << "Database username: ";
    std::cin >> value;
    data["database_username"] = value;

    std::cout << "Database password: ";
    std::cin >> value;
    data["database_password"] = value;

    std::ofstream out(config_path);
    out << data.dump(4);
    std::cout << "Done." << std::endl;
}

void config::InitConfig(const std::filesystem::path &project_path) {
    config_path = project_path.parent_path().parent_path()/ "data" / "config.json";
    yandex_key = get_config_field("yandex_key");
    yandex_secret_key = get_config_field("yandex_secret_key");
    database_username = get_config_field("database_username");
    database_password = get_config_field("database_password");
}

std::string config::get_config_field(const std::string &key) {
    std::ifstream file(config_path);
    // if (!file.is_open()) {
    //     std::cerr << "Could not open config.json" << std::endl;
    //     return "";
    // }

    json data;
    file >> data;

    return data[key];
}