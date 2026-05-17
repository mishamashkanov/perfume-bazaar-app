#ifndef DEAL_H
#define DEAL_H
#include "db.h"

class Deal {
    DB* db;
public:
    Deal(DB* d) : db(d) {}
    bool makeDeal(int pid, int bid, int kid, int qty);
    void closeDay(const std::string& date);
    void showDeals(const std::string& date);
    void salesReport(const std::string& from, const std::string& to);
    void buyersReport();
    void topTypeReport();
    void topBrokerReport();
    void suppliersReport();
};
#endif