#include "../modules/Config/Config.h"
#include "../modules/Database/Database.h"
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
    std::string exeName = argv[0];
    config::InitConfig(exeName);

    const Database *database = new Database();
    if (database->makeBackup()) {
        std::cout << "Error";
    }
    return 0;

    if (argc != 2) {
        std::cout << "Wrong number of arguments" << std::endl;
        return 0;
    }

    std::string parameter;
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

    try {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;
        sql::PreparedStatement *pstmt;

        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "Sql_322302!");

        con->setSchema("sensor_db");

        pstmt = con->prepareStatement(
                "INSERT INTO measurements(sensor_name, temperature, humidity) "
                "VALUES (?, ?, ?)");

        pstmt->setString(1, "Sensor_CPP_01");
        pstmt->setDouble(2, 22.5);
        pstmt->setDouble(3, 55.2);

        pstmt->executeUpdate();

        std::cout << "Data inserted successfully!" << std::endl;

        delete pstmt;
        delete con;

    } catch (sql::SQLException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}