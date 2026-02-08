cmake_minimum_required(VERSION 3.25)


project(zelenka_server)

set(CMAKE_CXX_STANDARD 20)

# Добавляем путь, который нашел find
include_directories(/usr/include/jdbc)
include_directories(/usr/include/mysql-cppconn)

# Поиск nlohmann_json
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    # Если пакет не найден (как в старых Debian), просто ищем заголовки
    include_directories(/usr/include/nlohmann)
endif()
# Или, если папки jdbc нет физически, можно сделать обходной путь (хак):
#include_directories(/usr/include)

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