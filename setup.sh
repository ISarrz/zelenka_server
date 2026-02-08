#!/bin/bash

# Прекращаем выполнение при ошибке
set -e

read -rsp "Введите пароль для MySQL root: " MYSQL_ROOT_PASSWORD
echo
read -rsp "Введите ваш GitHub Token: " GITHUB_TOKEN
echo

echo "--- 1. Обновление и установка базовых утилит ---"
export DEBIAN_FRONTEND=noninteractive

apt-get update -y
apt-get upgrade -y
# Устанавливаем всё необходимое для сборки C++ и работы сервера
apt-get install -y wget gnupg git tmux cmake build-essential libmysqlcppconn-dev awscli nlohmann-json3-dev

echo "--- 2. Настройка репозитория MySQL 8.0 ---"
# Скачиваем конфиг (версия 0.8.29 актуальна для Bookworm)
wget -q https://dev.mysql.com/get/mysql-apt-config_0.8.29-1_all.deb
dpkg -i mysql-apt-config_0.8.29-1_all.deb || apt-get install -f -y

# Добавляем ключи и репозиторий
gpg --keyserver keyserver.ubuntu.com --recv-keys B7B3B788A8D3785C
gpg --export B7B3B788A8D3785C | tee /usr/share/keyrings/mysql-archive-keyring.gpg > /dev/null

FILE="/etc/apt/sources.list.d/mysql.list"
echo "deb [signed-by=/usr/share/keyrings/mysql-archive-keyring.gpg] http://repo.mysql.com/apt/debian/ bookworm mysql-8.0" > "$FILE"

apt-get update

echo "--- 3. Установка MySQL Server ---"
# Автоматическая подстановка пароля для MySQL
debconf-set-selections <<EOF
mysql-community-server mysql-community-server/root-pass password $MYSQL_ROOT_PASSWORD
mysql-community-server mysql-community-server/re-root-pass password $MYSQL_ROOT_PASSWORD
mysql-server mysql-server/main_password password $MYSQL_ROOT_PASSWORD
mysql-server mysql-server/retype_password password $MYSQL_ROOT_PASSWORD
EOF

apt-get install -y mysql-community-server

echo "--- 4. Клонирование репозитория и сборка ---"
# Клонируем проект
git clone "https://${GITHUB_TOKEN}@github.com/ISarrz/zelenka_server.git"
cd zelenka_server
git checkout server

# Сразу подготавливаем проект к компиляции
mkdir -p build && cd build
cmake ..
make

echo "--- Готово! Проект скомпилирован в директории build ---"