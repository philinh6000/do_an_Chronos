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
        static const int DaysInMonth[]; // Tạo mảng đặt ngày trong tháng để lấy ngày chuẩn
        bool is_leap(int y) const; // Tạo bool xác nhận có phải năm nhuận hay không
        // Điểm mức độ ưu tiên và điểm khẩn cấp
        time_t begin_time; // Lấy giờ bắt đầu tạo task
        int priority_score; // Điểm ưu tiên từ 1-5 do user nhập, không đổi
        static const std::string Priority[]; // Tạo mảng đặt mức độ ưu tiên
    public:
        // Điểm mức độ ưu tiên và điểm khẩn cấp
        void input_priority(); // Hàm nhập điểm priority
        std::string output_Priority() const; // In loại priority từ điểm
        double urgency_score() const; // Điểm khẩn cấp thay đổi theo realtime mỗi lần gọi

        // Input
        void input(); // Hàm nhập cha
        void input_task_name(); // Chỉ nhập task name
        void input_time(); // Chỉ nhập time
        void input_flag_done(); // Nhập flag done false + complete time 0 cho task mới

        //---

        int correct_val(const std::string &text, const int min_val, const int max_val); // Kiểm tra kết quả và nhập cout nhanh    
        bool isValid() const {return deadline != -1;}
        bool operator <(const Task& other) const; // so sánh priority theo thời gian
        Task& operator++(); // Gia hạn thêm 1 ngày
        void output() const; // Hiển thị 1 task
        void outputDone() const; // Hiển thị task done, tương tự task thường, nhưng có done time
        void setFakeData(int i); // Tạo data giả để test

        // Hàm để lấy task_name = new_name
        // thêm const vì chỉ đọc name lấy = task_name chứ không sửa new_name ở đây
        // Dùng tham chiếu để lấy luôn new_name vừa được tạo để dùng cho task_name thay vì tạo bản sao.
        void setTaskName(const std::string &new_name) {task_name = new_name;}
        void setDeadline(const time_t dl) {deadline = dl;}
        // Chuyển các dữ liệu string, time thành task_name, deadline
        void setDataChuan(const time_t bg, const std::string &name, const time_t dl, const int psc, const bool isD, const time_t cd); 
       
        // Xử lý task done
        void markAsDone(); // Đánh dấu đã hoàn thành và lấy thời gian hoàn thành
        bool GetBoolDone() const {return is_done;} // Lấy kết quả is_done
        
        // --- Lấy thông tin ---
        time_t getDeadline() const {return deadline;}// Lấy deadline
        std::string getTaksName() const {return task_name;} // Lấy task name
        int getDay(int m, int y) const; // Lấy được ngày trong tháng theo năm.
        time_t GetCD() const {return completed_date;} // Lấy ngày hoàn thành task
        time_t getBeginDate() const {return begin_time;} // Lấy ngày bắt đầu tạo task
        int getPriority() const {return priority_score;} // Lấy điểm priority
};