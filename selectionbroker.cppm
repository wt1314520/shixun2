// Module selectionbroker
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:   taowang    2026-03-14 22:42:09
// Description:
//
//     [v0.1.2]     2026-03-14 23:12:18
//         * 1
import relationalbroker.cppm;
import std;
export module registrar:dm.selectionbroker;
import :domain.selection;
import :dm.base;

export class SelectionBroker : public RelationalBroker {
public:
    static SelectionBroker& instance();

    std::shared_ptr<Selection> findById(const std::string& studentId, const std::string& courseId);
    std::vector<std::shared_ptr<Selection>> findByStudent(const std::string& studentId);
    std::vector<std::shared_ptr<Selection>> findByCourse(const std::string& courseId);
    bool insert(const std::string& studentId, const std::string& courseId);
    bool updateScore(const std::string& studentId, const std::string& courseId, int score);
    bool remove(const std::string& studentId, const std::string& courseId);

private:
    SelectionBroker() = default;
    // 缓存策略可以更简单，此处省略缓存（可自行添加）
    std::shared_ptr<Selection> loadFromDB(const std::string& studentId, const std::string& courseId);
};

// 实现
SelectionBroker& SelectionBroker::instance() {
    static SelectionBroker instance;
    return instance;
}

std::shared_ptr<Selection> SelectionBroker::findById(const std::string& studentId, const std::string& courseId) {
    return loadFromDB(studentId, courseId);
}

std::shared_ptr<Selection> SelectionBroker::loadFromDB(const std::string& studentId, const std::string& courseId) {
    try {
        pqxx::work txn(getConnection());
        pqxx::result r = txn.exec_params("SELECT student_id, course_id, score FROM selections WHERE student_id = $1 AND course_id = $2",
                                         studentId, courseId);
        txn.commit();
        if (r.empty()) return nullptr;
        const auto& row = r[0];
        std::optional<int> score;
        if (!row[2].is_null())
            score = row[2].as<int>();
        return std::make_shared<Selection>(
            row[0].as<std::string>(),
            row[1].as<std::string>(),
            score
        );
    } catch (const std::exception& e) {
        std::cerr << "SelectionBroker::loadFromDB 错误: " << e.what() << std::endl;
        return nullptr;
    }
}

std::vector<std::shared_ptr<Selection>> SelectionBroker::findByStudent(const std::string& studentId) {
    std::vector<std::shared_ptr<Selection>> result;
    try {
        pqxx::work txn(getConnection());
        pqxx::result r = txn.exec_params("SELECT student_id, course_id, score FROM selections WHERE student_id = $1", studentId);
        txn.commit();
        for (const auto& row : r) {
            std::optional<int> score;
            if (!row[2].is_null())
                score = row[2].as<int>();
            result.push_back(std::make_shared<Selection>(
                row[0].as<std::string>(),
                row[1].as<std::string>(),
                score
            ));
        }
    } catch (const std::exception& e) {
        std::cerr << "SelectionBroker::findByStudent 错误: " << e.what() << std::endl;
    }
    return result;
}

std::vector<std::shared_ptr<Selection>> SelectionBroker::findByCourse(const std::string& courseId) {
    std::vector<std::shared_ptr<Selection>> result;
    try {
        pqxx::work txn(getConnection());
        pqxx::result r = txn.exec_params("SELECT student_id, course_id, score FROM selections WHERE course_id = $1", courseId);
        txn.commit();
        for (const auto& row : r) {
            std::optional<int> score;
            if (!row[2].is_null())
                score = row[2].as<int>();
            result.push_back(std::make_shared<Selection>(
                row[0].as<std::string>(),
                row[1].as<std::string>(),
                score
            ));
        }
    } catch (const std::exception& e) {
        std::cerr << "SelectionBroker::findByCourse 错误: " << e.what() << std::endl;
    }
    return result;
}

bool SelectionBroker::insert(const std::string& studentId, const std::string& courseId) {
    try {
        pqxx::work txn(getConnection());
        txn.exec_params("INSERT INTO selections (student_id, course_id) VALUES ($1, $2)", studentId, courseId);
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "SelectionBroker::insert 错误: " << e.what() << std::endl;
        return false;
    }
}

bool SelectionBroker::updateScore(const std::string& studentId, const std::string& courseId, int score) {
    try {
        pqxx::work txn(getConnection());
        txn.exec_params("UPDATE selections SET score = $3 WHERE student_id = $1 AND course_id = $2",
                                studentId, courseId, score);
                txn.commit();
                return true;
            } catch (const std::exception& e) {
                std::cerr << "SelectionBroker::updateScore 错误: " << e.what() << std::endl;
                return false;
            }
        }

        bool SelectionBroker::remove(const std::string& studentId, const std::string& courseId) {
            try {
                pqxx::work txn(getConnection());
                txn.exec_params("DELETE FROM selections WHERE student_id = $1 AND course_id = $2", studentId, courseId);
                txn.commit();
                return true;
            } catch (const std::exception& e) {
                std::cerr << "SelectionBroker::remove 错误: " << e.what() << std::endl;
                return false;
            }
        }
