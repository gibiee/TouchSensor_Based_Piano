#include <CapacitiveSensor.h>
#include <SoftwareSerial.h>

CapacitiveSensor cs_A0_13 = CapacitiveSensor(A0,13); //A0:송신핀, 13:수신핀
CapacitiveSensor cs_A1_12 = CapacitiveSensor(A1,12);
CapacitiveSensor cs_A2_11 = CapacitiveSensor(A2,11);
CapacitiveSensor cs_A3_10 = CapacitiveSensor(A3,10);
CapacitiveSensor cs_A4_9 = CapacitiveSensor(A4,9);
CapacitiveSensor cs_A5_8 = CapacitiveSensor(A5,8);
CapacitiveSensor cs_6_7 = CapacitiveSensor(6,7); //6:송신핀 7:수신핀

SoftwareSerial BTSerial(2,3);   //Tx:2 Rx:3

int piezo = 4;
int tones[] = {261, 294, 330, 349, 392, 440, 494};

unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long recordStartTime = 0;
String record = "";
bool isRecord = false;

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(piezo, OUTPUT);
}
void loop()
{
  long SEN1 = cs_6_7.capacitiveSensorRaw(30);
  long SEN2 = cs_A5_8.capacitiveSensorRaw(30);
  long SEN3 = cs_A4_9.capacitiveSensorRaw(30);
  long SEN4 = cs_A3_10.capacitiveSensorRaw(30);
  long SEN5 = cs_A2_11.capacitiveSensorRaw(30);
  long SEN6 = cs_A1_12.capacitiveSensorRaw(30);
  long SEN7 = cs_A0_13.capacitiveSensorRaw(30);

  /*
  Serial.print(SEN1);
  Serial.print(" ");
  Serial.print(SEN2);
  Serial.print(" ");
  Serial.print(SEN3);
  Serial.print(" ");
  Serial.print(SEN4);
  Serial.print(" ");
  Serial.print(SEN5);
  Serial.print(" ");
  Serial.print(SEN6);
  Serial.print(" ");
  Serial.println(SEN7);
  */

  // 건반 터치시 작동
  if(SEN1 > 10) {
    tone(piezo, tones[0]);
    if(isRecord == true && startTime == 0) {
      record += "Do";
      startTime = millis() - recordStartTime;
    }
  }
  if(SEN2 > 10) {
    tone(piezo, tones[1]);
    if(isRecord == true && startTime == 0) {
      record += "Le";
      startTime = millis() - recordStartTime;
    }
  }
  if(SEN3 > 10) {
    tone(piezo, tones[2]);
    if(isRecord == true && startTime == 0) {
      record += "Mi";
      startTime = millis() - recordStartTime;
    }
  }
  if(SEN4 > 10) {
    tone(piezo, tones[3]);
    if(isRecord == true && startTime == 0) {
      record += "Pa";
      startTime = millis() - recordStartTime;
    }
  }
  if(SEN5 > 10) {
    tone(piezo, tones[4]);
    if(isRecord == true && startTime == 0) {
      record += "Sol";
      startTime = millis() - recordStartTime;
    }
  }
  if(SEN6 > 10) {
    tone(piezo, tones[5]);
    if(isRecord == true && startTime == 0) {
      record += "La";
      startTime = millis() - recordStartTime;
    }
  }
  if(SEN7 > 10) {
    tone(piezo, tones[6]);
    if(isRecord == true && startTime == 0) {
      record += "Si";
      startTime = millis() - recordStartTime;
    }
  }

  // 모든 건반에서 손을 뗐을 때 noTone() 수행
  if(SEN1==0 && SEN2==0 && SEN3==0 && SEN4==0 && SEN5==0 && SEN6==0 && SEN7==0) {
    noTone(piezo);
    if(startTime != 0) {
      endTime = millis() - recordStartTime;
      record = record + ':' + startTime + '~' + endTime + ',';
      startTime = 0;
    }
  }

  // 블루투스 통신이 작동할 때 수행. = 어플에서 버튼을 눌렀을 때
  if (BTSerial.available()){
    char BTdata = BTSerial.read();
    Serial.println(BTdata); // 받은 문자 출력
    BTSerial.print(BTdata); // 받은 문자 그대로 전송

    // 녹음 시작(Start) 및 종료(End)
    if(BTdata == 'S') {
      recordStartTime = millis();
      isRecord = true;
    }
    else if(BTdata == 'E') {
      Serial.println(record); // record 출력
      BTSerial.print(record); // record 기록을 전송
      record = ""; //record 기록 초기화
      isRecord = false;
    }
  }
}
