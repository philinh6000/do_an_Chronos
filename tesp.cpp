// test.cpp
#include "task.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip> // Để dùng setfill và setw thêm số, đảm bảo định dạng 2 số cho time.


// Kiểm tra kết quả và nhập cout nhanh
int Task::correct_val(std::string text, int now){
    // Nhập text bằng cout:
    std::cout<<text;
    // Chuẩn bị kiểm tra int và kiểm tra so với now
    int gia_tri; // tạo biến nhập time.
    int i = 3; // Tạo số lần được nhập
    // Khi i lớn hơn 0 thì được nhập lại nếu sai
    while (i > 0){
        // Nếu gia_tri là số và gia_tri > now (biến now sẽ được điền theo từng field)
        if (std::cin>>gia_tri && gia_tri >= now){
            std::cin.ignore(1000, '\n'); // Dọn tối đa 1000 ký tự rác cho đến khi gặp \n
            return gia_tri; // Đúng thì trả về con số.
        }
        // Sai thì dùng cin.clear() để reset trạng thái, sẵn sàng nhận lệnh mới
        std::cin.clear();
        // Xóa dòng cin lỗi trước đó, sẵn sàng chuẩn bị nhập lại
        std::cin.ignore(1000, '\n');
        i--; // Giảm số lần được nhập
        // Gửi thông báo số lần còn được nhập lại:
        std::cout<<"Thong tin khong hop le!! Con "<<i<<" lan nhap!\n"
        <<text;
    }
    // Sau vòng lặp vẫn sai, thì báo lỗi và trả về giá trị -1 để dánh dấu xử lý trong deadline
    std::cout<<"Thong tin khong hop le!! Thao tac that bai!!";
    return -1;
}

// Nhập task name
void Task::input_task_name(){
    // Nhập tên task
    std::cout<<"Nhap ten task bang chu khong dau: ";
    std::getline(std::cin, task_name);
}

// Nhập time
void Task::input_time(){
    int d, m, y, hr, min;

    // Lấy thời gian hiện tại chuẩn bị so sánh từng phần tử của deadline với now.
    time_t now = time(0);
    struct tm* thoigian = localtime(&now);
    int nam_hien_tai = thoigian->tm_year+1900; // cắt lấy năm biến thành số để so sánh với y.
    int thang_hien_tai = thoigian->tm_mon+1; // cắt lấy tháng hiện tại thành số
    int ngay_hien_tai = thoigian->tm_mday;
    int gio_hien_tai = thoigian->tm_hour;
    int phut_hien_tai = thoigian->tm_min;

    // Nhập time
    // Nhập năm:
    y = correct_val("Nhap nam (bat buoc nhap 4 so): ", nam_hien_tai); 
    // Nếu năm là -1 = sai -> về menu luôn
    if (y == -1) return;

    // Nếu đúng thì rơi xuống nhập tháng
    // Nếu năm > now thì tháng mấy cũng được. đặt 1 vì chắc chắn không có tháng nhỏ hơn 1
    if (y > nam_hien_tai) m = correct_val("Nhap thang: ", 1);
    // Nếu năm = now thì tháng phải lớn hơn tháng hiện tại.
    else m = correct_val("Nhap thang: ", thang_hien_tai);

    if (m == -1) return; // Sai thì thoát ra menu

    // Tương tự với ngày, giờ, phút. Giả sử năm 2027 thì tháng 1 cũng > now.
    if (y > nam_hien_tai || m > thang_hien_tai) d = correct_val("Nhap ngay: ", 1);
    else d = correct_val("Nhap ngay: ", ngay_hien_tai);
    if (d == -1) return;

    // Chọn giờ mặc định hoặc nhập giờ phút: Nếu >= 24 giờ thì tự động lấy giờ mặc định
    if (y > nam_hien_tai || m > thang_hien_tai || d > ngay_hien_tai) hr = correct_val("Nhap gio (Nhap >= 24 de lay gio mac dinh 23:59): ", 0);
    else hr = correct_val("Nhap gio (Nhap >= 24 de lay gio mac dinh 23:59): ", gio_hien_tai);

    if (hr == -1) return; // Nếu giờ nhập ra sai thì return
    // Nếu ra đúng mà số giờ >= 24 thì lấy thời gian mặc định, bỏ qua nhập phút.
    if (hr >= 24){
        hr = 23;
        min = 59;
    }
    // Ngược lại thì hr vẫn giữ nguyên và tiến đến nhập phút
    else{
        if (y > nam_hien_tai || m > thang_hien_tai || d > ngay_hien_tai || hr > gio_hien_tai) min = correct_val("Nhap phut: ", 0);
        else min = correct_val("Nhap phut: ", phut_hien_tai + 1); // Lấy dư ra 1 phút so với hiện tại tránh lệch giờ
        if (min == -1) return;}
    // Nếu đúng hết thì xử lý: Chuyển đổi thời gian từ số nguyên thành giờ
    struct tm t = {0};
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    t.tm_hour = hr;
    t.tm_min = min;
    
    deadline = mktime(&t); // Lấy được thời gian deadline
}
// Nhập tên task và thời gian deadline
void Task::input(){
    input_task_name();
    input_time();
}