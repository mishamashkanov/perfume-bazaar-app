#include <iostream>
#include <cassert>
#include "db.h"
#include "deal.h"

void setup(DB& db) {
    db.run("CREATE TABLE users (id INTEGER PRIMARY KEY, login TEXT, password TEXT, role TEXT)");
    db.run("CREATE TABLE brokers (id INTEGER PRIMARY KEY, user_id INTEGER, name TEXT)");
    db.run("CREATE TABLE suppliers (id INTEGER PRIMARY KEY, name TEXT)");
    db.run("CREATE TABLE buyers (id INTEGER PRIMARY KEY, name TEXT)");
    db.run("CREATE TABLE products (id INTEGER PRIMARY KEY, name TEXT, type TEXT, price REAL, sup_id INTEGER, qty INTEGER)");
    db.run("CREATE TABLE deals (id INTEGER PRIMARY KEY, date TEXT, prod_id INTEGER, buyer_id INTEGER, brok_id INTEGER, qty INTEGER, sum REAL)");
    db.run("CREATE TABLE stats (id INTEGER PRIMARY KEY, brok_id INTEGER, deals INTEGER DEFAULT 0, sold INTEGER DEFAULT 0, revenue REAL DEFAULT 0)");
    db.run("INSERT INTO brokers (id, user_id, name) VALUES (1, 0, 'Test')");
    db.run("INSERT INTO buyers (id, name) VALUES (1, 'Buyer')");
    db.run("INSERT INTO products (id, name, type, price, sup_id, qty) VALUES (1, 'Test', 'Spirit', 1000, 1, 50)");
    db.run("INSERT INTO stats (brok_id) VALUES (1)");
}

void test_make() {
    DB db(":memory:");
    setup(db);
    Deal d(&db);
    assert(d.makeDeal(1, 1, 1, 5));
    std::cout << "[PASS] make deal\n";
}

void test_nostock() {
    DB db(":memory:");
    setup(db);
    Deal d(&db);
    assert(!d.makeDeal(1, 1, 1, 100));
    std::cout << "[PASS] no stock\n";
}

void test_stats() {
    DB db(":memory:");
    setup(db);
    Deal d(&db);
    d.makeDeal(1, 1, 1, 5);
    auto rows = db.fetch("SELECT deals, sold, revenue FROM stats WHERE brok_id=1");
    assert(std::stoi(rows[0]["deals"]) == 1);
    assert(std::stoi(rows[0]["sold"]) == 5);
    assert(std::stod(rows[0]["revenue"]) == 5000.0);
    std::cout << "[PASS] stats updated\n";
}

int main() {
    test_make();
    test_nostock();
    test_stats();
    std::cout << "All Deal tests passed\n";
    return 0;
}