// task.cpp
#include "task.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

// Nhập tên task và thời gian deadline
void Task::input(){
    int d, m, y, hr, min;
    // Nhập tên task
    std::cout<<"Nhap ten task bang chu khong dau: ";
    std::getline(std::cin, task_name);
    // Nhập time
    std::cout<<"Nhap ngay ket thuc: ";
    std::cin>>d;
    std::cout<<"Nhap thang ket thuc: ";
    std::cin>>m;
    std::cout<<"Nhap nam ket thuc: ";
    std::cin>>y;
    std::cout<<"Nhap gio ket thuc: ";
    std::cin>>hr;
    std::cout<<"Nhap phut ket thuc: ";
    std::cin>>min;
    std::cin.ignore();

    // Xử lý năm sai định dạng:
    if (y < 100) y += 2000; // Nếu < 100 thì thêm 2000. VD: 26 + 2000 = 2026

    // Chuyển đổi thời gian từ số nguyên thành giờ
    struct tm t = {0};
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    t.tm_hour = hr;
    t.tm_min = min;

    deadline = mktime(&t); // Lấy được thời gian deadline

    time_t now = time(0); // Lấy thời gian hiện tại
    if (deadline < now) { // So sánh nếu deadline < now
        std::cout << "[ERROR] Deadline nho ngay hien tai! Luu task that bai!\n";
        deadline = -1; // đánh dấu không hợp lệ
    }
}

// Đặt điều kiện so sánh nếu deadline < hơn.
bool Task::operator<(const Task& other) const{
    return this->deadline < other.deadline; // Ngày nhỏ hơn thì ưu tiên hơn
}

Task& Task:: operator++(){
    deadline +=86400; // Khi gọi operator++ thì deadline tự tăng 1 ngày
    return *this; // Trả về nguyên object Task thay vì chỉ có con số daedline
}

/*Ví dụ:
Task t;
++(++t); // tăng 2 lần liên tiếp — được vì return *this
Nếu return deadline thì dòng trên bị lỗi vì lần ++ thứ 2 không có object để gọi nữa.*/

/*Cách gọi trong menu:
if (choose == 5) {
    ++ds[viTri]; // gọi operator++ của task tại vị trí viTri
}*/

/*Vì sao không dùng if (choose) deadline +=86400 luôn?

// Cách thủ công — phải nhớ deadline là gì, viết lại mỗi lần:
if (choose == 5) ds[i].deadline += 86400; // lỗi: deadline là private!

// Cách dùng operator++ — gọn, an toàn, deadline vẫn private:
if (choose == 5) ++ds[i];*/

void Task::output() const{
    // Chuyển deadline thành dạng chuẩn dd/mm/yyyy hr:min
    struct tm* thoigian = localtime(&deadline);
    // In ra theo dạng thời gian, ngược lại với lúc chuyển d, m, y thành deadline
    std::cout<<task_name<<" | "
    <<thoigian->tm_mday<<"/" // dùng -> để lấy tương tự this, đưa thời gian thực tế để chuyển đổi
    <<(thoigian->tm_mon+1)<<"/" // bỏ trong ngoặc để thực hiện +1, trả lại số nguyên đúng (trong C++, tháng bắt đầu từ 0)
    <<(thoigian->tm_year+1900)<<" " // Tương tự, ngược lại lúc chuyển qua giây
    <<thoigian->tm_hour<<":"
    <<thoigian->tm_min<<"\n";
}

void Task::setFakeData(int i){
    task_name = "Task thu " + std::to_string(i); // task name = số thứ tự
    deadline = time(0) + (i*3600); // deadline lấy hiện tại + 1hr mỗi lần tạo
}

// Lấy deadline và task name để dùng ở nhiều tính năng
int Task::getDeadline() const{
    return deadline;
}

std::string Task::getTaksName() const{
    return task_name;
}

void Task::setDataChuan(std::string name, time_t dl){
    task_name = name; // Lấy task_name = thông tin string mới
    deadline = dl; // Lấy deadline = thông tin time mới
}