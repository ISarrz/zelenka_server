#include "../modules/Config/Config.h"
#include "../modules/Database/Database.h"
#include "../modules/Database/User/UserRepository.h"
#include "../modules/Server/Server.h"
#include <iostream>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <memory>

void ShowHelp(const std::string &exeName) {
    std::cout << "Usage: " << exeName << " <command> [options]\n\n";
    std::cout << "Commands (required):\n";
    std::cout << "  config         Set config (required before first run)\n";
    std::cout << "  server         Start server\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help     Show this help message\n";
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cout << "Wrong number of arguments" << std::endl;
        return 0;
    }

    std::string exeName = argv[0];
    config::InitConfig(exeName);

    std::string parameter = argv[1];
    if (parameter == "--help") {
        ShowHelp(argv[0]);
        return 0;
    }

    if (parameter == "config") {
        config::SetConfig(exeName);
        return 0;
    }

    if (!std::filesystem::exists(config::config_path)) {
        std::cerr << "Error: Config not found. Please run \"" << exeName
                  << " config\" first.\n";
        return 1;
    }

    if (parameter == "server") {
        Server();
    }

    return 0;
}