#include "db.h"
#include <iostream>

DB::DB(const std::string& path) {
    if (sqlite3_open(path.c_str(), &conn) != SQLITE_OK) {
        std::cerr << "DB error: " << sqlite3_errmsg(conn) << std::endl;
        exit(1);
    }
}

DB::~DB() { 
    sqlite3_close(conn); 
}

bool DB::run(const std::string& sql) {
    char* err = nullptr;
    if (sqlite3_exec(conn, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
        std::cerr << "SQL error: " << err << std::endl;
        sqlite3_free(err);
        return false;
    }
    return true;
}

std::vector<std::map<std::string, std::string>> DB::fetch(const std::string& sql) {
    std::vector<std::map<std::string, std::string>> rows;
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int cols = sqlite3_column_count(stmt);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::map<std::string, std::string> row;
            for (int i = 0; i < cols; i++) {
                row[sqlite3_column_name(stmt, i)] = (const char*)sqlite3_column_text(stmt, i) ?: "NULL";
            }
            rows.push_back(row);
        }
        sqlite3_finalize(stmt);
    }
    return rows;
}