// task.h
#pragma once // Read-Only 1 time
#include <string>
#include <ctime>


class Task{
    private:
        std::string task_name;
        // Dùng thư viện ctime để kết hợp struct tm và mktime tự tính time_t từ 1970
        time_t deadline; 
    public:
        void input(); // Nhập ngày
        bool isValid() const {return deadline != -1;} // Thêm bool đánh dấu deadline < now
        bool operator <(const Task& other) const; // so sánh priority theo thời gian
        Task& operator++(); // Gia hạn thêm 1 ngày
        void output() const; // Hiển thị 1 task
        void setFakeData(int i); // Tạo data giả để test
        int getDeadline() const; // Lấy deadline, dùng const vì chỉ xem
        std::string getTaksName() const; // Lấy task name
        // Chuyển các dữ liệu string, time thành task_name, deadline
        void setDataChuan(std::string name, time_t dl); 
};