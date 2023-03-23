# Trạm quan trắc - Meteorological observation station

Xây dựng một mô hình trạm quan trắc có thể đo được các thông số môi trường và theo dõi được thông qua internet.

*Building a meteorological observation station model that can measure environmental parameters and view them via the internet.*

## Các thông số đo và linh kiện sử dụng - Measurement parameters and components used

Hệ thống có thể đo được nhiệt độ, độ ẩm, cường độ tia UV và nồng độ bụi trong không khí.

*The system can measure temperature, humidity, UV intensity and dust concentration in the air.*

Các cảm biến được sử dụng:

*Sensors used:*

![image](https://user-images.githubusercontent.com/94043610/173487907-3da29dfc-f264-45b0-9764-e1c20872fe34.png)

##### Cảm biến DHT11 - Nhiệt độ và độ ẩm môi trường
##### DHT11 sensor - Temperature & humidity

![image](https://user-images.githubusercontent.com/94043610/173489273-121d2db7-6290-479e-84a7-f6d9466315b1.png)

##### Cảm biến ML8511 - Cường độ tia UV
##### ML8511 sensor - UV intensity

![image](https://user-images.githubusercontent.com/94043610/173488367-b10e3196-7c83-4c13-979c-6593d3d4d49c.png)

##### Cảm biến GP2Y1010AU0F - Nồng bụi trong không khi
##### GP2Y1010AU0F sensor - Dust concentration

Các linh kiện khác:

*Other components:*

* PIC16F877A
* LCD 20x4
* ESP8266 ESP-01S
* Điện trở, tụ điện, thạch anh... (Resistors, capacitors, quartz...)

## Sơ đồ mạch - Schematic diagram

Sơ đồ khối:

*Block diagram:*

![image](https://user-images.githubusercontent.com/94043610/173498388-e883a330-8eb6-4a1c-b6c9-718ceb0a47bb.png)

Sơ đồ mạch chi tiết

*Schematic diagram:*

![image](https://user-images.githubusercontent.com/94043610/173498469-9dacbda0-4565-450f-aac2-69d4b8172543.png)

Sử dụng file `proteus.pdsprj` để mô phỏng.

*Use the file `proteus.pdsprj` for simulation.*

## Phần mềm - Software

Lưu đồ:

*Flowchart:*

![image](https://user-images.githubusercontent.com/94043610/173502008-5baf36da-f287-4bdd-ab53-5fc088676a17.png)


Giao tiếp phần cứng:

*Hardware interface:*

* DHT11: 1-wire
* ML8511: analog
* GP2Y1010AU0F: analog
* LCD 20x4: 4-bit
* ESP8266 ESP-01S: UART (AT Command)

Xem thêm tại file `main.c`

*See more in flie `main.c`*

## Web và App - Web and App

Sử dụng [000webhostapp](https://vn.000webhost.com) để lưu trữ dữ liệu được gửi lên.

*Using [000webhostapp](https://vn.000webhost.com) to store submitted data.*

* Web:

Sử dụng HTML, CSS và JS

*Using HTML, CSS and JS*
* App:

Sử dụng Kođular

*Using Kodular*

## Kết quả - Results

Kết quả hiển thị trên LCD:

*Results are displayed on LCD:*

![image](https://user-images.githubusercontent.com/94043610/173503056-244d623d-8d8e-4be3-a1ad-3802246cfdc3.png)

Kết quả trên web:

*Results on website*

![image](https://user-images.githubusercontent.com/94043610/173503559-aff64f8a-4197-4632-864e-8d080a9d53cb.png)

Kết quả trên ứng dụng điện thoại:

*Results on mobile app*

![image](https://user-images.githubusercontent.com/94043610/173503758-7d4e9cec-9a46-4bfa-a29b-652fcefc3d7d.png)

