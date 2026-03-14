// Module
// File: studentcontroller.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何家欢      2026-03-14 20:39:25
// Description:
//
module;
#include <iostream>
#include <iomanip>
#include <string>
#include <optional>
export module registrar:applogic.studentcontroller;
import :domain.student;
import :domain.course;
import :domain.selection;
import :dm.studentbroker;
import :dm.coursebroker;
import :dm.selectionbroker;

export class StudentController {
public:
    void showMenu(const std::string& studentId);
    void listAllCourses();
    void enrollCourse(const std::string& studentId);
    void dropCourse(const std::string& studentId);
    void viewScores(const std::string& studentId);
};

void StudentController::showMenu(const std::string& studentId) {
    int choice;
    do {
        std::cout << "\n===== 学生菜单 =====\n";
        std::cout << "1. 浏览所有课程\n";
        std::cout << "2. 选课\n";
        std::cout << "3. 退课\n";
        std::cout << "4. 查看成绩\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        std::cin >> choice;

        switch (choice) {
            case 1: listAllCourses(); break;
            case 2: enrollCourse(studentId); break;
            case 3: dropCourse(studentId); break;
            case 4: viewScores(studentId); break;
            case 0: std::cout << "返回上级菜单\n"; break;
            default: std::cout << "无效选择!\n";
        }
    } while (choice != 0);
}

void StudentController::listAllCourses() {
    auto courses = CourseBroker::instance().findAll();
    std::cout << "\n所有课程:\n";
    std::cout << std::left << std::setw(10) << "课程号"
              << std::setw(20) << "课程名"
              << std::setw(8) << "学分"
              << "教师号\n";
    for (const auto& c : courses) {
        std::cout << std::left << std::setw(10) << c->getId()
                  << std::setw(20) << c->getName()
                  << std::setw(8) << c->getCredits()
                  << c->getTeacherId() << std::endl;
    }
}

void StudentController::enrollCourse(const std::string& studentId) {
    std::string courseId;
    std::cout << "请输入要选的课程号: ";
    std::cin >> courseId;

    auto course = CourseBroker::instance().findById(courseId);
    if (!course) {
        std::cout << "课程不存在!\n";
        return;
    }

    auto sel = SelectionBroker::instance().findById(studentId, courseId);
    if (sel) {
        std::cout << "你已经选过这门课了!\n";
        return;
    }

    if (SelectionBroker::instance().insert(studentId, courseId)) {
        std::cout << "选课成功!\n";
    } else {
        std::cout << "选课失败，请稍后重试。\n";
    }
}

void StudentController::dropCourse(const std::string& studentId) {
    std::string courseId;
    std::cout << "请输入要退的课程号: ";
    std::cin >> courseId;

    if (SelectionBroker::instance().remove(studentId, courseId)) {
        std::cout << "退课成功!\n";
    } else {
        std::cout << "退课失败，可能未选此课。\n";
    }
}

void StudentController::viewScores(const std::string& studentId) {
    auto selections = SelectionBroker::instance().findByStudent(studentId);
    if (selections.empty()) {
        std::cout << "暂无选课记录。\n";
        return;
    }
    std::cout << "\n你的成绩:\n";
    std::cout << std::left << std::setw(10) << "课程号"
              << std::setw(20) << "课程名"
              << "成绩\n";
    for (const auto& sel : selections) {
        auto course = CourseBroker::instance().findById(sel->getCourseId());
        std::string courseName = course ? course->getName() : "未知课程";
        std::string scoreStr = sel->getScore().has_value() ? std::to_string(sel->getScore().value()) : "未录入";
        std::cout << std::left << std::setw(10) << sel->getCourseId()
                  << std::setw(20) << courseName
                  << scoreStr << std::endl;
    }
}
