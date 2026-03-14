// Module coursebroker
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:   taowang    2026-03-14 20:01:21
// Description:
//
//     [v0.1.2]     2026-03-14 20:12:23
//         * 1
import std;
import relationalbroker.cppm;
export module registrar:dm.coursebroker;
import :domain.course;
import :dm.base;

export class CourseBroker : public RelationalBroker {
public:
    static CourseBroker& instance();

    std::shared_ptr<Course> findById(const std::string& id);
    std::vector<std::shared_ptr<Course>> findAll();
    bool insert(const std::shared_ptr<Course>& course);
    bool update(const std::shared_ptr<Course>& course);
    bool remove(const std::string& id);

private:
    CourseBroker() = default;
    std::unordered_map<std::string, std::weak_ptr<Course>> _cache;

    std::shared_ptr<Course> loadFromDB(const std::string& id);
    void addToCache(std::shared_ptr<Course> course);
    void removeFromCache(const std::string& id);
};

// 实现
CourseBroker& CourseBroker::instance() {
    static CourseBroker instance;
    return instance;
}

std::shared_ptr<Course> CourseBroker::findById(const std::string& id) {
    auto it = _cache.find(id);
    if (it != _cache.end()) {
        if (auto sp = it->second.lock())
            return sp;
        else
            _cache.erase(it);
    }
    return loadFromDB(id);
}

std::shared_ptr<Course> CourseBroker::loadFromDB(const std::string& id) {
    try {
        pqxx::work txn(getConnection());
        pqxx::result r = txn.exec_params("SELECT id, name, credits, teacher_id FROM courses WHERE id = $1", id);
        txn.commit();
        if (r.empty()) return nullptr;
        const auto& row = r[0];
        auto course = std::make_shared<Course>(
            row[0].as<std::string>(),
            row[1].as<std::string>(),
            row[2].as<int>(),
            row[3].as<std::string>()
        );
        addToCache(course);
        return course;
    } catch (const std::exception& e) {
        std::cerr << "CourseBroker::loadFromDB 错误: " << e.what() << std::endl;
        return nullptr;
    }
}

std::vector<std::shared_ptr<Course>> CourseBroker::findAll() {
    std::vector<std::shared_ptr<Course>> result;
    try {
        pqxx::work txn(getConnection());
        pqxx::result r = txn.exec("SELECT id, name, credits, teacher_id FROM courses");
        txn.commit();
        for (const auto& row : r) {
            auto course = std::make_shared<Course>(
                row[0].as<std::string>(),
                row[1].as<std::string>(),
                row[2].as<int>(),
                row[3].as<std::string>()
            );
            addToCache(course);
            result.push_back(course);
        }
    } catch (const std::exception& e) {
        std::cerr << "CourseBroker::findAll 错误: " << e.what() << std::endl;
    }
    return result;
}

bool CourseBroker::insert(const std::shared_ptr<Course>& course) {
    try {
        pqxx::work txn(getConnection());
        txn.exec_params("INSERT INTO courses (id, name, credits, teacher_id) VALUES ($1, $2, $3, $4)",
                        course->getId(), course->getName(), course->getCredits(), course->getTeacherId());
        txn.commit();
        addToCache(course);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "CourseBroker::insert 错误: " << e.what() << std::endl;
        return false;
    }
}

bool CourseBroker::update(const std::shared_ptr<Course>& course) {
    try {
        pqxx::work txn(getConnection());
        txn.exec_params("UPDATE courses SET name = $2, credits = $3, teacher_id = $4 WHERE id = $1",
                        course->getId(), course->getName(), course->getCredits(), course->getTeacherId());
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "CourseBroker::update 错误: " << e.what() << std::endl;
        return false;
    }
}

bool CourseBroker::remove(const std::string& id) {
    try {
        pqxx::work txn(getConnection());
        txn.exec_params("DELETE FROM courses WHERE id = $1", id);
        txn.commit();
        removeFromCache(id);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "CourseBroker::remove 错误: " << e.what() << std::endl;
        return false;
    }
}

void CourseBroker::addToCache(std::shared_ptr<Course> course) {
    _cache[course->getId()] = course;
}

void CourseBroker::removeFromCache(const std::string& id) {
    _cache.erase(id);
}
