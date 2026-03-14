// Module
// File: relationalbroker.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何家欢      2026-03-14 20:41:14
// Description:
//
#include <pqxx/pqxx>
#include <memory>
#include <string>
#include <stdexcept>
export module registrar:dm.base;

export class RelationalBroker {
protected:
    static pqxx::connection& getConnection() {
        // 请修改为你的数据库连接信息
        static pqxx::connection conn("dbname=course_system user=wt password=123456 hostaddr=127.0.0.1 port=5432");
        if (!conn.is_open()) {
            throw std::runtime_error("无法连接到数据库");
        }
        return conn;
    }

public:
    virtual ~RelationalBroker() = default;
};
