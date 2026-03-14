// Modulestudent
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:   taowang    2026-03-13 19:41:09
// Description:
//
//     [v0.1.2]     2026-03-13 19:43:18
//         * 1
import std;
export module registrar:domain.student;

export class Student {
public:
    Student(const std::string& id, const std::string& name, const std::string& major)
        : _id(id), _name(name), _major(major) {}

    std::string getId() const { return _id; }
    std::string getName() const { return _name; }
    std::string getMajor() const { return _major; }
    void setName(const std::string& name) { _name = name; }
    void setMajor(const std::string& major) { _major = major; }

private:
    std::string _id;
    std::string _name;
    std::string _major;
};
