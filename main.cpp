// main.cpp
#include "project.h" // project đã gọi task.h
#include <iostream>

int main(){
    Project p; // Tạo danh sách p với class Project
    // Đưa ds từ file vào RAM ngay khi khởi tạo
    p.loadFromFile();
    int choose;
    while (true){
        // Dọn dẹp sạch lựa chọn trước để chuẩn bị cho lựa chọn tiếp theo.
        system("cls");
        std::cout<<"\n===CHRONOS MENU===\n\n";
        std::cout<<"1. Them\n2. Xem\n3. GiaHan\n4. Xoa\n5. Xoa trong khoang duoc chon\n6. Undo\n7. Demo\n0. Thoat\n";
        std::cout<<"Chon: ";
        std::cin>>choose;
        std::cin.ignore(); // xóa enter cho lần tiếp.
        if (choose == 0) {
            // Lưu file an toàn rồi exit
            p.saveToFile();
            std::string confirm;
            // Xác nhận lại trước khi thoát, nếu không phải y thì quay lại main menu
            std::cout<<"Bam Y de xac nhan thoat, hoac bam phim bat ky de tro lai Main Menu: ";
            std::cin>>confirm;
            if (confirm == "y" || confirm == "Y") break;
        }
        // 1. Thêm
        else if (choose == 1) p.add_task(); // Thêm task vào lớp project p
        // 2. Xem tất cả (chỉ xem)
        else if (choose == 2) {
            p.display();
            p.back_to_menu(); // Xác nhận trước khi về menu
        }
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
        // 5. Xóa trong khoảng được chọn
        else if (choose == 5){
            p.display(); // Xem danh sách để chọn xóa
            p.erase_from_x_to_y();
        }
        // 6. Hoàn tác
        else if (choose == 6) p.undo();
        // 7. Demo thử với 10000 task một lượt
        else if (choose == 7) p.demoOverflow();
        else std::cout<<"Vui long nhap so trong Menu!";
    }
    return 0;
}