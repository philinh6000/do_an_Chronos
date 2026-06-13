// task.cpp
#include "task.h"
#include <algorithm>
#include <iomanip> // Để dùng setfill và setw thêm số, đảm bảo định dạng 2 số cho time.
// Xử lý mức độ ưu tiên
const std::string Task::Priority[] = {"0", "VERY LOW", "LOW", "MEDIUM", "HIGH", "CRITICAL"};
std::string Task::output_Priority() const{return Priority[priority_score];}

// Tính điểm khẩn cấp
double Task::urgency_score() const{
    // 1. Cập nhật thời gian hiện tại realtime
    time_t now = time(0);

    // 2. Nếu deadline < now -> quá hạn, uscore = 100
    if (deadline <= now) return 100.0;

    // 3. Tính thông số thời gian dựa trên thời gian gốc:
    // a. Lấy deadline - begin ra tổng thời gian khi tạo task
    double total_seconds = difftime(deadline, begin_time);
    // bọc an toàn:
    if (total_seconds <= 0) return 100.0;
    // b. Lấy now (realtime) - begin ra số giây đã trôi qua
    double second_elapse = difftime(now, begin_time);


    // 4. Tính pscore và sscore sao cho pscore + sscore = 100
    // a. pscore từ 12 đến tối đa 60 điểm -> sscore từ 88 -> 40 điểm
    double pscore = (float)priority_score/5.0 * 60; // VD: prio 1 -> pscore = 12
    double sscore = 100.0-pscore;

    // 5. Số điểm tăng mỗi giây (điểm cố định, tính trên time cố định)
    // Lấy điểm giây (88->40) chia cho tổng số giây = điểm/giây
    double second_rate = sscore/total_seconds;

    // 6. uscore = pscore + (thời gian đã qua * điểm mỗi giây) <- ra tổng số điểm trên thời gian đã qua
    return pscore + (second_elapse * second_rate);
}

// Xử lý ngày theo năm, tháng
// 1. Tạo mảng chứa ngày trong tháng
const int Task::DaysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
// 2. Xử lý năm nhuận: % 4 và 400 == 0, % 100 !=0
bool Task::is_leap(int y) const{return (y%4 == 0 && y%100 != 0 || y%400 == 0);}
// 3. Nếu là năm nhuận & tháng 2 thì return 29, còn không thì trả về ngày trong mảng
int Task::getDay(int m, int y) const{
    if (m == 2 && is_leap(y)) return 29;
    return DaysInMonth[m];
}

// Kiểm tra kết quả và nhập cout nhanh
int Task::correct_val(const std::string &text, const int min_val, const int max_val){
    // Nhập text bằng cout:
    std::cout<<text;
    // Chuẩn bị kiểm tra int và kiểm tra so với now
    int gia_tri; // tạo biến nhập time, số vị trí, số lựa chọn trong menu
    int i = 3; // Tạo số lần được nhập
    // Khi i lớn hơn 0 thì được nhập lại nếu sai
    while (i > 0){
        // Nếu gia_tri là số và gia_tri > now (biến now sẽ được điền theo từng field)
        if (std::cin>>gia_tri && gia_tri >= min_val && gia_tri <= max_val){
            std::cin.ignore(1000, '\n'); // Dọn tối đa 1000 ký tự rác cho đến khi gặp \n
            return gia_tri; // Đúng thì trả về con số.
        }
        // Sai thì dùng cin.clear() để reset trạng thái, sẵn sàng nhận lệnh mới
        std::cin.clear();
        // Xóa dòng cin lỗi trước đó, sẵn sàng chuẩn bị nhập lại
        std::cin.ignore(1000, '\n');
        i--; // Giảm số lần được nhập
        // Gửi thông báo số lần còn được nhập lại:
        if (i > 0) std::cout<<"Thong tin khong hop le!! Con "<<i<<" lan nhap!\n"
        <<text;
        else std::cout<<"Thong tin khong hop le!! Yeu cau that bai!!\n";
    }
    // Sau vòng lặp vẫn sai, thì trả về giá trị -1 để dánh dấu xử lý trong deadline
    return -1;
}
// Nhập Priority
void Task::input_priority(){
    std::cout<<"Chon muc do uu tien:\n1. VERY LOW\n2. LOW\n3. MEDIUM\n4. HIGH\n5. CRITICAL\n";
    priority_score = correct_val("Choose: ",1, 5);
}

// Nhập tên task name
void Task::input_task_name(){
    std::cout<<"Nhap ten task bang chu khong dau: ";
    std::getline(std::cin, task_name);
}

// Nhập deadline
void Task::input_time(){
    int d, m, y, hr, min;
    // Lấy thời gian hiện tại chuẩn bị so sánh từng phần tử của deadline với now.
    begin_time = time(0); // Đánh dấu thời gian tạo task
    time_t now = time(0);
    struct tm* thoigian = localtime(&now);
    int nam_hien_tai = thoigian->tm_year+1900; // cắt lấy năm biến thành số để so sánh với y.
    int thang_hien_tai = thoigian->tm_mon+1; // cắt lấy tháng hiện tại thành số
    int ngay_hien_tai = thoigian->tm_mday;
    int gio_hien_tai = thoigian->tm_hour;
    int phut_hien_tai = thoigian->tm_min;

    // Nhập time
    // Nhập năm:
    y = correct_val("Nhap nam (bat buoc nhap 4 so): ", nam_hien_tai, nam_hien_tai+100); 
    /*Nếu không giới hạn năm: từ 0 giờ 19/1/3001 có thể nhập nhưng không lưu
    Lý do: Thư viên MingW/MSVC ở phiên bản cũ có cấu hình mặc định chỉ hỗ trợ mktime
    đến ngày 31/12/3000 theo giáo trình và hết ngày 18/1/3001 theo thực tế.
    Từ 0 giờ ngày 19/1/3001 không được hàm _mktime hỗ trợ, nên sẽ return -1 
    -> deadline = -1 sau khi nhập hết -> theo hàm isValid()
    trong add_task làm cho deadline = -1 thì bỏ qua 
    -> trở về Main Menu mà không hiện thông báo gì cả.*/
  
    // Nếu năm là -1 = sai -> về menu luôn
    if (y == -1) {deadline = -1; return;}

    // Nếu đúng thì rơi xuống nhập tháng
    // Nếu năm > now thì tháng mấy cũng được. đặt 1 vì chắc chắn không có tháng nhỏ hơn 1
    if (y > nam_hien_tai) m = correct_val("Nhap thang: ", 1, 12);
    // Nếu năm = now thì tháng phải lớn hơn tháng hiện tại.
    else m = correct_val("Nhap thang: ", thang_hien_tai, 12);

    if (m == -1) {deadline = -1; return;} // Sai thì thoát ra menu

    // Tương tự với ngày, giờ, phút. Giả sử năm 2027 thì tháng 1 cũng > now.
    if (y > nam_hien_tai || m > thang_hien_tai) d = correct_val("Nhap ngay: ", 1, getDay(m, y));
    else d = correct_val("Nhap ngay: ", ngay_hien_tai, getDay(m, y));
    if (d == -1) {deadline = -1; return;}

    // Chọn giờ mặc định hoặc nhập giờ phút: Nếu >= 24 giờ thì tự động lấy giờ mặc định
    if (y > nam_hien_tai || m > thang_hien_tai || d > ngay_hien_tai) hr = correct_val("Nhap gio (Nhap 24 de lay gio mac dinh 23:59): ", 0, 24);
    else hr = correct_val("Nhap gio (Nhap 24 de lay gio mac dinh 23:59): ", gio_hien_tai, 24);

    if (hr == -1) {deadline = -1; return;} // Nếu giờ nhập ra sai thì return
    // Nếu ra đúng mà số giờ >= 24 thì lấy thời gian mặc định, bỏ qua nhập phút.
    if (hr >= 24){
        hr = 23;
        min = 59;
    }
    // Ngược lại thì hr vẫn giữ nguyên và tiến đến nhập phút
    else{
        if (y > nam_hien_tai || m > thang_hien_tai || d > ngay_hien_tai || hr > gio_hien_tai) min = correct_val("Nhap phut: ", 0, 59);
        else min = correct_val("Nhap phut: ", phut_hien_tai, 59);
        if (min == -1) {deadline = -1; return;}}
    // Nếu đúng hết thì xử lý: Chuyển đổi thời gian từ số nguyên thành giờ
    struct tm t = {0};
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    t.tm_hour = hr;
    t.tm_min = min;
    
    deadline = mktime(&t); // Lấy được thời gian deadline
}

void Task::input_flag_done(){
    is_done = false; // Khi task vừa nhập, done luôn = false
    completed_date = 0; // chưa có thời gian hoàn thành
}

// Nhập tên task và thời gian deadline
void Task::input(){
    input_task_name();
    input_time();
    if (deadline != -1){
        input_priority();
        input_flag_done();
    }
}
// Đánh dấu hoàn thành
void Task::markAsDone(){
    is_done = true;
    completed_date = time(0); // Lúc đánh dấu xong là hiện tại,
}

// Đặt điều kiện so sánh nếu deadline < hơn.
bool Task::operator<(const Task& other) const{
    if (this->deadline == other.deadline) return this->urgency_score() > other.urgency_score();
    return this->deadline < other.deadline; // Ngày nhỏ hơn thì ưu tiên hơn
}

Task& Task:: operator++(){
    deadline +=86400; // Khi gọi operator++ thì deadline tự tăng 1 ngày
    return *this; // Trả về nguyên object Task thay vì chỉ có con số daedline
}

/*Ví dụ:
Task t;
++(++t); // tăng 2 lần liên tiếp — được vì return *this
Nếu return deadline thì dòng trên bị lỗi vì lần ++ thứ 2 không có object để gọi nữa.*/

/*Cách gọi trong menu:
if (choose == 5) {
    ++ds[viTri]; // gọi operator++ của task tại vị trí viTri
}*/

/*Vì sao không dùng if (choose) deadline +=86400 luôn?

// Cách thủ công — phải nhớ deadline là gì, viết lại mỗi lần:
if (choose == 5) ds[i].deadline += 86400; // lỗi: deadline là private!

// Cách dùng operator++ — gọn, an toàn, deadline vẫn private:
if (choose == 5) ++ds[i];*/

void Task::output() const{
    // Bọc an toàn:
    if (!isValid()) return;
    // Nếu deadline < now rồi thì báo quá hạn
    time_t now = time(0); // Lấy thời gian hiện tại
    if (deadline < now) std::cout<<"[OVERDUE] ";
    // Chuyển deadline thành dạng chuẩn dd/mm/yyyy hr:min
    struct tm* thoigian = localtime(&deadline);
    int nam_deadline = (thoigian->tm_year+1900); // Tương tự, ngược lại lúc chuyển qua giây
    int thang_deadline = (thoigian->tm_mon+1); // bỏ trong ngoặc để thực hiện +1, trả lại số nguyên đúng (trong C++, tháng bắt đầu từ 0)
    int ngay_deadline = thoigian->tm_mday; // dùng -> để lấy tương tự this, đưa thời gian thực tế để chuyển đổi
    int gio_deadline = thoigian->tm_hour;
    int phut_deadline = thoigian->tm_min;

    std::cout<<"TASK NAME: "<<task_name<<" | DEADLINE: "
    <<std::setfill('0')<<std::setw(2)<<ngay_deadline<<"/" 
    <<std::setfill('0')<<std::setw(2)<<thang_deadline<<"/" 
    <<nam_deadline<<" " 
    <<std::setfill('0')<<std::setw(2)<<gio_deadline<<":"
    <<std::setfill('0')<<std::setw(2)<<phut_deadline<<" | PRIORITY: "
    <<output_Priority()<<" | PRIORITY SCORE: "
    <<std::fixed<<std::setprecision(1)<<urgency_score()<<"\n";
}

void Task::outputDone() const{
    // Bọc an toàn:
    if (!isValid()) return;
    // Nếu deadline < now rồi thì báo quá hạn
    time_t now = time(0); // Lấy thời gian hiện tại
    if (deadline < now) std::cout<<"[OVERDUE] ";
    // Chuyển deadline thành dạng chuẩn dd/mm/yyyy hr:min
    struct tm thoigian = *localtime(&deadline);
    int nam_deadline = (thoigian.tm_year+1900); // Tương tự, ngược lại lúc chuyển qua giây
    int thang_deadline = (thoigian.tm_mon+1); // bỏ trong ngoặc để thực hiện +1, trả lại số nguyên đúng (trong C++, tháng bắt đầu từ 0)
    int ngay_deadline = thoigian.tm_mday;
    int gio_deadline = thoigian.tm_hour;
    int phut_deadline = thoigian.tm_min;
    // Bọc an toàn với cd = 0
    if (completed_date <= 0){
        std::cout<<" - COMPLETED DATE: N/A\n";
        return;
    }
    // Chuyển completed date thành dạng chuẩn
    struct tm cd = *localtime(&completed_date);
    int nam_cd = (cd.tm_year+1900);
    int thang_cd = (cd.tm_mon+1);
    int ngay_cd = cd.tm_mday;
    int gio_cd = cd.tm_hour;
    int phut_cd = cd.tm_min;
    /*Dấu * ở kiểu dữ liệu và * ở localtime:
    1. Dấu * đặt ở kiểu dữ liệu (struct tm* thoigian) — Con trỏ (Pointer):
    Nghĩa là biến thoigian không phải là một cái thùng chứa ngày tháng,
    mà nó chỉ là một con trỏ hướng tới một cái thùng localtime trong bộ nhớ của máy tính.
    Vì nó là "con trỏ", nên trong C++, với struc bắt buộc phải dùng toán tử ->
    Khi đó, thoigian và cd cùng trỏ tới 1 hộp localtime -> dễ bị ghi đè.

    2. Dấu * đặt ở vế phải (*localtime(&deadline)) — Giải tham chiếu (Dereference)
    Hàm localtime trả về một "địa chỉ" của time_t, nên bắt buộc trong localtime dùng &
    nó không nhận giá trị trực tiếp.
    Nhưng khi thêm dấu * ở ngay trước nó
    -> * giải tham chiếu con trỏ mà localtime trả về
    <=> localtime trả về địa chỉ ->, * đi theo địa chỉ -> lấy nội dung ra.
    -> gán nó vào struct tm thoigian (không có dấu * ở kiểu dữ liệu)
    Lúc này, thoigian đã trở thành một "Cái thùng thực sự" độc lập, lưu bản sao dữ liệu an toàn 
    trong vùng nhớ của riêng nó, không sợ bị ai ghi đè nữa.
    Vì nó đã là một "Cái thùng thực sự" (biến) chứ không phải địa chỉ nữa, 
    ta phải dùng Dấu chấm (.) để mở thùng lấy đồ: thoigian.tm_year
    VD như: task t; t.add_task() <- đối tượng gọi hàm để dùng thì dùng (.).*/
    // In ra
    std::cout<<"TASK NAME: "<<task_name<<" | DEADLINE: "
    <<std::setfill('0')<<std::setw(2)<<ngay_deadline<<"/" 
    <<std::setfill('0')<<std::setw(2)<<thang_deadline<<"/" 
    <<nam_deadline<<" " 
    <<std::setfill('0')<<std::setw(2)<<gio_deadline<<":"
    <<std::setfill('0')<<std::setw(2)<<phut_deadline<<" | PRIORITY: "
    <<output_Priority()<<" | PRIORITY SCORE: "
    <<std::fixed<<std::setprecision(1)<<urgency_score()<<" - COMPLETED DATE: "
    <<std::setfill('0')<<std::setw(2)<<ngay_cd<<"/" 
    <<std::setfill('0')<<std::setw(2)<<thang_cd<<"/" 
    <<nam_cd<<" " 
    <<std::setfill('0')<<std::setw(2)<<gio_cd<<":"
    <<std::setfill('0')<<std::setw(2)<<phut_cd<<"\n";
}

void Task::setFakeData(int i){
    begin_time = time(0);
    task_name = "Task thu " + std::to_string(i); // task name = số thứ tự
    deadline = time(0) + (i*3600); // deadline lấy hiện tại + 1hr mỗi lần tạo
    priority_score = 1;
    is_done = false;
    completed_date = 0;
}

// Lấy task name và dl dùng nhanh
void Task::setDataChuan(const time_t bg, const std::string &name, const time_t dl, const int psc, const bool isD, const time_t cd){
    begin_time = bg; // Lấy thông tin begin date
    task_name = name; // Lấy task_name = thông tin string mới
    deadline = dl; // Lấy deadline = thông tin time mới
    priority_score = psc; // Lấy thông tin priority score
    is_done = isD; // Lấy is_done = thông tin bool
    completed_date = cd; // Lấy ngày hoàn thành = time
}