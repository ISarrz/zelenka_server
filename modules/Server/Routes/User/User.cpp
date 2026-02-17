#include <iostream>
#include <nlohmann/json.hpp>

#include "../../../Config/Config.hpp"
#include "../../../Database/Database.hpp"
#include "../Routes.hpp"
using json = nlohmann::json;

void Routes::RegisterUserRoutes(httplib::Server& svr,
                                UserRepository& user_repo) {
    svr.Post(
        "/register", [&](const httplib::Request& req, httplib::Response& res) {
            try {
                auto data = json::parse(req.body);

                if (!data.contains("login")) {
                    res.status = 400;
                    json response;
                    response["status"] = "error";
                    response["message"] = "Missing login field";
                    res.set_content(response.dump(), "application/json");
                    return;
                }

                if (!data.contains("email")) {
                    res.status = 400;
                    json response;
                    response["status"] = "error";
                    response["message"] = "Missing email field";
                    res.set_content(response.dump(), "application/json");
                    return;
                }

                if (!data.contains("password")) {
                    res.status = 400;
                    json response;
                    response["status"] = "error";
                    response["message"] = "Missing password field";
                    res.set_content(response.dump(), "application/json");
                    return;
                }

                std::string login = data.at("login");
                std::string email = data.at("email");
                std::string password = data.at("password");

                if (user_repo.existsByLogin(login) ||
                    user_repo.existsByEmail(email)) {
                    json error_response;
                    error_response["message"] = "User already exists";

                    res.status = 409;
                    res.set_content(error_response.dump(), "application/json");
                    return;
                }

                int user_id = user_repo.insert(login, email, password);

                json response;

                response["status"] = "success";
                response["message"] = "User registered successfully";
                response["data"] = {
                    {"id", user_id}, {"login", login}, {"email", email}};

                res.status = 200;
                res.set_content(response.dump(), "application/json");

            } catch (const std::exception& e) {
                json response;

                response["status"] = "error";
                response["message"] = "Server error: " + std::string(e.what());

                res.status = 400;
                res.set_content(response.dump(), "application/json");
            }
        });

    svr.Post("/auth", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            auto data = json::parse(req.body);
            if (!data.contains("info")) {
                res.status = 400;
                json response;
                response["status"] = "error";
                response["message"] = "Missing info field";
                res.set_content(response.dump(), "application/json");
                return;
            }

            if (!data.contains("password")) {
                res.status = 400;
                json response;
                response["status"] = "error";
                response["message"] = "Missing password field";
                res.set_content(response.dump(), "application/json");
                return;
            }

            std::string info = data.at("info");
            std::string password = data.at("password");
            std::optional<User> user;
            if (info.find('@') != std::string::npos) {
                user = user_repo.getByEmail(info);
            } else {
                user = user_repo.getByLogin(info);
            }

            if (user && user.value().password == password) {
                json response;
                response["status"] = "success";
                response["message"] = "Authentication successful";
                res.status = 200;
                res.set_content(response.dump(), "application/json");
            } else {
                json response;
                response["status"] = "error";
                response["message"] = "Authentication failed";
                res.status = 400;
                res.set_content(response.dump(), "application/json");
            }

        } catch (const std::exception& e) {
            res.status = 400;
            json response;
            response["status"] = "error";
            response["message"] = "Server error: " + std::string(e.what());
            res.set_content(response.dump(), "application/json");
        }
    });
}