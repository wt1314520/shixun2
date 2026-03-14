// Module teacherbroker
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:   taowang    2026-03-14 19:43:09
// Description:
//
//     [v0.1.2]     2026-03-14 19:43:18
//         * 1
import relationalbroker.cppm;
import std;
export module registrar:dm.teacherbroker;
import :domain.teacher;
import :dm.base;

export class TeacherBroker : public RelationalBroker {
public:
    static TeacherBroker& instance();

    std::shared_ptr<Teacher> findById(const std::string& id);
    std::vector<std::shared_ptr<Teacher>> findAll();
    bool insert(const std::shared_ptr<Teacher>& teacher);
    bool update(const std::shared_ptr<Teacher>& teacher);
    bool remove(const std::string& id);

private:
    TeacherBroker() = default;
    std::unordered_map<std::string, std::weak_ptr<Teacher>> _cache;

    std::shared_ptr<Teacher> loadFromDB(const std::string& id);
    void addToCache(std::shared_ptr<Teacher> teacher);
    void removeFromCache(const std::string& id);
};

// 实现
TeacherBroker& TeacherBroker::instance() {
    static TeacherBroker instance;
    return instance;
}

std::shared_ptr<Teacher> TeacherBroker::findById(const std::string& id) {
    auto it = _cache.find(id);
    if (it != _cache.end()) {
        if (auto sp = it->second.lock())
            return sp;
        else
            _cache.erase(it);
    }
    return loadFromDB(id);
}

std::shared_ptr<Teacher> TeacherBroker::loadFromDB(const std::string& id) {
    try {
        pqxx::work txn(getConnection());
        pqxx::result r = txn.exec_params("SELECT id, name, title FROM teachers WHERE id = $1", id);
        txn.commit();
        if (r.empty()) return nullptr;
        const auto& row = r[0];
        auto teacher = std::make_shared<Teacher>(
            row[0].as<std::string>(),
            row[1].as<std::string>(),
            row[2].as<std::string>()
        );
        addToCache(teacher);
        return teacher;
    } catch (const std::exception& e) {
        std::cerr << "TeacherBroker::loadFromDB 错误: " << e.what() << std::endl;
        return nullptr;
    }
}

std::vector<std::shared_ptr<Teacher>> TeacherBroker::findAll() {
    std::vector<std::shared_ptr<Teacher>> result;
    try {
        pqxx::work txn(getConnection());
        pqxx::result r = txn.exec("SELECT id, name, title FROM teachers");
        txn.commit();
        for (const auto& row : r) {
            auto teacher = std::make_shared<Teacher>(
                row[0].as<std::string>(),
                row[1].as<std::string>(),
                row[2].as<std::string>()
            );
            addToCache(teacher);
            result.push_back(teacher);
        }
    } catch (const std::exception& e) {
        std::cerr << "TeacherBroker::findAll 错误: " << e.what() << std::endl;
    }
    return result;
}

bool TeacherBroker::insert(const std::shared_ptr<Teacher>& teacher) {
    try {
        pqxx::work txn(getConnection());
        txn.exec_params("INSERT INTO teachers (id, name, title) VALUES ($1, $2, $3)",
                        teacher->getId(), teacher->getName(), teacher->getTitle());
        txn.commit();
        addToCache(teacher);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "TeacherBroker::insert 错误: " << e.what() << std::endl;
        return false;
    }
}

bool TeacherBroker::update(const std::shared_ptr<Teacher>& teacher) {
    try {
        pqxx::work txn(getConnection());
        txn.exec_params("UPDATE teachers SET name = $2, title = $3 WHERE id = $1",
                        teacher->getId(), teacher->getName(), teacher->getTitle());
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "TeacherBroker::update 错误: " << e.what() << std::endl;
        return false;
    }
}

bool TeacherBroker::remove(const std::string& id) {
    try {
        pqxx::work txn(getConnection());
        txn.exec_params("DELETE FROM teachers WHERE id = $1", id);
        txn.commit();
        removeFromCache(id);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "TeacherBroker::remove 错误: " << e.what() << std::endl;
        return false;
    }
}

void TeacherBroker::addToCache(std::shared_ptr<Teacher> teacher) {
    _cache[teacher->getId()] = teacher;
}

void TeacherBroker::removeFromCache(const std::string& id) {
    _cache.erase(id);
}
