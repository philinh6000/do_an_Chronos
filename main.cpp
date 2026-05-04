#include "project.h" // project đã gọi task.h
#include <iostream>

int main(){
    Project p; // Tạo danh sách p với class Project
    int choose;
    while (true){
        std::cout<<"\nCHRONOS MENU\n";
        std::cout<<"1. Them\n2. Xem\n3. GiaHan\n4. Xoa\n5. Undo\n6. Demo\n0. Thoat\n";
        std::cout<<"Chon: ";
        std::cin>>choose;
        std::cin.ignore(); // xóa khoảng trắng cho lần tiếp.
        if (choose == 0) break; // choose = 0 thì exit luôn
        // 1. Thêm
        else if (choose == 1) p.add_task(); // Thêm task vào lớp project p
        // 2. Xem tất cả (chỉ xem)
        else if (choose == 2) p.display();
        // 3. Gia hạn
        else if (choose == 3) {
            p.display(); // Hiển thị xem danh sách để chọn vị trí cần gia hạn
            int i;
            std::cout<<"Chon vi tri can gia han: ";
            std::cin>>i;
            p.gia_han(i);
        }
        // 4. Xóa
        else if (choose == 4){
            p.display(); // Xem danh sách vị trí cần xóa
            int i; // Vị trí muốn xóa
            std::cout<<"Chon vi tri can xoa: ";
            std::cin>>i;
            p.deleted(i);
        }
        // 5. Hoàn tác
        else if (choose == 5) p.undo();
        // 6. Demo thử với 10000 task một lượt
        else if (choose == 6) p.demoOverflow();
    }
    return 0;
}