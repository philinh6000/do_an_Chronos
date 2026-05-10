// project.cpp
#include "project.h"
#include <iostream>
#include <thread> // Để dùng sleep thay cho back_to_menu
#include <chrono> // Để dùng sleep thay cho back_to_menu

// Sắp xếp task thành priority
void Project::priority(){
    std::sort(ds.begin(), ds.end());
}

// Thêm bước xác nhận đã xem xong trước khi quay lại Main Menu
void Project::back_to_menu(std::string text){
    std::cout<<text;
    std::string key;
    do{
        std::cin>>key;
    }
    while(key != "0");
    if (key == "0") return;
}

// Gọi sleep nhanh
void Project::sleep(int s){
    std::this_thread::sleep_for(std::chrono::seconds(s));
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
    // Bọc an toàn:
    if (t.isValid()){
        history.push(ds);
        ds.push_back(t);
        priority(); // sắp xếp lại danh sách theo độ ưu tiên
        redo.push(ds); // Lưu trạng thái hiện tại vào stack
        saveToFile(); // Lưu snapshot vào f
        std::cout<<"Luu task thanh cong! Bam 2 de xem lai!\n";
    }
    sleep(2); // Dừng một chút để xem thông báo.
}

// Sua task
void Project::sua_task(){
    display(); // Hiển thị ra trước để xem task cần sửa
    // Gọi task cần sửa là Task t
    Task t;
    int choose_task;
    // Chọn task muốn sửa
    choose_task = t.correct_val("Chon task muon sua: ", 0);
    // Nếu task không tồn tại
    if (choose_task < 0 || choose_task >= (int)ds.size()) {
        std::cout<<"Vi tri khong ton tai!!";
        sleep(2);
    }else{
        // Nếu đúng -> Lưu trước khi sửa.
        history.push(ds);

        // Xử lý nếu nội dung được chọn không có trong menu
        // Chọn loại field muốn sửa:
        std::string choose_field;
        std::cout<<"1. Sua Task Name\n2. Sua Deadline\n";
        std::cin>>choose_field;
        std::cin.ignore();
        // Nếu chọn field 1
        if (choose_field == "1") {
            std::string new_name;
            std::cout<<"Nhap ten task moi: ";
            std::getline(std::cin, new_name);
            ds[choose_task].setTaskName(new_name);
        }
        // Chọn 2 thì nhập deadline
        else if (choose_field == "2"){
            ds[choose_task].input_time();
        }
        // Nhập ký tự khác thì return
        else {
            std::cout<<"[ERROR] Field không tồn tại!!";
            sleep(2);
        }
        priority(); // Sắp xếp lại
        redo.push(ds); // Lưu vào redo
        saveToFile(); // Lưu vào ổ đĩa
        std::cout<<"Sua task thanh cong! Bam 2 de xem lai!\n";
        sleep(2);}
}
// Gia hạn
void Project::gia_han(){
    display(); // Hiện thị để lựa vị trí
    int vi_tri;
    Task t;
    vi_tri = t.correct_val("Nhap vi tri: ", 0);
    if (vi_tri < 0 || vi_tri >= (int)ds.size()) {
        std::cout<<"Vi tri khong ton tai!\n";
        sleep(2);
    }else{
        // Nếu thỏa điều kiện thì thực thi:
        history.push(ds);
        ++ds[vi_tri]; // gia hạn ở vị trí được chọn
        redo.push(ds); // Lưu trạng thái sau khi cập nhật để redo
        saveToFile();
        priority(); // Sắp xếp lại sau khi gia hạn
        std::cout<<"Gia han thanh cong! Bam 2 de xem lai!\n";
        sleep(2);}
}

// Xóa
void Project::deleted(){
    display();
    int vi_tri;
    Task t;
    vi_tri = t.correct_val("Nhap vi tri: ", 0);
    if (vi_tri < 0 || vi_tri >= (int)ds.size()) {
        std::cout<<"Vi tri khong ton tai!\n";
        sleep(2);
    }else{
        history.push(ds); // Lưu trạng thái undo
        ds.erase(ds.begin() + vi_tri);
        redo.push(ds); // Lưu trạng thái
        saveToFile();
        std::cout<<"Xoa task thanh cong! Bam 2 de xem lai!\n";
        sleep(2);}
}

// Xóa trong khoảng được chọn
void Project::erase_from_x_to_y(){
    std::string confirm;
    int x, y;
    std::cout<<"Chon vi tri bat dau muon xoa: ";
    std::cin>>x;
    std::cout<<"Chon vi tri ket thuc muon xoa: ";
    std::cin>>y;
    if ((x >= 0 && x < (int)ds.size()) && (y > 0 && y < (int)ds.size())){
        std::cout<<"Bam Y de xac nhan xoa! Bam phim khac de tu choi!\n";
        std::cin>>confirm;
        if (confirm == "y" || confirm == "Y"){
            // Xóa từ vị trí bắt đầu x, đến vị trí y.
            // Vì erase sẽ dừng trước end nên +1 để xóa được ở vị trí y.
            history.push(ds);
            ds.erase(ds.begin() + x, ds.begin() + y + 1);
            redo.push(ds); // Lưu vào snapshot
            saveToFile(); // Lưu vào file
            std::cout<<"Xoa task thanh cong! Bam 2 de xem lai!\n";
            sleep(2);
        }else {
            std::cout<<"Xoa that bai!!\n";
            sleep(2);
        }
    }
}

// Hoàn tác
void Project::undo(){
    /*VD: ds đang có A, B, C:
    1. Thêm: history.push -> lưu lại A B C, còn A B C D thì chưa lưu
    Nhưng ds đang dừng ở trạng thái A B C D
    2. Xóa B -> history.push lưu lại ABCD -> stack có ABC ABCD và ds là ACD
    => ds đang là ACD, còn history ở vị trí top là ABCD
    3. Khi hoàn tác:*/
    // Nếu history có dữ liệu
    if (!history.empty()){
        // Đưa vào redo trước
        redo.push(ds);
        // Lấy ABCD ra ds trở lại trước -> ds = history.top()
        ds = history.top(); // -> Lúc này ds đang là ABCD
        // Nhưng trong stack vẫn còn lưu top trên cùng là ABCD
        // Nếu không xóa -> sau đó hoàn tác sẽ luôn là ABCD => Cần history.pop
        history.pop(); // => Lúc này ds là ABCD và stack xóa ABCD rồi nên còn ABC
        // => Lần sau hoàn tác tiếp thì sẽ lại đến vị trí tiếp theo
        saveToFile();
        std::cout<<"Undo thanh cong! Bam 2 de xem lai!\n";
        sleep(2);
    }else {
        std::cout<<"Khong co gi de Undo!!\n";
        sleep(2);
    }
}

// Redo - Hủy hoàn tác, quay lại bước trước
void Project::Redo(){
    /*VD: Thêm 5 task -> 1 2 3 4 5 -> undo 5 lần dừng ở vị trí 0
    -> lưu ngược vào redo có: 5 4 3 2 1
    -> Khi redo sẽ từ vị trí 0 lấy ngược lại 1 2 3 4 5, dừng lại tùy thích.
    Giả sử redo 3 lần -> 1 2 3 -> lưu ngược history 1 2 3
    -> Khi undo sẽ lại lùi ngược 3 2 1*/
    if (!redo.empty()){
        // Lưu vào history trước
        history.push(ds); 
        ds = redo.top(); // Lấy redo trên cùng ra
        redo.pop(); // Xóa cái trên cùng đi
        saveToFile(); // Lưu lại
        std::cout<<"Redo thanh cong! Bam 2 de xem lai!\n";
        sleep(2);
    }else {
        std::cout<<"Khong co gi de Redo!!\n";
        sleep(2);
    }
}

void Project::demoOverflow(){
    std::string confirm;
    std::cout<<"Ban se khoi tao 10000 task demo de test. Bam Y de xac nhan! Bam phim khac de tu choi!\n";
    std::cin>>confirm;
    if (confirm == "y" || confirm == "Y"){
        std::cout<<"Test 10000 task\n";
        for (int i = 0; i < 10000; i++){
            Task t;
            t.setFakeData(i);
            ds.push_back(t);
        }
        saveToFile();
        std::cout<<"Tao Demo thanh cong! Bam 2 de xem lai!\n";
        sleep(2);
    }else {
        std::cout<<"Ban da huy thao tac, khong co gi xay ra!!\n";
        sleep(2);
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
            // Lọc bỏ dòng data rác với deadline = -1 để an toàn
            if (!t.isValid()) continue;
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
            // Lọc bỏ line hỏng
            if (dl <= 0) continue;
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