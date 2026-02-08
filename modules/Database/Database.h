#pragma once
#include "Database.h"
#include <iostream>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <memory>

class Database {
  public:
    Database();

    static int makeBackup();
    static int loadBackup();

    [[nodiscard]] sql::Connection* getConnection() const;

  private:
    sql::mysql::MySQL_Driver *driver_;
    std::unique_ptr<sql::Connection> connection_;
};
