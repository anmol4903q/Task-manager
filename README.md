# C++ Task Manager Website

Build:  g++ -std=c++17 main.cpp -o server -pthread   (Windows/MinGW: add -lws2_32)
Run:    ./server   ->  open http://localhost:8080

Split:
- Person A (backend): main.cpp  - routes, storage
- Person B (frontend): public/  - index.html, style.css, app.js

API: GET /api/tasks | POST /api/tasks (title) | POST /api/tasks/:id/toggle | DELETE /api/tasks/:id
