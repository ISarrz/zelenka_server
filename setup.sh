#!/bin/bash

echo "--- Начинаю обновление системы ---"

apt-get update -y
apt-get upgrade -y
apt-get dist-upgrade -y
apt-get autoremove -y
apt-get autoclean -y

echo "--- Обновление завершено успешно! ---"