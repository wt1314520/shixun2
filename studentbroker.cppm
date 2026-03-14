// Module
// File: studentbroker.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何家欢      2026-03-14 20:45:17
// Description:
//
module;
#include "relationalbroker.cppm"  // 注意：实际模块导入应使用 import，但为简化此处用 #include
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
export module registrar:dm.studentbroker;
import :domain.student;
import :dm.base;

export class StudentBroker : public RelationalBroker {
public:
    static StudentBroker& instance();

    std::shared_ptr<Student> findById(const std::string& id);
    std::vector<std::shared_ptr<Student>> findAll();
    bool insert(const std::shared_ptr<Student>& student);
    bool update(const std::shared_ptr<Student>& student);
    bool remove(const std::string& id);

private:
    StudentBroker() = default;
    std::unordered_map<std::string, std::weak_ptr<Student>> _cache;

    std::shared_ptr<Student> loadFromDB(const std::string& id);
    void addToCache(std::shared_ptr<Student> student);
    void removeFromCache(const std::string& id);
};

// 实现
StudentBroker& StudentBroker::instance() {
    static StudentBroker instance;
    return instance;
}

std::shared_ptr<Student> StudentBroker::findById(const std::string& id) {
    auto it = _cache.find(id);
    if (it != _cache.end()) {
        if (auto sp = it->second.lock())
            return sp;
        else
            _cache.erase(it);
    }
    return loadFromDB(id);
}

std::shared_ptr<Student> StudentBroker::loadFromDB(const std::string& id) {
    try {
        pqxx::work txn(getConnection());
        pqxx::result r = txn.exec_params("SELECT id, name, major FROM students WHERE id = $1", id);
        txn.commit();
        if (r.empty()) return nullptr;
        const auto& row = r[0];
        auto student = std::make_shared<Student>(
            row[0].as<std::string>(),
            row[1].as<std::string>(),
            row[2].as<std::string>()
        );
        addToCache(student);
        return student;
    } catch (const std::exception& e) {
        std::cerr << "StudentBroker::loadFromDB 错误: " << e.what() << std::endl;
        return nullptr;
    }
}

std::vector<std::shared_ptr<Student>> StudentBroker::findAll() {
    std::vector<std::shared_ptr<Student>> result;
    try {
        pqxx::work txn(getConnection());
        pqxx::result r = txn.exec("SELECT id, name, major FROM students");
        txn.commit();
        for (const auto& row : r) {
            auto student = std::make_shared<Student>(
                row[0].as<std::string>(),
                row[1].as<std::string>(),
                row[2].as<std::string>()
            );
            addToCache(student);
            result.push_back(student);
        }
    } catch (const std::exception& e) {
        std::cerr << "StudentBroker::findAll 错误: " << e.what() << std::endl;
    }
    return result;
}

bool StudentBroker::insert(const std::shared_ptr<Student>& student) {
    try {
        pqxx::work txn(getConnection());
        txn.exec_params("INSERT INTO students (id, name, major) VALUES ($1, $2, $3)",
                        student->getId(), student->getName(), student->getMajor());
        txn.commit();
        addToCache(student);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "StudentBroker::insert 错误: " << e.what() << std::endl;
        return false;
    }
}

bool StudentBroker::update(const std::shared_ptr<Student>& student) {
    try {
        pqxx::work txn(getConnection());
        txn.exec_params("UPDATE students SET name = $2, major = $3 WHERE id = $1",
                        student->getId(), student->getName(), student->getMajor());
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "StudentBroker::update 错误: " << e.what() << std::endl;
        return false;
    }
}

bool StudentBroker::remove(const std::string& id) {
    try {
        pqxx::work txn(getConnection());
        txn.exec_params("DELETE FROM students WHERE id = $1", id);
        txn.commit();
        removeFromCache(id);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "StudentBroker::remove 错误: " << e.what() << std::endl;
        return false;
    }
}

void StudentBroker::addToCache(std::shared_ptr<Student> student) {
    _cache[student->getId()] = student;
}

void StudentBroker::removeFromCache(const std::string& id) {
    _cache.erase(id);
}
