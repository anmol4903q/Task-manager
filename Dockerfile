FROM debian:bookworm-slim AS build
RUN apt-get update && apt-get install -y --no-install-recommends g++ ca-certificates && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY main.cpp httplib.h ./
RUN g++ -std=c++17 -O2 -pthread main.cpp -o task-manager

FROM debian:bookworm-slim
RUN apt-get update && apt-get install -y --no-install-recommends libstdc++6 ca-certificates && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY --from=build /app/task-manager ./task-manager
COPY public ./public
EXPOSE 10000
CMD ["./task-manager"]
