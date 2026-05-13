// task.h
#pragma once // Read-Only 1 time
#include <string>
#include <ctime>

class Task{
    private:
        std::string task_name;
        // Dùng thư viện ctime để kết hợp struct tm và mktime tự tính time_t từ 1970
        time_t deadline;
        bool is_done; // Xác nhận task đã xong hoặc chưa
        time_t completed_date; // Kiểm tra thời gian hoàn thành
    public:
        void input(); // Hàm nhập cha
        void input_task_name(); // Chỉ nhập task name
        void input_time(); // Chỉ nhập time
        bool isValid() const {return deadline != -1;}
        bool operator <(const Task& other) const; // so sánh priority theo thời gian
        Task& operator++(); // Gia hạn thêm 1 ngày
        void output() const; // Hiển thị 1 task
        void outputDone() const; // Hiển thị task done, tương tự task thường, nhưng có done time
        void setFakeData(int i); // Tạo data giả để test
        time_t getDeadline() const {return deadline;}// Lấy deadline
        std::string getTaksName() const {return task_name;} // Lấy task name
        // Chuyển các dữ liệu string, time thành task_name, deadline
        void setDataChuan(const std::string &name, const time_t dl, const bool isD, const time_t cd); 
        int correct_val(const std::string &text, const int min_val, const int max_val); // Kiểm tra kết quả và nhập cout nhanh
        // Hàm để lấy task_name = new_name
        // thêm const vì chỉ đọc name lấy = task_name chứ không sửa new_name ở đây
        // Dùng tham chiếu để lấy luôn new_name vừa được tạo để dùng cho task_name thay vì tạo bản sao.
        void setTaskName(const std::string &new_name) {task_name = new_name;}
        void setDeadline(const time_t dl) {deadline = dl;}
        // Xử lý task done
        void markAsDone(); // Đánh dấu đã hoàn thành và lấy thời gian hoàn thành
        bool GetBoolDone() const {return is_done;} // Lấy kết quả is_done
        time_t GetCD() const {return completed_date;} // Lấy ngày hoàn thành task
};