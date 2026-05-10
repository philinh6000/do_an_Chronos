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