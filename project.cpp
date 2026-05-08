// project.cpp
#include "project.h"
#include <iostream>

// Sắp xếp task thành priority
void Project::priority(){
    std::sort(ds.begin(), ds.end());
}

// Thêm bước xác nhận đã xem xong trước khi quay lại Main Menu
void Project::back_to_menu(){
    std::cout<<"\nBam phim bat ky de quay lai Main Menu\n";
    std::string key;
    std::cin>>key;
}

// 2. Hiển thị, chỉ xem.
void Project::display() const{
    std::cout<<"\n===TASKLIST===\n\n";
    for (int i = 0; i < (int)ds.size(); i++){
        std::cout<<i<<". ";
        ds[i].output();
    }
}

// Mỗi thao tác đều lưu snapshot vào f ngay để đảm bảo an toàn.
// 1. Thêm task
void Project::add_task(){
    Task t; // gọi task được thêm vào là t
    t.input(); // Gọi hàm input để nhập tên task và deadline
    // Nếu t != -1 = isvalid
    // thêm toàn bộ t vừa nhập vào danh sách vector bằng push_back
    if (t.isValid()) {
        ds.push_back(t);
        priority(); // sắp xếp lại danh sách theo độ ưu tiên
        history.push(ds); // Lưu trạng thái hiện tại vào stack
        saveToFile(); // Lưu snapshot vào f
    }
    display(); // Hiển thị lại danh sách sau khi thêm dù thành công hay không
    back_to_menu();
}

// 3. Gia hạn
void Project::gia_han(int vi_tri){
    if (vi_tri >=0 && vi_tri < (int)ds.size()){
        ++ds[vi_tri]; // gia hạn ở vị trí được chọn
        history.push(ds); // Lưu trạng thái mới
        saveToFile();
        priority(); // Sắp xếp lại sau khi gia hạn
        display(); // Hiển thị lại danh sách sau khi gia hạn
        back_to_menu();
    }else {
        std::cout<<"Vi tri khong ton tai!!\n";
        back_to_menu();
    }
}

// 4. Xóa
void Project::deleted(int vi_tri){
    // chọn vị trí trong menu, xóa task ở vị trí được chọn.
    if (vi_tri >= 0 && vi_tri < (int)ds.size()) {
        ds.erase(ds.begin() + vi_tri);
        history.push(ds); // Lưu trạng thái
        saveToFile();
        display(); // Hiển thị lại danh sách sau khi xóa
        back_to_menu();
    }else {
        std::cout<<"Vi tri khong ton tai!!\n";
        back_to_menu();
    }
}

// 5. Xóa trong khoảng được chọn
void Project::erase_from_x_to_y(){
    std::string confirm;
    int x, y;
    std::cout<<"Chon vi tri bat dau muon xoa: ";
    std::cin>>x;
    std::cout<<"Chon vi tri ket thuc muon xoa: ";
    std::cin>>y;
    std::cout<<"Bam Y de xac nhan xoa!\n";
    std::cin>>confirm;
    if (confirm == "y" || confirm == "Y"){
        // Xóa từ vị trí bắt đầu x, đến vị trí y.
        // Vì erase sẽ dừng trước end nên +1 để xóa được ở vị trí y.
        ds.erase(ds.begin() + x, ds.begin() + y + 1);
        history.push(ds); // Lưu vào snapshot
        saveToFile(); // Lưu vào file
        display(); // Hiển thị lại danh sách sau khi xóa
        back_to_menu();
    }else {
        std::cout<<"Ban da huy xoa! Xoa that bai!!\n";
        back_to_menu();
    }
}

// 6. Hoàn tác
void Project::undo(){
    /*VD:
    ds đang có A, B, C:
    1. Thêm: history.push -> lưu lại A B C, còn A B C D thì chưa lưu
    Nhưng ds đang dừng ở trạng thái A B C D
    2. Xóa B -> history.push lưu lại ABCD -> stack có ABC ABCD và ds là ACD
    => ds đang là ACD, còn history ở vị trí top là ABCD
    3. Khi hoàn tác:*/
    // Nếu history có dữ liệu
    if (!history.empty()){
        // Lấy ABCD ra ds trở lại trước -> ds = history.top()
        ds = history.top(); // -> Lúc này ds đang là ABCD
        // Nhưng trong stack vẫn còn lưu top trên cùng là ABCD
        // Nếu không xóa -> sau đó hoàn tác sẽ luôn là ABCD => Cần history.pop
        history.pop(); // => Lúc này ds là ABCD và stack xóa ABCD rồi nên còn ABC
        // => Lần sau hoàn tác tiếp thì sẽ lại đến vị trí tiếp theo
        saveToFile();
        display(); // Hiển thị danh sách sau khi undo
        back_to_menu();
    }else {
        std::cout<<"Không có gì để hoàn tác!!\n";
        back_to_menu();
    }
}

// 7. Redo - Hủy hoàn tác, quay lại bước trước


void Project::demoOverflow(){
    std::string confirm;
    std::cout<<"Ban se khoi tao 10000 task demo de test. Bam Y de xac nhan!\n";
    std::cin>>confirm;
    if (confirm == "y" || confirm == "Y"){
        std::cout<<"Test 10000 task\n";
        for (int i = 0; i < 10000; i++){
            Task t;
            t.setFakeData(i);
            ds.push_back(t);
        }
        saveToFile();
        display();
        back_to_menu();
    }else {
        std::cout<<"Ban da huy thao tac, khong co gi xay ra!!\n";
        back_to_menu();
    }
}

// Lưu ds vào file:
// 1. Tạo đường dẫn và folder
void Project::setFolder(){
    // Khởi tạo đường dẫn
    std::string command = "mkdir \"C:\\Users\\Public\\Chronos\" > nul 2>&1";
    // "nul 2>&1" là để lệnh chạy ngầm, không hiện cmd lên.
    // Trong Window, đường dẫn đúng thường là "\\".
    // Lệnh mkdir sẽ tự bỏ qua nếu đã có folder nên không cần kiểm tra
    system(command.c_str()); // Thực thi tạo path ở command
}

// 2. Bắt đầu lưu ds vào file. Có thể tự đặt tên
void Project::saveToFile(){
    // Đảm bảo đã có folder
    setFolder();
    // Ghi vào file data = ofstream, f là tên path đến data.chronos, có thể đặt tên riêng
    std::ofstream f("C:/Users/Public/Chronos/data.chronos");
    // Khi f.is_open thì ghi từng dòng của ds vào f
    if (f.is_open()) {
        // Dùng const vì chỉ xem, kiểu auto.
        // Dùng &t tham chiếu duyệt từng dòng trong ds, không tạo bản sao.
        for (const auto &t: ds){
            // Ghi lại theo dịnh dạng task_name | deadline vào f
            f<<t.getTaksName()<<" | "<<t.getDeadline()<<"\n";
        }
        // Sau khi lưu xong thì đóng f an toàn
        f.close();
        std::cout<<"Da luu file thanh cong!!\n";
    }else std::cout<<"[ERROR]: Khong the mo file de ghi!!\n";
}

// 3. Load data từ f về lại RAM khi mở Chronos
void Project::loadFromFile(){
    // Dùng ifstream đọc file txt ở path
    std::ifstream f("C:/Users/Public/Chronos/data.chronos");
    // Nếu f chưa tồn tại -> không có gì để load
    if (!f.is_open()) return;
    // Xóa ds ở RAM hiện tại để chuẩn bị lấy ds ở f vào
    ds.clear();
    std::string line; // Tạo biến line để đọc từng hàng của f
    // Khi trong f còn hàng để bỏ vào line
    while (std::getline(f, line)){
        // Cắt dữ liệu trong file để đưa vào đúng chỗ.
        // Bên trái | là task_name, bên phải là deadline
        /*size_t: số nguyên không âm (unsigned int) cực lớn
        Nếu find('|') không tìm thấy | sẽ trả về giá trị đặc biết std::string::npos
        Là số rất lớn, nên cần dùng size_t*/
        size_t pos = line.find('|'); // Tìm | để cắt, pos là nội dung các ô giữa |
        // Nếu pos không phải dấu | thì xét
        if (pos != std::string::npos){ 
            // Lấy string là name = line.substr(0, pos): field 0 của pos
            std::string name = line.substr(0, pos);
            // Lấy time_t là time -> string to ll (số nguyên long long)
            // Trong line.substr() lấy pos + 1: lấy vị trí field 1 sau |
            std::time_t dl = std::stoll(line.substr(pos + 1));
            // Đặt Task t để đưa thành từng Task
            Task t;
            t.setDataChuan(name, dl); // Đưa name và dl vào task_name và dl
            ds.push_back(t); // đưa từng t vào ds vector trong RAM
        } 
    }
    // Load xong hết vào RAM thì đóng file an toàn
    f.close();
    // Sắp xếp lại
    priority();
}