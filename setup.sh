#!/bin/bash

set -e

read -rsp "Введите ваш GitHub Token: " GITHUB_TOKEN
echo

apt-get update -y
apt-get upgrade -y
apt-get install -y wget gnupg git tmux curl

curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh

systemctl start docker
systemctl enable docker

git clone "https://${GITHUB_TOKEN}@github.com/ISarrz/zelenka_server.git"

echo "Установка завершена! Docker готов к работе."