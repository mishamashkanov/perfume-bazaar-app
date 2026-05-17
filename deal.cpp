#include "deal.h"
#include <iostream>

bool Deal::makeDeal(int pid, int bid, int kid, int qty) {
    auto p = db->fetch("SELECT qty, price FROM products WHERE id=" + std::to_string(pid));
    if (p.empty() || std::stoi(p[0]["qty"]) < qty) {
        std::cout << "Not enough stock!\n";
        return false;
    }
    double sum = std::stod(p[0]["price"]) * qty;
    db->run(
        "INSERT INTO deals (date, prod_id, buyer_id, brok_id, qty, sum) "
        "VALUES (date('now'), " + std::to_string(pid) + ", " +
        std::to_string(bid) + ", " + std::to_string(kid) + ", " +
        std::to_string(qty) + ", " + std::to_string(sum) + ")"
    );
    db->run("UPDATE products SET qty = qty - " + std::to_string(qty) + " WHERE id=" + std::to_string(pid));
    db->run(
        "UPDATE stats SET deals=deals+1, sold=sold+" + std::to_string(qty) +
        ", revenue=revenue+" + std::to_string(sum) + " WHERE brok_id=" + std::to_string(kid)
    );
    std::cout << "Deal done! Sum: " << sum << "\n";
    return true;
}

void Deal::closeDay(const std::string& date) {
    db->run(
        "UPDATE products SET qty = qty - "
        "(SELECT COALESCE(SUM(qty),0) FROM deals WHERE prod_id=products.id AND date < '" + date + "')"
    );
    db->run("DELETE FROM deals WHERE date < '" + date + "'");
    std::cout << "Day closed. Deals before " << date << " removed.\n";
}

void Deal::showDeals(const std::string& date) {
    auto rows = db->fetch(
        "SELECT d.id, p.name, d.qty, d.sum, b.name AS broker, u.name AS buyer "
        "FROM deals d "
        "JOIN products p ON d.prod_id=p.id "
        "JOIN brokers b ON d.brok_id=b.id "
        "JOIN buyers u ON d.buyer_id=u.id "
        "WHERE d.date='" + date + "'"
    );
    for (auto& r : rows)
        std::cout << r["id"] << " " << r["name"] << " " << r["qty"] << " " << r["sum"] << " " << r["broker"] << " " << r["buyer"] << "\n";
}

void Deal::salesReport(const std::string& from, const std::string& to) {
    auto rows = db->fetch(
        "SELECT p.name, SUM(d.qty) AS q, SUM(d.sum) AS s "
        "FROM deals d JOIN products p ON d.prod_id=p.id "
        "WHERE d.date BETWEEN '" + from + "' AND '" + to + "' GROUP BY p.name"
    );
    for (auto& r : rows) {
        std::cout << r["name"] << " " << r["q"] << " " << r["s"] << "\n";
    }
}

void Deal::buyersReport() {
    auto rows = db->fetch(
        "SELECT p.name, u.name AS buyer, SUM(d.qty) AS q, SUM(d.sum) AS s "
        "FROM deals d JOIN products p ON d.prod_id=p.id JOIN buyers u ON d.buyer_id=u.id "
        "GROUP BY p.name, u.name ORDER BY p.name"
    );
    for (auto& r : rows) {
        std::cout << r["name"] << " -> " << r["buyer"] << " " << r["q"] << " " << r["s"] << "\n";
    }
}

void Deal::topTypeReport() {
    auto rows = db->fetch(
        "SELECT p.type, u.name AS buyer, SUM(d.qty) AS q, SUM(d.sum) AS s "
        "FROM deals d JOIN products p ON d.prod_id=p.id JOIN buyers u ON d.buyer_id=u.id "
        "WHERE p.type=(SELECT type FROM (SELECT p2.type, SUM(d2.qty) AS t FROM deals d2 JOIN products p2 ON d2.prod_id=p2.id GROUP BY p2.type ORDER BY t DESC LIMIT 1)) "
        "GROUP BY p.type, u.name"
    );
    for (auto& r : rows) {
        std::cout << r["type"] << " -> " << r["buyer"] << " " << r["q"] << " " << r["s"] << "\n";
    }
}

void Deal::topBrokerReport() {
    auto rows = db->fetch(
        "SELECT b.name, b.address, b.year, s.deals, GROUP_CONCAT(DISTINCT sp.name) AS sups "
        "FROM brokers b JOIN stats s ON b.id=s.brok_id "
        "JOIN deals d ON b.id=d.brok_id JOIN products p ON d.prod_id=p.id JOIN suppliers sp ON p.sup_id=sp.id "
        "WHERE s.deals=(SELECT MAX(deals) FROM stats) GROUP BY b.id"
    );
    for (auto& r : rows) {
        std::cout << r["name"] << " " << r["address"] << " " << r["year"] << " " << r["deals"] << " deals " << r["sups"] << "\n";
    }
}

void Deal::suppliersReport() {
    auto rows = db->fetch(
        "SELECT sp.name AS sup, b.name AS brok, SUM(d.qty) AS q, SUM(d.sum) AS s "
        "FROM deals d JOIN products p ON d.prod_id=p.id JOIN suppliers sp ON p.sup_id=sp.id JOIN brokers b ON d.brok_id=b.id "
        "GROUP BY sp.name, b.name ORDER BY sp.name"
    );
    for (auto& r : rows) {
        std::cout << r["sup"] << " -> " << r["brok"] << " " << r["q"] << " " << r["s"] << "\n";
    }
}