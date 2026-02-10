#include "../modules/Config/Config.h"
#include "../modules/Server/Server.h"
#include <iostream>
#include <memory>

void ShowHelp(const std::string &exeName)
{
    std::cout << "Usage: " << exeName << " <command> [options]\n\n";
    std::cout << "Commands (required):\n";
    std::cout << "  config         Set config (required before first run)\n";
    std::cout << "  server         Start server\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help     Show this help message\n\n";
}

int main(int argc, char *argv[])
{

    std::string exeName = argv[0];

    if (argc == 2 && (argv[1] == "-h" || argv[1] == "--help"))
    {
        ShowHelp(argv[0]);
        return 0;
    }

    config::InitConfig(exeName);

    Server();

    return 0;
}