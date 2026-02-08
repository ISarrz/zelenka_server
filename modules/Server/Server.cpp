#include "Server.h"

#include "../Database/Database.h"
#include "../Database/User/AbstractUserRepository.h"
#include "../Database/User/User.h"
#include "../Database/User/UserRepository.h"

Server::Server() {
    auto *database = new Database();
    auto user_repo = UserRepository(database);

    httplib::Server svr;

    svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("Hello World!", "text/plain");
    });

    svr.Post("/register", [](const httplib::Request &req,
                             httplib::Response &res) {
        try {
            auto data = json::parse(req.body);

            std::string login = data.at("login");
            std::string email = data.at("email");
            std::string password = data.at("password");
            std::cout << "Регистрация пользователя: " << login << std::endl;

            if (login == "admin") {
                json error_response;
                error_response["message"] = "User already exists";

                res.status = 409;
                res.set_content(error_response.dump(), "application/json");
                return;
            }

            json response_user;
            response_user["id"] = 101;
            response_user["login"] = login;
            response_user["email"] = email;

            res.status = 201;
            res.set_content(response_user.dump(), "application/json");

        } catch (const std::exception &e) {
            std::cout << "error\n";
            res.status = 400;
            res.set_content("{\"error\":\"Invalid JSON\"}", "application/json");
        }
    });

    svr.Post("/sensor_monitoring",
             [](const httplib::Request &req, httplib::Response &res) {
                 try {
                     auto data = json::parse(req.body);
                     int id = data.at("id");
                     float temperature = data.at("temperature");
                     float humidity = data.at("humidity");
                     float hydration = data.at("hydration");
                     float pressure = data.at("pressure");
                     std::cout << "Данные получены:\n"
                               << "ID: " << id << "\n"
                               << "Температура " << temperature << "\n"
                               << "Влажность воздуха: " << humidity << "\n"
                               << "Влажность почвы: " << hydration << "\n"
                               << "Давление: " << pressure << "\n\n";

                     json response_user;

                     res.status = 201;
                     res.set_content(response_user.dump(), "application/json");

                 } catch (const std::exception &e) {
                     res.status = 400;
                     std::cout << "error\n";
                     std::cout << e.what() << "\n\n";
                 }
             });

    svr.Post("/auth",
             [&user_repo](const httplib::Request &req, httplib::Response &res) {
                 try {
                     auto data = json::parse(req.body);

                     std::string info = data.at("info");
                     std::string password = data.at("password");
                     std::optional<User> user;
                     json response;
                     if (info.find('@') == std::string::npos) {
                         user = user_repo.getByEmail(info);

                     } else {
                         user = user_repo.getByLogin(info);
                     }
                     if (user && user.value().password == password) {
                         res.status = 200;
                     } else {
                         res.status = 401;
                     }


                 } catch (const std::exception &e) {
                     // std::cout << "error\n";
                     // res.status = 400;
                     // res.set_content("{\"error\":\"Invalid JSON\"}",
                     // "application/json");
                 }
             });

    std::cout << "Сервер запущен на http://0.0.0.0:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
}