#!/bin/bash

echo "--- Начинаю обновление системы ---"

apt-get update -y
apt-get upgrade -y
apt-get dist-upgrade -y
apt-get autoremove -y
apt-get autoclean -y

apt install git -y
git clone https://github.com/ISarrz/zelenka_server.git
git checkout server

echo "--- Обновление завершено успешно! ---"