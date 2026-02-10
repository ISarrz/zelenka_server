#!/bin/bash

set -e

read -rsp "Введите ваш GitHub Token: " GITHUB_TOKEN
echo

apt-get update -y
apt-get upgrade -y

apt-get install -y wget gnupg git tmux docker


git clone "https://${GITHUB_TOKEN}@github.com/ISarrz/zelenka_server.git"

echo "Репозиторий успешно клонирован!"