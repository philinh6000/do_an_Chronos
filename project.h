// project.h
#pragma once
#include "task.h"
#include <stack>
#include <algorithm>
#include <vector>
#include <fstream> // Đọc, ghi file
#include <cstdlib> // ép tạo path & folder lưu file = lênh mkdir
//namespace fs = std::filesystem; // Đặt tên viết tắt cho filesystem nếu cần dùng

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
        void erase_from_x_to_y(); // Xóa trong khoảng được chọn
        void undo();
        void demoOverflow(); // demo để test 10000 task
        void setFolder(); // Tạo đường dẫn chứa và khởi tạo folder
        void saveToFile(); // Lưu ds vào file
        void loadFromFile(); // Đọc ds từ file
};