//*************************************************************************
// SimpleTask Library - Simple sequential task manager for Arduino
// V 0.1
// Lebrary Owner : Kamon Singtong (www.makearduino.com)
// fb : makearduino
//*************************************************************************
// ตัวอย่างการใช้งาน 
// 1. เรียกฟังก์ชั่น Task.create(handler,interval) ใน ฟังก์ชั่น setup 
//     handler  : ฟังก์ชั่นหรือ Lambda ที่ต้องการเรียกใช้งานตามรอบ interval
//     interval : ระยะเวลาหน่วงในแต่ระรอบ
// 2. เรียกฟังก์ชั่น Task.loop() ใน ฟังก์ชั่น loop
//*************************************************************************
/*MAX_TASK กำหนดจำนวน Task สูงสุด กำหนดเท่าที่ใช้ มีผลกับ RAM*/
//#define MAX_TASK 16
/*TASK_OVERFLOW_BIT กำหนดค่า overflow ต้องมีค่าน้อยกว่า 32 ถ้าไม่ได้กำหนดจะใช้ default 31 bit = 24วัน*/
//#define TASK_OVERFLOW_BIT 31

#include <SimpleTask.h>
int count=0;
void printSecond(task_t &tk);

void setup(){
    Serial.begin(115200);
    pinMode(LED_BUILTIN,OUTPUT);
    while(!Serial)delay(0);
    delay(100);
    /* LED Blink ON 0.5s/ OFF 0.5s */
    //กำหนด handler โดย Lambda expression
    Task.create([](task_t &tk){ digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));},500);
    /* Serial print millis() every 1000ms*/
    Task.create(serialPrintTask,1000);
}

void loop(){
    Task.loop();//ถ้องต้องการระบุ interval เป็น micros ใช้ Task.loop(micors());
}

void serialPrintTask(task_t &tk){
  Serial.println(tk.timeoffset); //timeoffset เริมนับ 0ms จากรอบแรกที่เข้า loop
  if(count>10){
    Serial.println("Stop print task. (LED Still blink)");
    tk.enabled = false;
  }
  else if(count==5){
    Serial.println("Change task interval to 2000ms.");
    tk.interval = 2000;    
  } 
  count++;    
}