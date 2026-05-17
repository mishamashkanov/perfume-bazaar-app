#include <iostream>
#include <cassert>
#include "db.h"
#include "auth.h"

void test_ok() {
    DB db(":memory:");
    db.run("CREATE TABLE users (id INTEGER PRIMARY KEY, login TEXT, password TEXT, role TEXT)");
    db.run("INSERT INTO users (login, password, role) VALUES ('admin', '123', 'manager')");
    Auth auth(&db);
    assert(auth.login("admin", "123") && auth.getRole() == "manager");
    std::cout << "[PASS] login ok\n";
}

void test_fail() {
    DB db(":memory:");
    db.run("CREATE TABLE users (id INTEGER PRIMARY KEY, login TEXT, password TEXT, role TEXT)");
    db.run("INSERT INTO users (login, password, role) VALUES ('admin', '123', 'manager')");
    Auth auth(&db);
    assert(!auth.login("admin", "wrong") && !auth.ok());
    std::cout << "[PASS] login fail\n";
}

void test_logout() {
    DB db(":memory:");
    db.run("CREATE TABLE users (id INTEGER PRIMARY KEY, login TEXT, password TEXT, role TEXT)");
    db.run("INSERT INTO users (login, password, role) VALUES ('admin', '123', 'manager')");
    Auth auth(&db);
    auth.login("admin", "123");
    auth.logout();
    assert(!auth.ok());
    std::cout << "[PASS] logout\n";
}

int main() {
    test_ok();
    test_fail();
    test_logout();
    std::cout << "All Auth tests passed\n";
    return 0;
}