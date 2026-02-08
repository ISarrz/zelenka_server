
#include "UserRepository.h"
UserRepository::UserRepository(Database *database) { database_ = database; }

std::optional<User> UserRepository::getById(const size_t user_id) {
    sql::Connection *con = database_->getConnection();

    const std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement("SELECT * FROM users WHERE id = ?"));

    pstmt->setInt(1, user_id);
    const std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    if (res->next()) {
        return Constructor(res.get());
    }

    return std::nullopt;
}

std::optional<User> UserRepository::getByLogin(const std::string &login) {
    sql::Connection *con = database_->getConnection();

    const std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement("SELECT * FROM users WHERE login = ?"));

    pstmt->setString(1, login);
    const std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    if (res->next()) {
        return Constructor(res.get());
    }

    return std::nullopt;
}

std::optional<User> UserRepository::getByEmail(const std::string &email) {
    sql::Connection *con = database_->getConnection();

    const std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement("SELECT * FROM users WHERE email = ?"));

    pstmt->setString(1, email);
    const std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    if (res->next()) {
        return Constructor(res.get());
    }

    return std::nullopt;
}

User UserRepository::Constructor(const sql::ResultSet *info) {
    User user;
    user.id = info->getInt("id");
    user.login = info->getString("login");
    user.email = info->getString("email");
    user.password = info->getString("password");
    return user;
}