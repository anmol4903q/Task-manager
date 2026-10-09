// Task Manager website - C++ backend (cpp-httplib) + HTML/JS frontend
#include "httplib.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Task { int id; string title; bool done; };

vector<Task> tasks;
int nextId = 1;
mutex mtx;
const string FILE_NAME = "tasks.txt";

// ---- storage: one line per task -> id|done|title ----
void save() {
    ofstream f(FILE_NAME);
    for (auto& t : tasks) f << t.id << "|" << t.done << "|" << t.title << "\n";
}

void load() {
    ifstream f(FILE_NAME);
    string line;
    while (getline(f, line)) {
        size_t a = line.find('|'), b = line.find('|', a + 1);
        if (a == string::npos || b == string::npos) continue;
        Task t{stoi(line.substr(0, a)), line.substr(b + 1), line[a + 1] == '1'};
        tasks.push_back(t);
        if (t.id >= nextId) nextId = t.id + 1;
    }
}

string esc(const string& s) {
    string o;
    for (char c : s) {
        if (c == '"' || c == '\\') { o += '\\'; o += c; }
        else if (c == '\n' || c == '\r') o += ' ';
        else o += c;
    }
    return o;
}

string toJson() {
    ostringstream o;
    o << "[";
    for (size_t i = 0; i < tasks.size(); i++) {
        if (i) o << ",";
        o << "{\"id\":" << tasks[i].id << ",\"title\":\"" << esc(tasks[i].title)
          << "\",\"done\":" << (tasks[i].done ? "true" : "false") << "}";
    }
    o << "]";
    return o.str();
}

int main() {
    load();
    httplib::Server svr;
    svr.set_mount_point("/", "./public");  // serves index.html

    svr.Get("/api/tasks", [](const httplib::Request&, httplib::Response& res) {
        lock_guard<mutex> g(mtx);
        res.set_content(toJson(), "application/json");
    });

    svr.Post("/api/tasks", [](const httplib::Request& req, httplib::Response& res) {
        string title = req.get_param_value("title");
        if (title.empty()) { res.status = 400; return; }
        title.erase(remove(title.begin(), title.end(), '|'), title.end());
        lock_guard<mutex> g(mtx);
        tasks.push_back({nextId++, title, false});
        save();
        res.set_content(toJson(), "application/json");
    });

    svr.Post(R"(/api/tasks/(\d+)/toggle)", [](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);
        lock_guard<mutex> g(mtx);
        for (auto& t : tasks) if (t.id == id) t.done = !t.done;
        save();
        res.set_content(toJson(), "application/json");
    });

    svr.Delete(R"(/api/tasks/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);
        lock_guard<mutex> g(mtx);
        for (size_t i = 0; i < tasks.size(); i++)
            if (tasks[i].id == id) { tasks.erase(tasks.begin() + i); break; }
        save();
        res.set_content(toJson(), "application/json");
    });

    const char* portEnv = std::getenv("PORT");
    int port = portEnv ? std::atoi(portEnv) : 8080;
    if (port <= 0 || port > 65535) port = 8080;
    cout << "Running on port " << port << "\n";
    if (!svr.listen("0.0.0.0", port)) {
        cerr << "Failed to start server on port " << port << "\n";
        return 1;
    }
    return 0;
}
