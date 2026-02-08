#pragma once
#include "../Database.h"
#include "AbstractUserRepository.h"
#include "User.h"
#include <optional>

class UserRepository : public AbstractUserRepository {
  public:
    ~UserRepository() override = default;
    explicit UserRepository(Database *database);

    std::optional<User> getById(size_t id) override;
    std::optional<User> getByLogin(const std::string& login) override;
    std::optional<User> getByEmail(const std::string& email) override;

  private:
    static User Constructor(const sql::ResultSet* info);
    Database *database_;
};
