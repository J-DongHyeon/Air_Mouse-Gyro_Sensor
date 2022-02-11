## 공중에서 손목 움직임만으로 마우스를 제어하는 Air Mouse 제작 (자이로 센서 MPU 6050 이용)

#### 1. 손목에 자이로 센서를 부착
#### 2. 자이로 센서를 이용하여 손목의 움직임 각속도를 측정, 계산
#### 3. 각속도 데이터를 이용하여 마우스 커서 움직임을 제어
-----------------

### 아두이노 우노 보드를 이용하여 일단 마우스 움직임만 제어하고자 함
#### 1. HoodLoader2 라이브러리를 USB MCU에 올림으로써 CDC bootloader와 Fast USB-Serial Bridge 간 스위치가 가능하도록 함
#### 2. I/O MCU 업로드 : 자이로 센서로부터 손목 움직임 각속도 데이터 추출, 가공 후 USB MCU에 시리얼 통신 전송
#### (I/O MCU로는 USB HID 장치로 인식이 되지 않기 때문에 마우스 제어 불가)
#### 3. USB MCU 업로드 : 손목 움직임 각속도 데이터를 시리얼 통신 수신 후 이를 이용하여 마우스 제어

![ezgif com-gif-maker (12)](https://user-images.githubusercontent.com/86474141/148759700-b6fd94bd-7db5-47ec-9e0b-c75fb3f35f6d.gif)


#### 4. 한계점 : USB MCU는 I/O MCU와의 시리얼 통신만 가능할 뿐, I/O핀들을 사용할 수가 없다. 따라서 마우스 클릭 이벤트에 대한 처리를 마땅히 할 수가 없다.

----------------------
### 레오나르도 보드를 이용하여 제작

### 회로도
<img width="525" alt="2022-02-11 23;42;17" src="https://user-images.githubusercontent.com/86474141/153612426-43d47189-f7e1-48b8-87ae-2da279a00383.PNG">
### 연결 사진
![KakaoTalk_20220212_001229311](https://user-images.githubusercontent.com/86474141/153617876-fea8b67a-3135-47e5-a876-fd101892e445.jpg)

### 추가 사항
#### 1. 더블 클릭 기능의 버튼을 추가하였다.
> 공중에서 클릭을 하기 때문에 클릭할 때 마다 마우스에 미세한 흔들림이 있다. 따라서 좌클릭을 빠르게 두 번 하는 것이 힘드므로 더블 클릭 버튼을 추가로 만들었다.
#### 2. Potentiometer를 추가함으로써 마우스의 감도 조절을 가능하도록 하였다.

![KakaoTalk_20220212_001229311](https://user-images.githubusercontent.com/86474141/153618054-f8f99daf-39ea-4c08-ad4b-5e280cdb3106.jpg)
