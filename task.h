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
        void input(); // Hàm nhập cha
        void input_task_name(); // Chỉ nhập task name
        void input_time(); // Chỉ nhập time
        bool isValid() const {return deadline != -1;}
        bool operator <(const Task& other) const; // so sánh priority theo thời gian
        Task& operator++(); // Gia hạn thêm 1 ngày
        void output() const; // Hiển thị 1 task
        void setFakeData(int i); // Tạo data giả để test
        time_t getDeadline() const ;// Lấy deadline
        std::string getTaksName() const; // Lấy task name
        // Chuyển các dữ liệu string, time thành task_name, deadline
        void setDataChuan(std::string name, time_t dl); 
        int correct_val(std::string text, int min_val, int max_val); // Kiểm tra kết quả và nhập cout nhanh
        // Lấy task_name và name bằng new name
        void setTaskName(std::string name) {task_name = name;}
        void setDeadline(time_t dl) {deadline = dl;}
};