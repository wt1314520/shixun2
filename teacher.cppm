// Module teacher
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created:   taowang    2026-03-13 19:55:09
// Description:
//
//     [v0.1.2]     2026-03-13 19:58:18
//         * 1
import std;
export module registrar:domain.teacher;

export class Teacher {
public:
    Teacher(const std::string& id, const std::string& name, const std::string& title)
        : _id(id), _name(name), _title(title) {}

    std::string getId() const { return _id; }
    std::string getName() const { return _name; }
    std::string getTitle() const { return _title; }
    void setName(const std::string& name) { _name = name; }
    void setTitle(const std::string& title) { _title = title; }

private:
    std::string _id;
    std::string _name;
    std::string _title;
};
