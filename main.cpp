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
        #ifdef _WIN32
            system("cls");
        #else
            system("clear"); // MAC thì dùng clear
        #endif
        p.display(); // Hiển thị lại tl để chọn Menu.
        std::cout<<"\n===CHRONOS MENU===\n\n";
        std::cout<<"1. Them\n2. Xem Completed Task\n3. Cap nhat\n4. Xoa trong khoang duoc chon\n5. Undo\n6. Redo\n7. Demo\n8. Refresh\n0. Thoat\n";
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
        // 2. Xem task đã hoàn thành (chỉ xem)
        else if (choose == "2") {
            p.display_done();
            p.back_to_menu("Bam 0 de tro lai Main Menu!\n");
        }

        // Sua task
        else if (choose == "3") {
            system("cls");
            p.update_task();
        }
        // 5. Xóa trong khoảng được chọn
        else if (choose == "4")p.erase_from_x_to_y();
        // 6. Hoàn tác
        else if (choose == "5") p.undo();
        // 7. Redo
        else if (choose == "6") p.Redo();
        // 8. Demo thử với 10000 task một lượt
        else if (choose == "7") p.demoOverflow();
        else if (choose == "8") p.sleep(1);
        else {
            std::cout<<"Vui long nhap so trong Menu!";
            p.sleep(2);
        }
    }
    return 0;
}