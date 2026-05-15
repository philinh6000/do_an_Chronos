// project.h
#pragma once
#include "task.h"
#include <stack>
#include <algorithm>
#include <fstream> // Đọc, ghi file
#include <sstream> // Dùng stringstream cắt chuỗi trong file
#include <cstdlib> // ép tạo path & folder lưu file = lênh mkdir
//namespace fs = std::filesystem; // Đặt tên viết tắt cho filesystem nếu cần dùng

class Project{
    private:
        // Tạo vector chứa ds
        std::vector<Task> ds;
        // Tạo stack chứa history
        std::stack<std::vector<Task>> history;
        // Tạo stack để redo (tiến tới khi undo nhầm)
        std::stack<std::vector<Task>> redo;
    public:
        // Tạo các hàm với vector để điều phối trong main
        // Không cần đưa vector vào tham số vì đã ở trong private
        void priority(); // Sắp xếp thứ tự ưu tiên
        void add_task(); // Thêm
        void display() const; // Hiển thị - chỉ xem
        void display_done() const; // Hiển thị task done
        void update_task(); // Cập nhật task
        void erase_from_x_to_y(); // Xóa trong khoảng được chọn
        void undo();
        void Redo();
        void demoOverflow(); // demo để test 10000 task
        void setFolder(); // Tạo đường dẫn chứa và khởi tạo folder
        void saveToFile(); // Lưu ds vào file
        void loadFromFile(); // Đọc ds từ file                                                                                                                   VB=
        void sleep(int s); // Gọi sleep nhanh
        // Chỉ hiển thị text thông báo -> chỉ xem
        // và không được dùng để tham chiếu ở đâu nên không cần &
        void back_to_menu(const std::string text); 
};