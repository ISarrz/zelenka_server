cmake_minimum_required(VERSION 3.28)
project(zelenka_server)

set(CMAKE_CXX_STANDARD 20)

# Добавляем путь, который нашел find
include_directories(/usr/include/mysql-cppconn)

add_executable(zelenka_server scripts/main.cpp
        modules/Config/Config.cpp
        modules/Database/Database.cpp
        modules/Database/Database.h
        modules/Server/Server.cpp
        modules/Server/Server.h
        modules/Database/User/User.h
        modules/Database/User/UserRepository.cpp
        modules/Database/User/UserRepository.h
        modules/Database/User/AbstractUserRepository.h)

# Линкуем библиотеку
target_link_libraries(zelenka_server mysqlcppconn)