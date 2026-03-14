// Module course
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:   taowang    2026-03-13 20:01:09
// Description:
//
//     [v0.1.2]     2026-03-13 20:02:45
//         * 1
import std;
export module registrar:domain.course;

export class Course {
public:
    Course(const std::string& id, const std::string& name, int credits, const std::string& teacherId)
        : _id(id), _name(name), _credits(credits), _teacherId(teacherId) {}

    std::string getId() const { return _id; }
    std::string getName() const { return _name; }
    int getCredits() const { return _credits; }
    std::string getTeacherId() const { return _teacherId; }

    void setName(const std::string& name) { _name = name; }
    void setCredits(int credits) { _credits = credits; }
    void setTeacherId(const std::string& teacherId) { _teacherId = teacherId; }

private:
    std::string _id;
    std::string _name;
    int _credits;
    std::string _teacherId;
};
