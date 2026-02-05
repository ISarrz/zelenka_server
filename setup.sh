#!/bin/bash
set -euo pipefail

export DEBIAN_FRONTEND=noninteractive

# ===== Ввод переменных в самом начале =====
read -rsp "Введите пароль для MySQL root: " MYSQL_ROOT_PASSWORD
echo

read -rsp "Введите GitHub token: " GITHUB_TOKEN
echo


echo "--- Начинаю обновление системы ---"

apt-get update -y
apt-get upgrade -y
apt-get dist-upgrade -y
apt-get autoremove -y
apt-get autoclean -y

apt -y install wget gnupg debconf-utils git

# ===== Скачиваем mysql-apt-config =====
wget -q https://dev.mysql.com/get/mysql-apt-config_0.8.29-1_all.deb -O /tmp/mysql-apt-config.deb

# ===== Автовыбор MySQL 8.0 (важно: ДО dpkg -i) =====
debconf-set-selections <<EOF
mysql-apt-config mysql-apt-config/select-server select mysql-8.0
mysql-apt-config mysql-apt-config/select-product select Ok
EOF

# ===== Устанавливаем без меню =====
dpkg -i /tmp/mysql-apt-config.deb

# ===== Ключ =====
gpg --keyserver keyserver.ubuntu.com --recv-keys B7B3B788A8D3785C
gpg --export B7B3B788A8D3785C | tee /usr/share/keyrings/mysql-archive-keyring.gpg > /dev/null

# ===== Перезаписываем репозиторий =====
FILE="/etc/apt/sources.list.d/mysql.list"

tee "$FILE" > /dev/null <<'EOF'
deb [signed-by=/usr/share/keyrings/mysql-archive-keyring.gpg] http://repo.mysql.com/apt/debian/ bookworm mysql-8.0
deb-src [signed-by=/usr/share/keyrings/mysql-archive-keyring.gpg] http://repo.mysql.com/apt/debian/ bookworm mysql-8.0
EOF

apt update

# ===== Пароль для mysql-community-server =====
debconf-set-selections <<EOF
mysql-community-server mysql-community-server/root-pass password $MYSQL_ROOT_PASSWORD
mysql-community-server mysql-community-server/re-root-pass password $MYSQL_ROOT_PASSWORD
EOF

# ===== Установка =====
DEBIAN_FRONTEND=noninteractive apt -y install mysql-community-server

# ===== Убираем временный файл =====
rm -f /tmp/mysql-apt-config.deb

# ===== Клонирование репозитория с токеном =====
git clone "https://${GITHUB_TOKEN}@github.com/ISarrz/zelenka_server.git"
cd zelenka_server
git checkout server

echo "--- Установка завершена успешно! ---"

# ===== Самоудаление скрипта =====
SCRIPT_PATH="$(readlink -f "$0")"
rm -f -- "$SCRIPT_PATH"
