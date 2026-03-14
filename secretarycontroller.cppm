// Module
// File: secretarycontroller.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何家欢      2026-03-14 20:42:01
// Description:
//
#include <iostream>
#include <string>
export module registrar:applogic.secretarycontroller;
import :domain.student;
import :domain.teacher;
import :domain.course;
import :dm.studentbroker;
import :dm.teacherbroker;
import :dm.coursebroker;
import :dm.selectionbroker;

export class SecretaryController {
public:
    void showMenu();
    void addStudent();
    void deleteStudent();
    void addTeacher();
    void deleteTeacher();
    void addCourse();
    void deleteCourse();
};

void SecretaryController::showMenu() {
    int choice;
    do {
        std::cout << "\n===== 教学秘书菜单 =====\n";
        std::cout << "1. 添加学生\n";
        std::cout << "2. 删除学生\n";
        std::cout << "3. 添加教师\n";
        std::cout << "4. 删除教师\n";
        std::cout << "5. 添加课程\n";
        std::cout << "6. 删除课程\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        std::cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: deleteStudent(); break;
            case 3: addTeacher(); break;
            case 4: deleteTeacher(); break;
            case 5: addCourse(); break;
            case 6: deleteCourse(); break;
            case 0: std::cout << "返回上级菜单\n"; break;
            default: std::cout << "无效选择!\n";
        }
    } while (choice != 0);
}

void SecretaryController::addStudent() {
    std::string id, name, major;
    std::cout << "请输入学号: "; std::cin >> id;
    std::cout << "请输入姓名: "; std::cin >> name;
    std::cout << "请输入专业: "; std::cin >> major;

    auto student = std::make_shared<Student>(id, name, major);
    if (StudentBroker::instance().insert(student)) {
        std::cout << "学生添加成功。\n";
    } else {
        std::cout << "添加失败，学号可能已存在。\n";
    }
}

void SecretaryController::deleteStudent() {
    std::string id;
    std::cout << "请输入要删除的学号: "; std::cin >> id;
    if (StudentBroker::instance().remove(id)) {
        std::cout << "学生删除成功。\n";
    } else {
        std::cout << "删除失败，学号不存在。\n";
    }
}

void SecretaryController::addTeacher() {
    std::string id, name, title;
    std::cout << "请输入教师号: "; std::cin >> id;
    std::cout << "请输入姓名: "; std::cin >> name;
    std::cout << "请输入职称: "; std::cin >> title;

    auto teacher = std::make_shared<Teacher>(id, name, title);
    if (TeacherBroker::instance().insert(teacher)) {
        std::cout << "教师添加成功。\n";
    } else {
        std::cout << "添加失败，教师号可能已存在。\n";
    }
}

void SecretaryController::deleteTeacher() {
    std::string id;
    std::cout << "请输入要删除的教师号: "; std::cin >> id;
    if (TeacherBroker::instance().remove(id)) {
        std::cout << "教师删除成功。\n";
    } else {
        std::cout << "删除失败，教师号不存在。\n";
    }
}

void SecretaryController::addCourse() {
    std::string id, name, teacherId;
    int credits;
    std::cout << "请输入课程号: "; std::cin >> id;
    std::cout << "请输入课程名: "; std::cin >> name;
    std::cout << "请输入学分: "; std::cin >> credits;
    std::cout << "请输入教师号: "; std::cin >> teacherId;

    auto teacher = TeacherBroker::instance().findById(teacherId);
    if (!teacher) {
        std::cout << "教师号不存在，无法添加课程。\n";
        return;
    }

    auto course = std::make_shared<Course>(id, name, credits, teacherId);
    if (CourseBroker::instance().insert(course)) {
        std::cout << "课程添加成功。\n";
    } else {
        std::cout << "添加失败，课程号可能已存在。\n";
    }
}

void SecretaryController::deleteCourse() {
    std::string id;
    std::cout << "请输入要删除的课程号: "; std::cin >> id;
    // 检查是否有选课记录，若有则提示
    auto selections = SelectionBroker::instance().findByCourse(id);
    if (!selections.empty()) {
        std::cout << "该课程已有学生选课，删除将同时删除所有选课记录，确定吗？(y/n): ";
        char ch;
        std::cin >> ch;
        if (ch != 'y' && ch != 'Y')
            return;
    }
    if (CourseBroker::instance().remove(id)) {
        std::cout << "课程删除成功。\n";
    } else {
        std::cout << "删除失败，课程号不存在。\n";
    }
}
