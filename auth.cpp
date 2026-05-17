#include "auth.h"
#include <iostream>

bool Auth::login(const std::string& login, const std::string& pass) {
    auto rows = db->fetch(
        "SELECT id, role FROM users WHERE login='" + login + "' AND password='" + pass + "'"
    );
    if (!rows.empty()) {
        uid = std::stoi(rows[0]["id"]);
        role = rows[0]["role"];
        std::cout << "Welcome, " << login << "!\n";
        return true;
    }
    std::cout << "Wrong login or password!\n";
    return false;
}