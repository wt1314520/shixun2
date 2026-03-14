// Module
// File: teachercontroller.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何家欢      2026-03-14 20:40:36
// Description:
//
#include <iostream>
#include <iomanip>
#include <string>
#include <optional>
export module registrar:applogic.teachercontroller;
import :domain.teacher;
import :domain.course;
import :domain.selection;
import :dm.teacherbroker;
import :dm.coursebroker;
import :dm.selectionbroker;

export class TeacherController {
public:
    void showMenu(const std::string& teacherId);
    void listMyCourses(const std::string& teacherId);
    void enterScores(const std::string& teacherId);
};

void TeacherController::showMenu(const std::string& teacherId) {
    int choice;
    do {
        std::cout << "\n===== 教师菜单 =====\n";
        std::cout << "1. 查看我教的课程\n";
        std::cout << "2. 录入成绩\n";
        std::cout << "0. 返回\n";
        std::cout << "请选择: ";
        std::cin >> choice;

        switch (choice) {
            case 1: listMyCourses(teacherId); break;
            case 2: enterScores(teacherId); break;
            case 0: std::cout << "返回上级菜单\n"; break;
            default: std::cout << "无效选择!\n";
        }
    } while (choice != 0);
}

void TeacherController::listMyCourses(const std::string& teacherId) {
    auto allCourses = CourseBroker::instance().findAll();
    std::cout << "\n您教授的课程:\n";
    std::cout << std::left << std::setw(10) << "课程号"
              << std::setw(20) << "课程名"
              << "学分\n";
    for (const auto& c : allCourses) {
        if (c->getTeacherId() == teacherId) {
            std::cout << std::left << std::setw(10) << c->getId()
                      << std::setw(20) << c->getName()
                      << c->getCredits() << std::endl;
        }
    }
}

void TeacherController::enterScores(const std::string& teacherId) {
    std::string courseId;
    std::cout << "请输入要录入成绩的课程号: ";
    std::cin >> courseId;

    auto course = CourseBroker::instance().findById(courseId);
    if (!course || course->getTeacherId() != teacherId) {
        std::cout << "您无权录入此课程的成绩！\n";
        return;
    }

    auto selections = SelectionBroker::instance().findByCourse(courseId);
    if (selections.empty()) {
        std::cout << "该课程暂无选课学生。\n";
        return;
    }

    for (auto& sel : selections) {
        auto student = StudentBroker::instance().findById(sel->getStudentId());
        std::string studentName = student ? student->getName() : "未知";
        std::cout << "学生 " << studentName << " (" << sel->getStudentId() << ")";
        if (sel->getScore().has_value()) {
            std::cout << " 已有成绩: " << sel->getScore().value() << "，是否修改？(y/n): ";
            char ch;
            std::cin >> ch;
            if (ch != 'y' && ch != 'Y')
                continue;
        }
        int score;
        std::cout << " 请输入成绩(0-100): ";
        std::cin >> score;
        if (score < 0 || score > 100) {
            std::cout << "成绩无效，跳过。\n";
            continue;
        }
        if (SelectionBroker::instance().updateScore(sel->getStudentId(), courseId, score)) {
            std::cout << "录入成功。\n";
        } else {
            std::cout << "录入失败。\n";
        }
    }
}
