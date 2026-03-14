// Module selection
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:   taowang    2026-03-13 20:08:09
// Description:
//
//     [v0.1.2]     2026-03-13 20:08:45
//         * 1
import std;
export module registrar:domain.selection;

export class Selection {
public:
    Selection(const std::string& studentId, const std::string& courseId, std::optional<int> score = std::nullopt)
        : _studentId(studentId), _courseId(courseId), _score(score) {}

    std::string getStudentId() const { return _studentId; }
    std::string getCourseId() const { return _courseId; }
    std::optional<int> getScore() const { return _score; }
    void setScore(int score) { _score = score; }

private:
    std::string _studentId;
    std::string _courseId;
    std::optional<int> _score;
};
