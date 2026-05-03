// project.cpp
#include "project.h"
#include <iostream>

// Sắp xếp task thành priority
void Project::priority(){
    std::sort(ds.begin(), ds.end());
}

// 2. Hiển thị, chỉ xem.
void Project::display() const{
    for (int i = 0; i < (int)ds.size(); i++){
        std::cout<<i<<". ";
        ds[i].output();
    }
}

// 1. Thêm task
void Project::add_task(){
    history.push(ds); // Lưu trạng thái hiện tạo vào stack
    Task t; // gọi task được thêm vào là t
    t.input(); // Gọi hàm input để nhập tên task và deadline
    ds.push_back(t); // thêm toàn bộ t vừa nhập vào danh sách vector bằng push_back
    priority(); // sắp xếp lại danh sách theo độ ưu tiên
    display(); // Hiển thị lại
}

// 3. Gia hạn
void Project::gia_han(int vi_tri){
    history.push(ds); // Lưu trạng thái trước
    ++ds[vi_tri]; // gia hạn ở vị trí được chọn
    display(); 
}

// 4. Xóa
void Project::deleted(int vi_tri){
    history.push(ds); // Lưu trạng thái
    // chọn vị trí trong menu, xóa task ở vị trí được chọn.
    if (vi_tri >= 0 && vi_tri < (int)ds.size()) ds.erase(ds.begin() + vi_tri);
    display(); // Hiển thị
}

// 5. Hoàn tác
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
        display(); // Hiển thị danh sách sau khi undo
    }else std::cout<<"Không có gì để hoàn tác!!";
}