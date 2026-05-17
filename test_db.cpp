#include <iostream>
#include <cassert>
#include "db.h"

void test_open() {
    DB db(":memory:");
    std::cout << "[PASS] open\n";
}

void test_table() {
    DB db(":memory:");
    assert(db.run("CREATE TABLE t (id INTEGER PRIMARY KEY, name TEXT)"));
    std::cout << "[PASS] create table\n";
}

void test_insert() {
    DB db(":memory:");
    db.run("CREATE TABLE t (id INTEGER PRIMARY KEY, name TEXT)");
    db.run("INSERT INTO t (name) VALUES ('test')");
    auto rows = db.fetch("SELECT name FROM t WHERE id=1");
    assert(rows.size() == 1 && rows[0]["name"] == "test");
    std::cout << "[PASS] insert select\n";
}

int main() {
    test_open();
    test_table();
    test_insert();
    std::cout << "All DB tests passed\n";
    return 0;
}