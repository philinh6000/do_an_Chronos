### Cách để lấy thời gian trong khoảng, không cộng dồn:
```c++
// Hàm này lo hết việc: Nhập, Check số/chữ, Check số lượt (3 lần)
int nhapSoAnToan(std::string thong_bao, int min_val, int max_val) {
    int gia_tri;
    int i = 3;
    while (i > 0) {
        std::cout << thong_bao << " (Con " << i << " lan): ";
        if (std::cin >> gia_tri && gia_tri >= min_val && gia_tri <= max_val) {
            std::cin.ignore(1000, '\n'); // Dọn Enter thừa
            return gia_tri; // Nhập đúng thì thoát luôn
        }
        // Nếu sai (chữ hoặc ngoài khoảng)
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        i--;
        std::cout << "Gia tri khong hop le!\n";
    }
    return -1; // Trả về -1 nếu sau 3 lần vẫn sai
}
```

### Cách chọn nhập giờ mặc định ngay trong nhập giờ:
```c++
std::string input_str;
int i = 3;
while (i > 0) {
    std::cout << "Nhap gio (hoac nhap 'A' de lay 23:59): ";
    std::cin >> input_str;

    // 1. Nếu user nhập 'A' hoặc 'a'
    if (input_str == "A" || input_str == "a") {
        hr = 23; min = 59;
        std::cin.ignore(1000, '\n');
        return; // Xong luôn cả giờ và phút!
    }

    // 2. Nếu không phải 'A', thử biến chuỗi đó thành số
    try {
        int temp_hr = std::stoi(input_str); // Hàm này biến string thành int
        if (temp_hr >= moc_gio_hien_tai && temp_hr <= 23) {
            hr = temp_hr;
            std::cin.ignore(1000, '\n');
            break; // Nhập giờ xong, xuống nhập phút tiếp
        }
    } catch (...) { 
        // Nếu nhập chữ linh tinh không phải 'A', stoi sẽ báo lỗi, rơi vào đây
    }

    std::cin.clear();
    std::cin.ignore(1000, '\n');
    i--;
    std::cout << "Khong hop le! Con " << i << " lan.\n";
}
```

### Mô tả [THUẬT TOÁN TÍNH ĐIỂM KHẨN CẤP ĐỘNG - DYNAMIC URGENCY SCORE]
```C++
// 1. Khi khởi tạo Task (Tính toán tĩnh - Cố định):
   - base_priority (1 - 5)
   - pscore = (base_priority / 5.0) * 60.0                     // Trọng số Priority (Max 60)
   - max_time_score = 100.0 - pscore                           // Quỹ điểm dành cho Thời gian (Biến thiên động)
   - total_hrs = deadline_timestamp - begin_timestamp          // Tổng quỹ thời gian của task (Hours)
   - hourly_rate = max_time_score / total_hrs                  // Vận tốc tăng điểm mỗi giờ

// 2. Khi hiển thị danh sách (Tính toán động - Real-time):
   - hrs_left = deadline_timestamp - current_timestamp         // Số giờ còn lại
   - hrs_elapsed = total_hrs - hrs_left                        // Số giờ đã trôi qua kể từ khi tạo
   
   - Nếu hrs_left <= 0: urgency_score = 100.0                  // Xử lý quá hạn (Edge case)
   - Ngược lại:        urgency_score = pscore + (hrs_elapsed * hourly_rate)
```
```C++
Thêm điểm khẩn cấp: Tổng 100 điểm
- pscore: cố định từ 12 - 60 (tối đa 60%) double pscore = priority_score/5.0 * 60 -> tính 1 lần khi tạo task
- %hrscore = 100 - pscore. VD: pscore = 60 -> %hrscore = 40; pscore = 12 -> %hrscore = 88 -> luôn đủ 100 điểm dù p bao nhiêu -> tính 1 lần khi tạo task
- hrscore = %hrscore/hrsleft -> Số điểm phải tăng mỗi giờ -> tính 1 lần khi tạo task
- total_hrs = deadline - begin -> tổng số giờ -> tính 1 lần khi tạo task

- hrsleft_realtime = deadline - now -> số giờ còn lại -> tính mỗi lần gọi display trong hàm tính uscore
- hrsup = total_hrs - hrsleft_realtime -> số giờ đã trôi qua -> tính mỗi lần gọi display trong hàm tính uscore
- uscore = pscore + hrsup*hrscore -> tính mỗi lần gọi display trong hàm tính uscore

"""Ví dụ:
T1: pscore = 12 - total_hrs = 40
T2: pscore = 60 - total_hrs = 40
T3: pscore = 60 - total_hrs = 20

Lúc này, các giá trị tính ra của 3 task sẽ là:

T1: %hrscore = 88 - hrscore 88/40 = 2.2
T2: %hrs = 40 - hrs = 1
T3: %hrs = 40 - hrs = 40/20 = 2

Lúc này tính uscore cho các task:
Giả sử trôi qua 1hr
T1: hrsleft = 39 - hrsup = 1 → usc = 12 + 1*2.2 = 14.2
T2: usc = 60 + 1*1 = 61
T3: usc = 60 + 1*2 = 62

Khi đạt tới mốc 19 điểm (gần hết giờ của task 3)
T1: usc = 12 + 19*2.2 = 53.8
T2: usc = 60 + 19*1 = 79
T3: usc = 60 + 19*2 = 98 → đúng, vì tới deadline rồi

Tiếp theo là mốc khi 2 task trên tới 39hr (sát hạn deadline)
T1: usc = 12 + 39*2.2 = 97.8
T2: usc = 60 + 39*1 = 99 → đúng, prio cao hơn thì điểm cao hơn dù cùng sắp hết giờ.

Khi hrsleft còn 0 → tất cả chắc chắn đạt 100."""
```