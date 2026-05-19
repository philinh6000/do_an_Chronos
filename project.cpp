// project.cpp
#include "project.h"
#include <iostream>
#include <thread> // Để dùng sleep thay cho back_to_menu
#include <chrono> // Để dùng sleep thay cho back_to_menu
#ifdef _WIN32
    #include <windows.h> // Dùng để chạy SetFileAttributes ẩn file
#else
    #include <unistd.h>
#endif

//=== UTILS ===
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
}

// Gọi sleep nhanh
void Project::sleep(int s){
    std::this_thread::sleep_for(std::chrono::seconds(s));
}

// 2. Hiển thị task chưa done
void Project::display() const{
    std::cout<<"\n===TASKLIST===\n\n";
    int i = 0;
    for (const auto &t:ds){
        // Nếu task chưa done thì in ra
        if (!t.GetBoolDone()){
            std::cout<<"Vi tri Task: "<<i<<". ";
            t.output();
        }
        i++;
    }
    std::cout<<"\n"; // Thêm 1 enter khi in xong để cách đẹp với menu
}
// Hiển thị task done
void Project::display_done() const{
    std::cout<<"\n===COMPLETED TASK===\n\n";
    int i = 0;
    for (const auto &t:ds){
        // Nếu task done thì in ra
        if (t.GetBoolDone()){
            std::cout<<"Vi tri Task: "<<i<<". ";
            t.outputDone();
        }
        i++;
    }
    std::cout<<"\n";
}

// === FUNCTION ===
// Mỗi thao tác đều lưu snapshot vào f ngay để đảm bảo an toàn.
// 1. Thêm task
void Project::add_task(){
    Task t; // gọi task được thêm vào là t
    t.input(); // Gọi hàm input để nhập tên task và deadline
    // Bọc an toàn:
    if (t.isValid()){
        // Không dùng std::move vì cần giữ quá khứ, nếu sửa trên history sẽ làm mất bản cũ
        history.push(ds); 
        ds.push_back(t);
        priority(); // sắp xếp lại danh sách theo độ ưu tiên
        redo.push(ds); // Lưu trạng thái hiện tại vào stack
        saveToFile(); // Lưu snapshot vào f
        std::cout<<"Luu task thanh cong!!\n";
    }
    sleep(2); // Dừng một chút để xem thông báo.
}

// Sửa task
void Project::update_task(){
    display(); // Hiển thị ra trước để xem task cần sửa
    // Gọi task cần sửa là Task t
    Task t;
    int choose_task;
    // Chọn task muốn sửa, nếu sai sẽ báo thất bại và quay về main menu
    choose_task = t.correct_val("Chon task muon sua: ", 0, (int)ds.size()-1);
    if (choose_task == -1) sleep(2);
    if (choose_task != -1){
        // Nếu đúng -> Lưu trước khi sửa.
        history.push(ds);

        // Xử lý nếu nội dung được chọn không có trong menu
        // Chọn loại field muốn sửa:
        std::string choose_field;
        int i = 3;
        // Nếu chọn field 1
        do{
            system("cls");
            std::cout<<"===UPDATE MENU===\n\n";
            std::cout<<"1. Rename\n2. Sua Deadline\n3. Xac nhan da hoan thanh\n4. Gia han\n5. Xoa\n0. Back to Main Menu\n";
            std::cout<<"Chon: ";
            std::cin>>choose_field;
            std::cin.ignore();
            if (choose_field == "0") {
                std::cout<<"Back to Main Menu\n";
                break;
            }
            else if (choose_field == "1") {
                std::string new_name;
                std::cout<<"Nhap ten task moi: ";
                std::getline(std::cin, new_name);
                ds[choose_task].setTaskName(new_name);
                std::cout<<"Sua ten task thanh cong!!\n";
            }
            // Chọn 2 thì nhập deadline
            else if (choose_field == "2") {
                ds[choose_task].input_time();
                std::cout<<"Sua deadline thanh cong!!\n";
            }
            // Chọn 3 để xác nhận task đã hoàn thành
            else if (choose_field == "3") {
                ds[choose_task].markAsDone();
                std::cout<<"Xac nhan Task: "<<choose_task<<" da hoan thanh!!!";
            }
            // Gia hạn
            else if (choose_field == "4") {
                ++ds[choose_task]; // gia hạn ở vị trí được chọn
                std::cout<<"Gia han thanh cong!!\n";
            }
            // Xoa
            else if (choose_field == "5") {
                ds.erase(ds.begin() + choose_task);
                std::cout<<"Xoa task thanh cong!!\n";
            }
            // Nhập ký tự khác thì return
            else {
                std::cout<<"[ERROR] Field khong ton tai!! Con "<<i<<" lan nhap\n";
                sleep(1);
            }
            i--;
        }
        // Nếu nhập linh tinh thì có 3 cơ hội
        // Nếu nhập 0 thì thoát về main menu ngay
        while ((choose_field != "0" && choose_field !="1" && choose_field != "2" && choose_field != "3" && choose_field != "4" && choose_field != "5") && i != 0); 
        priority(); // Sắp xếp lại
        redo.push(ds); // Lưu vào redo
        saveToFile(); // Lưu vào ổ đĩa
        sleep(2);
    }
}

// Xóa trong khoảng được chọn
void Project::erase_from_x_to_y(){
    display_done();
    std::string confirm;
    Task t;
    int x, y;
    x = t.correct_val("Chon vi tri bat dau muon xoa: ", 0, (int)ds.size()-1);
    if (x != -1){
        y = t.correct_val("Chon vi tri ket thuc muon xoa: ", 0, (int)ds.size()-1);
        if (x >= 0 && y < (int)ds.size() && x <= y){
            std::cout<<"Bam Y de xac nhan xoa! Bam phim khac de tu choi!\n";
            std::cin>>confirm;
            if (confirm == "y" || confirm == "Y"){
                // Xóa từ vị trí bắt đầu x, đến vị trí y.
                // Vì erase sẽ dừng trước end nên +1 để xóa được ở vị trí y.
                history.push(ds);
                ds.erase(ds.begin() + x, ds.begin() + y + 1);
                redo.push(ds); // Lưu vào snapshot
                saveToFile(); // Lưu vào file
                std::cout<<"Xoa task thanh cong!!\n";
                sleep(2);
            }else {
                std::cout<<"Xoa that bai!!\n";
                sleep(2);
            }
        }
    }
    else sleep(2);
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
        redo.push(std::move(ds));
        /* C++11 trở lên cho phép dùng std::move để di chuyển
        đẩy ds vào redo mà không phải là copy */

        // Lấy ABCD ra ds trở lại trước -> ds = history.top()
        ds = std::move(history.top()); // -> Lúc này ds đang là ABCD
        /* Tương tự di chuyển lấy history top vào ds thay vì copy history*/

        // Nhưng trong stack vẫn còn lưu top trên cùng là ABCD
        // Nếu không xóa -> sau đó hoàn tác sẽ luôn là ABCD => Cần history.pop
        history.pop(); // => Lúc này ds là ABCD và stack xóa ABCD rồi nên còn ABC
        // => Lần sau hoàn tác tiếp thì sẽ lại đến vị trí tiếp theo
        saveToFile();
        std::cout<<"Undo thanh cong!!\n";
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
        history.push(std::move(ds)); 
        ds = std::move(redo.top()); // Lấy redo trên cùng ra
        redo.pop(); // Xóa cái trên cùng đi
        saveToFile(); // Lưu lại
        std::cout<<"Redo thanh cong!!\n";
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
        std::cout<<"Tao Demo thanh cong!!\n";
        sleep(2);
    }else {
        std::cout<<"Ban da huy thao tac, khong co gi xay ra!!\n";
        sleep(2);
    }
}

// Lưu ds vào file ngay tại folder chạy, tạo file ẩn
// 1. Bọc ẩn file
/*SetFileAttributesA là hàm của Windows API, nhận 2 tham số:

path.c_str() — path là std::string của C++, 
nhưng Windows API chỉ hiểu kiểu char* của C thuần.
.c_str() chuyển đổi string C++ sang dạng đó.
Chữ A ở cuối SetFileAttributesA nghĩa là dùng bảng mã ASCII
bản khác là SetFileAttributesW dùng Unicode.

FILE_ATTRIBUTE_HIDDEN — hằng số được định nghĩa sẵn trong windows.h, giá trị là 0x2
Nó ra lệnh cho Windows đặt thuộc tính ẩn cho file
giống như user click chuột phải → Properties → tick "Hidden".*/
void Project::hideFile(const std::string& path) {
    #ifdef _WIN32
        SetFileAttributesA(path.c_str(), FILE_ATTRIBUTE_HIDDEN);
    #endif
    // Mac/Linux: file bắt đầu bằng dấu . đã tự ẩn rồi, không cần làm gì thêm
    // Chỉ bọc lại để an toàn vì MAC không có SetFileAttributesA()
}
// Mở ẩn file trước khi ghi để có thể thao tác trên file.
void Project::UnhideFile(const std::string& path){
    #ifdef _WIN32
        SetFileAttributesA(path.c_str(), FILE_ATTRIBUTE_NORMAL);
    #endif
}
// 2. Bắt đầu lưu ds vào file. Có thể tự đặt tên
void Project::saveToFile(){
    // Tạo path lưu ẩn file:
    // dấu . đằng trước tên, trong MAC và Linux giúp ẩn file, nhưng ở Window chỉ là name.
    std::string path = ".data.chronos";
    UnhideFile(path);
    // Ghi vào file data = ofstream, f là tên path đến data.chronos
    std::ofstream f(path);
    // Khi f.is_open thì ghi từng dòng của ds vào f
    if (f.is_open()) {
        // Dùng const vì chỉ xem, kiểu auto.
        // Dùng &t tham chiếu duyệt từng dòng trong ds, không tạo bản sao.
        for (const auto &t: ds){
            // Lọc bỏ dòng data rác với deadline = -1 để an toàn
            if (!t.isValid()) continue;
            // Ghi lại theo dịnh dạng task_name | deadline vào f
            f<<t.getBeginDate()<<" | "<<t.getTaksName()<<" | "<<t.getDeadline()<<" | "<<t.getPriority()<<" | "<<t.GetBoolDone()<<" | "<<t.GetCD()<<"\n";
        }
        // Sau khi lưu xong thì đóng f an toàn
        f.close();
        // Tự ẩn file ngay sau khi lưu
        hideFile(path);
        std::cout<<"Da luu file thanh cong!!\n";
    }else std::cout<<"[ERROR]: Khong the mo file de ghi!!\n";
}

// 3. Load data từ f về lại RAM khi mở Chronos
void Project::loadFromFile(){
    // Dùng ifstream đọc file txt ở path
    std::string path = ".data.chronos";
    UnhideFile(path); // bỏ ẩn để đọc
    std::ifstream f(path);
    // Nếu f chưa tồn tại -> không có gì để load
    if (!f.is_open()) return;
    // Xóa ds ở RAM hiện tại để chuẩn bị lấy ds ở f vào
    ds.clear();
    std::string line; // Tạo biến line để đọc từng hàng của f
    // Khi trong f còn hàng để bỏ vào line
    while (std::getline(f, line)){
        /*try-catch: thử chạy đoạn code trong try.
        Nếu có lỗi bất kỳ (thiếu field, sai kiểu dữ liệu, v.v.)
        thì nhảy xuống catch, bỏ qua dòng đó và đọc dòng tiếp theo.
        Không crash toàn bộ chương trình.*/
        try {
            // Dùng sstream để cắt chuỗi
            std::stringstream ss(line); // Đưa từng line trong f vào ss
            // Tạo biến lấy dữ liệu
            std::string name, temp; // Đặt temp để lấy string dl, cd, isD chuyển về đúng dạng
            time_t bg = 0, dl = 0, cd = 0;
            int psc = 1;
            bool isD = false;
            // Đọc từng phần được cắt nhau bởi dấu | bằng getline
            // dùng std::ws để xóa khoảng trắng thừa

            // === FIELD BẮT BUỘC — nếu thiếu thì catch bắt, bỏ dòng này ===
            std::getline(ss >> std::ws, temp, '|'); // Lấy được begin date
            bg = std::stoll(temp); // Chuyển temp từ string thành ll lấy bg
            std::getline(ss >> std::ws, name, '|'); // Lấy được tên 
            // Dùng temp lấy dl
            std::getline(ss >> std::ws, temp, '|'); 
            // Chuyển về dl
            dl = std::stoll(temp); // Chuyển temp từ string thành ll lấy dl
            // Lọc bỏ line hỏng
            if (dl <= 0) continue;

            // === FIELD TÙY CHỌN — thiếu thì dùng giá trị mặc định ===
            // Mỗi field bọc try riêng để field sau không bị ảnh hưởng
            try{
                // Dùng temp lấy prio
                std::getline(ss >> std::ws, temp, '|');
                psc = std::stoi(temp);
            } catch(...){psc = 1;} // Có thì lấy ra, không có thì mặc định 1
            try{
                /*Đặt cứ != 0 thì là true, == 0 thì false.
                Nếu có lúc bị lưu thành 5/0 => 5 !=0 vẫn được tính thành true, không bị bỏ qua.*/
                std::getline(ss >> std::ws, temp, '|');
                isD = (std::stoi(temp)) != 0; // Chuyển str thành int lấy bool
            } catch(...) {isD = false;} // Không có thì lấy mặc định false
            try{
                std::getline(ss >> std::ws, temp);
                cd = std::stoll(temp); // Chuyển str thành ll lấy cd
            } catch(...) {cd = 0;} // Không có thì mặc định chưa có cd

            // Đặt Task t để đưa thành từng Task trong ds
            Task t;
            t.setDataChuan(bg, name, dl, psc, isD, cd); // Đưa name và dl vào task_name và dl
            ds.push_back(t); // đưa từng t vào ds vector trong RAM
        }
        catch(...){
            // Nếu thiếu infor bắt buộc: bg, name, dl thì bỏ qua hoàn toàn
            continue;
        }
    }
    // Load xong hết vào RAM thì đóng file an toàn
    f.close();
    // Đọc xong thì ẩn lại
    hideFile(path);
    // Sắp xếp lại
    priority();
}