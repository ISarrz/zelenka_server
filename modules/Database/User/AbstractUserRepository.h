#pragma once
#include "User.h"
#include <optional>

class AbstractUserRepository {
  public:
    virtual ~AbstractUserRepository() = default;

    virtual std::optional<User> getById(size_t id) = 0;
    virtual std::optional<User> getByLogin(const std::string& login) = 0;
    virtual std::optional<User> getByEmail(const std::string& email) = 0;

    private:

};
