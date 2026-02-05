#!/bin/bash

echo "--- Начинаю обновление системы ---"
export DEBIAN_FRONTEND=noninteractive
apt-get update -y
apt-get upgrade -y
apt-get dist-upgrade -y
apt-get autoremove -y
apt-get autoclean -y

apt update
apt -y install wget
apt -y install gnupg

wget https://dev.mysql.com/get/mysql-apt-config_0.8.29-1_all.deb
dpkg -i mysql-apt-config_0.8.29-1_all.deb
gpg --keyserver keyserver.ubuntu.com --recv-keys B7B3B788A8D3785C
gpg --export B7B3B788A8D3785C | tee /usr/share/keyrings/mysql-archive-keyring.gpg > /dev/null

set -euo pipefail

FILE="/etc/apt/sources.list.d/mysql.list"

tee "$FILE" > /dev/null <<'EOF'
deb [signed-by=/usr/share/keyrings/mysql-archive-keyring.gpg] http://repo.mysql.com/apt/debian/ bookworm mysql-8.0
deb-src [signed-by=/usr/share/keyrings/mysql-archive-keyring.gpg] http://repo.mysql.com/apt/debian/ bookworm mysql-8.0
EOF

apt update
!/usr/bin/env bash
set -euo pipefail

read -rsp "Введите пароль для MySQL root: " MYSQL_ROOT_PASSWORD
echo

debconf-set-selections <<EOF
mysql-community-server mysql-community-server/root-pass password $MYSQL_ROOT_PASSWORD
mysql-community-server mysql-community-server/re-root-pass password $MYSQL_ROOT_PASSWORD
EOF

DEBIAN_FRONTEND=noninteractive apt -y install mysql-community-server

#apt -y install mysql-server

#apt install git -y
#git clone https://github.com/ISarrz/zelenka_server.git
#cd zelenka_server
#git checkout server


echo "--- Обновление завершено успешно! ---"