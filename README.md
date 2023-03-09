# Use-ros-build-myself-robot
Use ROS1 to build myself robot.

## 系統架構
![圖片1](https://user-images.githubusercontent.com/102845636/223955033-25464efa-290b-4665-8226-cd7bb461364f.png)

## 機器人實體照    
<img src="https://user-images.githubusercontent.com/102845636/223955510-ac465627-5d71-4a60-b917-ae0b688fad7a.jpg" width = "600" height = "400" />
<img src="https://user-images.githubusercontent.com/102845636/223955516-d7bffb3d-04e1-405a-a9ce-db096098eca0.jpg" width = "600" height = "400" />
<img src="https://user-images.githubusercontent.com/102845636/223955524-898ff1e8-8f70-4763-8b1d-77f48c7c0e83.jpg" width = "600" height = "400" />
<img src="https://user-images.githubusercontent.com/102845636/223955529-9ff299aa-faf2-457d-b1a2-be4b388d9c2e.jpg" width = "600" height = "400" />

## 各資料夾介紹
    
* **ArduinoMega(Arduino Mega 1程式碼)**

  * Controller.h: 用於處理回授信號與計算馬達當前角度與速度
  * imu.h: 讀取Jy61資訊
  * Parameter.h: 機器人參數
  * PID.h: PID計算
  
* **urdf_try_1(主要檔案)**

  * **launch**:
  
    *	display: 在rviz上顯示機器人
    *	gazebo: 在gazebo上顯示機器人
    *	main: 開啟雷達、攝影機、與Arduino通訊
    *	rtabmap: SLAM
    *	navigation: 導航
    
  * **param:導航包要用的參數**
    
  * **rviz_config: rviz顯示用參數**
  
  * **script:**
  
    * Arduino.py: 與Arduino mega 1 溝通用
    *	Joint_state_pub.py: 發佈四顆輪子的transform topic
    * Pub_robot_info: 發佈從機器人上各感測器訊息，如IMU、馬達角度、馬達目前速度
    * Robot_control: 監聽/cmd_vel topic 控制機器人移動
    * Robot_tf_pub: 發佈機器人與odom topic座標系轉換
    
## 開啟USB權限
```
  sudo chmod 777 /dev/ttyACM0  
  sudo chmod 777 /dev/ttyUSB0 
```
## 啟動底盤與控制
```
  roslaunch urdf_try_1 main.launch  
  roslaunch turtlebot_teleop keyboard_teleop.launch
```
## 啟動rtab建圖
```
  roslaunch urdf_try_1 rtabmap.launch
```
## 啟動導航 (要先把keyboard_teleop.launch 關閉)
```
  roslaunch urdf_try_1 navigation.launch
```


 
