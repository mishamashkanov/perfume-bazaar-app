#ifndef DB_H
#define DB_H
#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>

class DB {
    sqlite3* conn;
public:
    DB(const std::string& path);
    ~DB();
    bool run(const std::string& sql);
    std::vector<std::map<std::string, std::string>> fetch(const std::string& sql);
};
#endif