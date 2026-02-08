# 1. Используем Debian как самую стабильную среду для этих библиотек
FROM debian:bookworm

# 2. Устанавливаем все нужные тебе библиотеки
# nlohmann-json3-dev и libmysqlcppconn-dev — это именно те пакеты,
# которые "закроют" твои ошибки fatal error.
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libmysqlcppconn-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

# 3. Указываем рабочую директорию
WORKDIR /app

# 4. Копируем проект
COPY . .

# 5. Собираем проект так же, как ты делал это вручную
RUN mkdir -p build && cd build && \
    cmake .. && \
    make

# 6. Запуск сервера
CMD ["./build/zelenka_server"]