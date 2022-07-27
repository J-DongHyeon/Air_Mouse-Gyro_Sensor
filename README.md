
<h1 align="center"> Air Mouse </h1>

<h4 align="center"> 공중에서 손목 움직임만으로 마우스를 제어하는 Air Mouse 제작 </h4>
<br>

<p align= "center">
<img src=/docs/readme_Imgs/readme_img1.PNG width=550 height=350></p> 
</br></br>

------------------------------------------

# 1. 기획 계기

<p align= "center">
<img src=/docs/readme_Imgs/readme_img2.PNG width=550 height=300></p> 

> &nbsp;`Arduino UNO 보드` 를 이용하여 여러 센서들을 테스트 해보던 중, `MPU 6050 센서` 를 연결하여 테스트 할 때 흥미로웠다. 나의 움직임을 측정하여 3축 각속도, 가속도 값을 출력해내고, 이를 조금 가공하여 각도까지 계산해 내는 과정이 신기하였다. 이를 실생활에 적용 시켜서 시간을 들여 하나의 프로젝트를 만들면 재미있겠다는 생각이 들었다. <br><br>
> &nbsp;MPU 6050 센서 사용법을 익힐 겸, 재미있는 프로젝트를 진행할 겸 `Air Mouse` 를 만들기 위한 구상을 하였고, 이를 구현하기 위해 본 프로젝트를 진행하였다.
<br><br>

------------------------------------------

# 2. 프로젝트 개요

- ## 시나리오 1.
  - 첫 번째 방법으로, `아두이노 UNO 보드` 를 이용하여 컴퓨터의 마우스 제어를 하고자 하였다.
    - 문제점 1.
    > &nbsp; 마우스 입력장치 처리와 관련된 헤더파일 `Mouse.h` 를 UNO 보드에 업로드 하려니 되지 않았다. 그 이유를 찾아보니, UNO 보드에 파일이 업로드 되는 `ATMega 328 MCU` 는 컴퓨터와의 USB 통신이 직접적으로 되지 않기 때문에 HID 입력 장치로써 인식이 되지 않는다는 것이 문제 였다.<br>
    > &nbsp; 반면, UNO 보드의 또 다른 MCU인 `ATMega 16u2 MCU` 는 컴퓨터와 ATMega 328 간의 `USB to Serial` 변환 과정을 담당하기 때문에 컴퓨터와의 USB 통신이 직접적으로 이루어진다. (MCU 이름에 u가 들어간 MCU는 컴퓨터와의 USB 통신이 가능하다는 것이다.)<br>
    > 즉, ATMega 16u2 MCU는 HID 입력 장치로써 인식이 된다는 것이다. 하지만, 기본적으로 ATMega 16u2 에는 파일을 업로드 할 수 없는 상태이기 때문에 마우스 입력장치 처리가 불가능 하다.
   
    - 첫 번째 문제 해결
    > &nbsp; 결론적으로, UNO 보드로 마우스 입력장치 처리를 하려면 ATMega 16u2 에 파일을 업로드 할 수 있어야 한다. ATMega 16u2 를 프로그래밍 가능한 상태로 만들기 위해서 `HoodLoader2 부트로더` 를 이용하였다. HoodLoader2 는 `CDC 부트로더` 와 `고속 USB 시리얼 브릿지` 로 구성된다. CDC 부트로더는 ATMega 16u2 MCU에도 프로그래밍이 가능하도록 하는 기능을 하고, 고속 USB 시리얼 브릿지는 컴퓨터와 ATMega 328 간의 USB to Serial 통신을 담당한다. <br>
    > &nbsp; 쉽게 말하면, ATMega 16u2 의 기본 기능인 USB to Serial 기능은 그대로 존재하고, 추가적으로 ATMega 16u2 에도 프로그래밍이 가능하게 됐다는 것이다. <br>
    > &nbsp; ATMega 16u2 의 기존 부트로더를 HoodLoader2 부트로더로 변경하는 과정은 `https://luftaquila.io/blog/diy/arduino-hid-control-1/` 를 참고하였다.

  첫 번째 문제를 해결하고 ATMega 16u2 에도 프로그래밍이 가능하게 됨으로써, UNO 보드로 컴퓨터 마우스 움직임 처리가 가능해졌다. <br>
    > 1. 공중에서 손으로 MPU 6050 센서를 잡고 움직인다. <br>
    > 2. ATMega 328 은 MPU 6050 으로부터 손목의 X축, Z축 (상하, 좌우) 각속도 데이터를 받아 가공하고, 이 값을 하드웨어 시리얼로 ATMega 16u2 로 전송한다. <br>
    > 3. ATMega 16u2 는 하드웨어 시리얼로 받은 각속도 데이터에 비례하여 마우스 움직임을 제어하도록 하였다.
  
  - ### UNO 보드로 컴퓨터 마우스 움직임을 제어하는 시연 영상

<p align= "center">
<img src=https://user-images.githubusercontent.com/86474141/148759700-b6fd94bd-7db5-47ec-9e0b-c75fb3f35f6d.gif width=400 height=250></p> 

-
  -
    - 문제점 2.
    > &nbsp; ATMega 16u2 는 UNO 보드의 I/O 핀들을 직접 제어할 수가 없다. 단지, ATMega 328 이 I/O 핀들에 대해 처리한 데이터만 하드웨어 시리얼으로 받을 수 있다. 게다가, ATMega 16u2 와 ATMega 328 간 연결된 하드웨어 시리얼은 1쌍 뿐이므로 굉장히 제한적이다. <br>
    > &nbsp; 위에서, UNO 보드로 마우스 움직임을 제어한 것을 보면 ATMega 16u2 와 ATMega 328 간의 하드웨어 시리얼은 각속도 데이터를 전송하는 용도로 사용하고 있다. 만약, 마우스 클릭을 구현하기 위해 버튼을 I/O 핀에 연결한다 했을 때, 이 버튼 신호를 ATMega 16u2 에 전송할 방법이 제한적이다. <br>
    > &nbsp; 즉, UNO 보드로 마우스 움직임 제어는 가능하겠지만, 마우스 클릭에 대한 처리는 힘들다고 볼 수 있다. 
    
    - 두 번째 문제 해결
    > &nbsp; UNO 보드로는 마우스 입력장치 제어를 완전히 할 수 없다는 판단이 들었다. <br>
    > &nbsp; 다른 MCU를 사용하는 보드를 찾던 도중 `ATMega 32u4 MCU` 를 사용하는 `Leonardo 보드` 를 찾게 되었고, 이 보드를 이용하여 `Air Mouse` 제작을 시도하였다.

- ## 시나리오 2.
  - 두 번째 방법으로, `아두이노 Leonardo 보드` 를 이용하여 컴퓨터의 마우스 제어를 하고자 하였다.
  > &nbsp; Leonardo 보드는 `ATMega 32u4` 를 사용한다. ATMega 32u4 는 컴퓨터와의 USB 통신이 직접적으로 가능하며, 파일 업로드가 가능하고, I/O 핀들 제어가 가능하다. 따라서 Leonardo 보드를 이용하면 마우스 제어를 완전히 할 수 있다.
</br></br>

------------------------------------------

# 3. 사용부품 & 구성

<p align= "center">
<img src=/docs/readme_Imgs/readme_img3.PNG width=550 height=180></p> 

<p align= "center">
<img src=/docs/readme_Imgs/readme_img4.PNG width=450 height=140></p>

-
  - Arduino Leonardo Board
  - MPU 6050
  - Potentiometer
  - Push Button X3
  - 10K ohm Resister X3
<br>

  > &nbsp; 마우스 클릭 버튼을 총 3개로 구성하였다. 각 버튼은 좌 클릭, 더블 클릭, 우 클릭 기능을 한다. 공중에서 클릭을 하기 때문에 클릭할 때마다 마우스에 미세한 흔들림이 있는데, 이 때문에 좌 클릭을 빠르게 두 번 하는 것이 힘들다. 따라서 더블 클릭 역할을 하는 버튼을 추가로 만든 것이다. <br>
  > &nbsp; Potentiometer 로 가변 저항을 조절함으로써 마우스의 움직임 감도 조절이 가능하도록 하였다.
</br></br>

------------------------------------------

# 4. 회로도

- ## 회로도 구상
<p align= "center">
<img width=450 height=300 src="https://user-images.githubusercontent.com/86474141/153612426-43d47189-f7e1-48b8-87ae-2da279a00383.PNG"></p>
</br>

- ## 실제 연결 모습
<p align= "center">
<img width=450 height=300 src=https://user-images.githubusercontent.com/86474141/153618660-6f1cdc85-c185-4b67-806b-9d827410e038.jpg></p>
</br>

-
  - MPU 6050 : I2C 통신 (SDA, SCL 핀 / 2번, 3번 핀)
  - 1번 버튼 : 좌 클릭 (13번 핀)
  - 2번 버튼 : 더블 클릭 (12번 핀)
  - 3번 버튼 : 우 클릭 (11번 핀)
  - Potentiometer : A0 핀
<br><br>

------------------------------------------

# 5. 시연 영상

<p align= "center">
<img width=450 height=300 src=https://user-images.githubusercontent.com/86474141/153619403-48896342-cc71-49cc-ad53-b9442dd25513.gif></p>
<br><br>

------------------------------------------

# 6. 한계점

> &nbsp;공중에서 마우스를 제어하는 것이기 때문에 클릭 할 때 마다 마우스에 미세한 흔들림이 있다. 따라서 처음 사용하는 사람에게는 어색할 수 있고, 정확한 클릭을 하는데에 어려움이 있을 수 있다.
<br><br>

------------------------------------------

# 7. 참고 사이트

#### 📙 https://luftaquila.io/blog/diy/arduino-hid-control-1/
#### 📙 https://mosesnah.tistory.com/5
