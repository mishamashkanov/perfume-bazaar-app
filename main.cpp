#include <iostream>
#include "db.h"
#include "auth.h"
#include "deal.h"

void menuManager(Deal& d) {
    int c; std::string a, b;
    do {
        std::cout << "\n1.Sales 2.Buyers 3.TopType 4.TopBroker 5.Suppliers 6.Close 7.Deals 0.Exit\n>";
        std::cin >> c;
        switch (c) {
            case 1: std::cout << "From: "; std::cin >> a; std::cout << "To: "; std::cin >> b; d.salesReport(a, b); break;
            case 2: d.buyersReport(); 
                break;
            case 3: d.topTypeReport(); 
                break;
            case 4: d.topBrokerReport(); 
                break;
            case 5: d.suppliersReport(); 
                break;
            case 6: std::cout << "Date: ";
                std::cin >> a; 
                d.closeDay(a); 
                break;
            case 7: std::cout << "Date: "; 
                std::cin >> a; 
                d.showDeals(a); 
                break;
        }
    } while (c != 0);
}

void menuBroker(Deal& d, int kid) {
    int c, p, b, q;
    do {
        std::cout << "\n1.Deal 2.Today 0.Exit\n>";
        std::cin >> c;
        switch (c) {
            case 1: std::cout << "Product Buyer Qty: "; 
                std::cin >> p >> b >> q; 
                d.makeDeal(p, b, kid, q); 
                break;
            case 2: d.showDeals("date('now')"); 
                break;
        }
    } while (c != 0);
}

int main() {
    DB db("data/base.db");
    Auth auth(&db);
    Deal deal(&db);
    std::string login, pass;
    std::cout << "Login: "; std::cin >> login;
    std::cout << "Pass: "; std::cin >> pass;
    if (auth.login(login, pass)) {
        if (auth.getRole() == "manager") {
            menuManager(deal);
        }
        else {
            auto r = db.fetch("SELECT id FROM brokers WHERE user_id=" + std::to_string(auth.getUid()));
            if (!r.empty()) {
                menuBroker(deal, std::stoi(r[0]["id"]));
            }
        }
        auth.logout();
    }
}