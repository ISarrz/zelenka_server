#!/bin/bash

echo "--- Начинаю обновление системы ---"
export DEBIAN_FRONTEND=noninteractive
apt-get update -y
apt-get upgrade -y
apt-get dist-upgrade -y
apt-get autoremove -y
apt-get autoclean -y

apt install git -y
git clone https://github.com/ISarrz/zelenka_server.git
cd zelenka_server
git checkout server

apt install gnupg -y
apt install -y gnupg debconf-utils wget
wget https://dev.mysql.com/get/mysql-apt-config_0.8.29-1_all.deb
echo "mysql-apt-config mysql-apt-config/select-server select mysql-8.0" | debconf-set-selections

dpkg -i mysql-apt-config_0.8.29-1_all.deb
apt update
apt install -y mysql-server

echo "--- Обновление завершено успешно! ---"