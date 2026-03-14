// Module
// File: main.cpp   Version: 0.1.0   License: AGPLv3
// Created: 何家欢      2026-03-14 20:44:36
// Description:
//
import <iostream>;
import <string>;
import :applogic.studentcontroller;
import :applogic.teachercontroller;
import :applogic.secretarycontroller;

void studentLoop() {
    std::string id;
    std::cout << "请输入你的学号: ";
    std::cin >> id;
    StudentController ctrl;
    ctrl.showMenu(id);
}

void teacherLoop() {
    std::string id;
    std::cout << "请输入你的教师号: ";
    std::cin >> id;
    TeacherController ctrl;
    ctrl.showMenu(id);
}

void secretaryLoop() {
    SecretaryController ctrl;
    ctrl.showMenu();
}

int main() {
    std::cout << "======== 选课系统 ========\n";
    int role;
    do {
        std::cout << "请选择角色:\n";
        std::cout << "1. 学生\n2. 教师\n3. 教学秘书\n0. 退出\n";
        std::cout << "选择: ";
        std::cin >> role;

        switch (role) {
            case 1: studentLoop(); break;
            case 2: teacherLoop(); break;
            case 3: secretaryLoop(); break;
            case 0: std::cout << "系统退出。\n"; break;
            default: std::cout << "无效角色!\n";
        }
    } while (role != 0);
    return 0;
}

