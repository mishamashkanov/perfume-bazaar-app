#ifndef AUTH_H
#define AUTH_H
#include "db.h"

class Auth {
    DB* db;
    int uid = 0;
    std::string role;
public:
    Auth(DB* d) : db(d) {}
    bool login(const std::string& login, const std::string& pass);
    void logout() { 
        uid = 0; role.clear(); 
    }
    bool ok() { 
        return uid > 0; 
    }
    std::string getRole() { 
        return role; 
    }
    int getUid() { 
        return uid; 
    }
};
#endif