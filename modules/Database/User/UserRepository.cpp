
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
    const std::unique_ptr<sql::PreparedStatement> pstmt(
            database_->getConnection()->prepareStatement(
                    "SELECT * FROM users WHERE email = ?"));

    pstmt->setString(1, email);
    const std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    if (res->next()) {
        return Constructor(res.get());
    }

    return std::nullopt;
}
std::optional<std::vector<User>> UserRepository::getAll() {
    const std::unique_ptr<sql::PreparedStatement> pstmt(
            database_->getConnection()->prepareStatement(
                    "SELECT * FROM users"));

    const std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    std::vector<User> users;
    while (res->next()) {
        users.push_back(Constructor(res.get()));
    }
    if (!users.empty()) {
        return users;
    }

    return std::nullopt;
}
void UserRepository::insert(std::string login, std::string email,
                            std::string password) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
                database_->getConnection()->prepareStatement(
                        "INSERT INTO users (login, email, password) VALUES (?, "
                        "?, ?)"));

        // 2. Привязываем значения к плейсхолдерам (индексация начинается с 1)
        pstmt->setString(1, login);
        pstmt->setString(2, email);
        pstmt->setString(3, password);

        // 3. Выполняем запрос
        pstmt->executeUpdate();

        std::cout << "Пользователь " << login << " успешно добавлен."
                  << std::endl;

    } catch (sql::SQLException &e) {
        std::cerr << "Ошибка при вставке пользователя: " << e.what()
                  << std::endl;
        // Здесь можно выбросить исключение дальше или вернуть bool статус
    }
}
void UserRepository::remove(User &user) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(
                database_->getConnection()->prepareStatement(
                        "DELETE FROM users WHERE id = ?"));

        pstmt->setInt(1, user.id);

        int rowsAffected = pstmt->executeUpdate();

        if (rowsAffected > 0) {
            std::cout << "Пользователь с id " << user.id << " удален."
                      << std::endl;
        } else {
            std::cout << "Пользователь с id " << user.id << " не найден."
                      << std::endl;
        }

    } catch (sql::SQLException &e) {
        std::cerr << "Ошибка при удалении: " << e.what() << std::endl;
    }
}
User UserRepository::Constructor(const sql::ResultSet *info) {
    User user;
    user.id = info->getInt("id");
    user.login = info->getString("login");
    user.email = info->getString("email");
    user.password = info->getString("password");
    return user;
}