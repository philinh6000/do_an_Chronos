### rp_log.md
# 5/5/26 - 9/5/26:
1. Thêm lưu file vào ổ đĩa
2. Thêm tính năng xóa nhanh từ x -> y
3. Thêm cảnh báo OVERDUE với task quá hạn deadline
4. Thêm tính năng clear CLI mỗi lần quay về Main Menu
5. Thêm xác nhận trước khi quay về Main Menu
6. Sửa lỗi undo
7. Thêm Redo (đảm bào undo - redo đúng số bước với nhau)
8. Thêm xác nhận trước khi: xóa từ x đến y, demo, exit

Follow-up:
1. Có nên thêm thời gian hiện tại ngay đầu TASKLIST để tiện xem lịch?
2. Có nên giữ tính năng 4, 5 không hay bỏ luôn để thuận tiện, không cần mỗi lần đều bị hỏi mới về được Main Menu? - Giữ, nhưng cải tiến thành chọn 0 -> main menu, chọn 3 -> sửa task, else -> chọn lại.

---
Testcase & Bug:
1. Nhập ngay/thang/nam vào d, m, y -> freeze 
-> fix: báo lỗi nhập không đúng định dạng và quay lại main menu. - DONE 5/10/26
2. Nếu trong choose ở Main Menu nhập string -> lỗi infinite loop -> vì choose là int 
-> fix: đổi choose thành string - DONE 5/9/26
3. Tự cộng dồn thời gian khi nhập lớn hơn thời gian thực tế. VD: nhập tháng 13 -> đổi thành tháng 1 năm sau -> NOT A BUG - tính năng.

Follow-up:
1. Khi user choose 1 tính năng nào đó -> muốn quay lại bước trước: Bấm Esc hoặc bấm 0 - PENDING
2. Đổi thứ tự: Nhập năm -> tháng -> ngày -> giờ -> phút. Mỗi bước nhập đều so sánh với now. DONE - 5/10/26
Mỗi field sai (< now hoặc nhập string) có i = 3 lần nhập lại (while i--), mỗi lần sai sẽ đếm lùi:"Thời gian không hợp lệ, vui lòng nhập lại, còn i lần nhập". DONE - 5/10/26
Sau 3 lần vẫn nhập sai -> return ra Main Menu luôn với thông báo:"Thời gian không hợp lệ, không thể khởi tạo!". - DONE 5/10/26
Nếu nhập đúng int và là số > now -> thực thi tính năng cộng dồn thời gian. DONE - 5/10/26
Đối với giờ phút:
- if (hr == "A") hr = 23, min = 59; else if nhập sai -> while i--, nhập đúng thì cho qua. - DONE 5/10/26
3. Thêm tính năng:'Sửa Task'
-> Cách thực thi: Trong sửa -> Chọn vị trí task (chọn task) -> Chọn vị trí field (chọn field) -> std::cin>>new_infor -> task_name/deadline = new_infor. - DONE - 5/10/26
4. Chọn nhiều task 1 lượt -> Chọn nhiều field cần sửa một lượt -> lần lượt nhập thông tin mới từng field - PENDING

** Tính năng 'Sửa' và lựa chọn sau mỗi tính năng:
- Trong menu: choose == 3 để sửa
- Ở mỗi lần kết thúc tính năng (ví dụ sau khi thêm, xem, sửa, xóa, gia hạn) cho chọn:
1. Bấm 0 để quay lại Main Menu - DONE - 5/10/26
2. Bấm 3 để sửa -> giúp việc sửa có thể chọn lại nhanh ngay sau mỗi lần sửa -> gần tương đương sửa nhiều field liên tục, thay vì phải quay ra Main Menu chọn sửa. - SKIP
3. Else: báo lỗi: bắt buộc bấm 0 hoặc 3. - SKIP

---
# 9/5 - 14/5
## 9/5
1. fix bug freeze khi nhập string ở Main Menu.
## 10/5
2. fix bug freeze khi nhập string ở time.
3. Thêm tính năng so sánh ở từng field ngày, tháng, năm, giờ, phút với now.
4. Thêm tính năng nhập tối đa 3 lần khi sai (nhập string hoặc thời gian < now)
5. Thêm tính năng tự động lấy giờ mặc định nếu nhập giờ >= 24.
6. Tách input ra thành 2 hàm con input_task_name và input_deadline, gọi lại trong input
7. Sửa giahan và deleted để check int vi_tri trong hàm.
8. Thêm tính năng sửa task.
9. Thêm sleep(s), change back_to_menu chỉ về menu khi bấm 0, dùng sleep thay cho back_to_menu ở các function trừ display.
10. sửa erase_from_x_to_y: thêm điều kiện kiểm tra x, y.
## 12/5
11. fix bug không lưu khi nhập thời gian quá lớn: Tạo giới hạn thời gian tối đa
12. Thêm tính năng 3 lần nhập lại nếu nhập sai vị trí cho: sửa, gia hạn, xóa task
## 14/5
13. Sửa: hiển thị TL ngay lúc chọn Menu, bỏ tính năng bấm xem TL.
14. Thêm tính năng Task Completed trong Update Task.
15. Thêm xem lại Task Completed
16. Bổ sung lưu CT và CD vào file.
17. Sửa: đưa gia hạn task, xóa task vào update_task -> cùng là thao tác cập nhật trên 1 task.
18. Sửa tùy chọn trên menu: xóa lựa chọn gia hạn, xóa trong main menu -> đời vào Update Task
## 14/5
19. Fix: Tạo giới hạn đúng chuẩn cho ngày, tháng, năm để không tự cộng dồn sai ngày
20. Fix: Thêm phím 0 Exit trong Update Task, chọn task sai 3 lần sẽ quay về main menu, chọn update menu sai 3 lần để quay về main menu hoặc bấm 0 để lập tức về main menu
21. Dời TL lên trên Main Menu

---
# 15/5-17/5
## 16/5
1. Thêm mức độ ưu tiên: private -> user chọn -> if 1: VERY LOW - 5: CRITICAL
2. Thêm điểm khẩn cấp: Tổng 100 điểm
pscore: cố định từ 12 - 60 (tối đa 60%) double pscore = priority_score/5.0 * 60 -> tính 1 lần khi tạo task
%hrscore = 100 - pscore. VD: pscore = 60 -> %hrscore = 40; pscore = 12 -> %hrscore = 88 -> luôn đủ 100 điểm dù p bao nhiêu -> tính 1 lần khi tạo task
hrscore = %hrscore/hrsleft -> Số điểm phải tăng mỗi giờ -> tính 1 lần khi tạo task
total_hrs = deadline - begin -> tổng số giờ -> tính 1 lần khi tạo task
hrsleft_realtime = deadline - now -> số giờ còn lại -> tính mỗi lần gọi display trong hàm tính uscore
hrsup = total_hrs - hrsleft_realtime -> số giờ đã trôi qua -> tính mỗi lần gọi display trong hàm tính uscore
uscore = pscore + hrsup*hrscore -> tính mỗi lần gọi display trong hàm tính uscore

```python
"""Ví dụ:
T1: pscore = 12 - total_hrs = 40
T2: pscore = 60 - total_hrs = 40
T3: pscore = 60 - total_hrs = 20

Lúc này, các giá trị tính ra của 3 task sẽ là:

T1: %hrscore = 88 - hrscore 88/40 = 2.2
T2: %hrs = 40 - hrs = 1
T3: %hrs = 40 - hrs = 40/20 = 2

Lúc này tính uscore cho các task:
Giả sử trôi qua 1hr
T1: hrsleft = 39 - hrsup = 1 → usc = 12 + 1*2.2 = 14.2
T2: usc = 60 + 1*1 = 61
T3: usc = 60 + 1*2 = 62

Khi đạt tới mốc 19 điểm (gần hết giờ của task 3)
T1: usc = 12 + 19*2.2 = 53.8
T2: usc = 60 + 19*1 = 79
T3: usc = 60 + 19*2 = 98 → đúng, vì tới deadline rồi

Tiếp theo là mốc khi 2 task trên tới 39hr (sát hạn deadline)
T1: usc = 12 + 39*2.2 = 97.8
T2: usc = 60 + 39*1 = 99 → đúng, prio cao hơn thì điểm cao hơn dù cùng sắp hết giờ.

Khi hrsleft còn 0 → tất cả chắc chắn đạt 100."""
```