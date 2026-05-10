// main.cpp
#include "project.h" // project đã gọi task.h
#include <iostream>

int main(){
    Project p; // Tạo danh sách p với class Project
    // Đưa ds từ file vào RAM ngay khi khởi tạo
    p.loadFromFile();
    std::string choose;
    while (true){
        // Dọn dẹp sạch lựa chọn trước để chuẩn bị cho lựa chọn tiếp theo.
        system("cls");
        std::cout<<"\n===CHRONOS MENU===\n\n";
        std::cout<<"1. Them\n2. Xem\n3. Sua\n4. Gia Han\n5. Xoa\n6. Xoa trong khoang duoc chon\n7. Undo\n8. Redo\n9. Demo\n0. Thoat\n";
        std::cout<<"Chon: ";
        std::cin>>choose;
        std::cin.ignore(); // xóa enter cho lần tiếp.
        if (choose == "0") {
            // Lưu file an toàn rồi exit
            p.saveToFile();
            std::string confirm;
            // Xác nhận lại trước khi thoát, nếu không phải y thì quay lại main menu
            do{
                std::cout<<"Bam Y de xac nhan thoat, hoac N de tro lai Main Menu: ";
                std::cin>>confirm;
            }
            while(confirm != "y" && confirm != "Y" && confirm != "n" && confirm != "N");
            if (confirm == "y" || confirm == "Y") break;
        }
        // 1. Thêm
        else if (choose == "1") p.add_task(); // Thêm task vào lớp project p
        // 2. Xem tất cả (chỉ xem)
        else if (choose == "2") {
            p.display();
            p.back_to_menu("Bam 0 de tro lai Main Menu!\n");
        }
        // Sua task
        else if (choose == "3") {
            p.sua_task();
        }
        // 3. Gia hạn
        else if (choose == "4") {
            p.gia_han();
        }
        // 4. Xóa
        else if (choose == "5"){
            p.deleted();
        }
        // 5. Xóa trong khoảng được chọn
        else if (choose == "6"){
            p.display(); // Xem danh sách để chọn xóa
            p.erase_from_x_to_y();
        }
        // 6. Hoàn tác
        else if (choose == "7") p.undo();
        // 7. Redo
        else if (choose == "8") p.Redo();
        // 8. Demo thử với 10000 task một lượt
        else if (choose == "9") p.demoOverflow();
        else {
            std::cout<<"Vui long nhap so trong Menu!";
            p.sleep(2);
        }
    }
    return 0;
}