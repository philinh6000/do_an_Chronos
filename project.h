// project.h
#pragma once
#include "task.h"
#include <vector>
#include <stack>
#include <algorithm>
class Project{
    private:
        // Tạo vector chứa ds
        std::vector<Task> ds;
        // Tạo stack chứa history
        std::stack<std::vector<Task>> history;
    public:
        // Tạo các hàm với vector để điều phối trong main
        // Không cần đưa vector vào tham số vì đã ở trong private
        void priority(); // Sắp xếp thứ tự ưu tiên
        void add_task(); // 1. Thêm
        void display() const; // 2. Hiển thị - chỉ xem
        void gia_han(int vi_tri); // 3. Gia hạn
        void deleted(int vi_tri); // Xóa 1 task trong danh sách
        void undo();
};