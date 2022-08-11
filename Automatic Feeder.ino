#include "RTClib.h"
RTC_DS3231 rtc;

// Sensor Suhu
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire pin_DS18B20 (7); //PIN Sensor Suhu
DallasTemperature DS18B20 (&pin_DS18B20);

// Sensor Berat
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <HX711.h>
LiquidCrystal_I2C lcd(0X27, 16, 2);
HX711 scale (3, 2); //DT 6 , SC 5

float calibration_factor = -439;
float units;
float ounces;
float akhir;
Servo servo;
Servo servo1;
int flag;
float pakan2;

int range[7];
byte alpha[7];
byte alpha_hasil[7];
byte rules[12];
float hasil;
float gram; //hasil defuzzifikasi
float gram1;
float pakan; // pakan
float jarak_kiri;
int jarak_kanan;
int udang_hidup;
float udang_hidup1;
float suhu;
float inputsuhu;
float inputumur;
float Suhu_D;
float Suhu_N;
float Suhu_P;
float Umur_1;
float Umur_2;
float Umur_3;
float Umur_4;
float aturan1;
float aturan2;
float aturan3;
float aturan4;
float aturan5;
float aturan6;
float aturan7;
float aturan8;
float aturan9;
float aturan10;
float aturan11;
float aturan12;
float derajat;
//DateTime now;
int hari;
int bulan;
int tahun;
int umur;
int hari2;
int bulan2;
int tahun2;
int umur2;
int selisih;
int selisih1;
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

void setup() {
  Serial.begin(9600);
  DS18B20.begin();
  servo.attach(4);
  servo.write(100);
 
  servo1.attach(8);
  servo1.write(0);
  //motor.setSpeed(12);

//RTC Setup
 
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("Timbang Pakan");
  scale.set_scale(calibration_factor);
  scale.tare();
  
  

  range [0] = 26; // fungsi keanggotaan SuhuD
  range [1] = 29; // fungsi keanggotaan SuhuN
  range [2] = 32; // fungsi keanggotaan SuhuP
  range [3] = 20; // fungsi keanggotaan Umur1
  range [4] = 29; // fungsi keanggotaan Umur2
  range [5] = 38; // fungsi keanggotaan Umur3
  range [6] = 44; // fungsi keanggotaan Umur4

}

void loop() {
 DateTime now = rtc.now();
 Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    delay(1000);
 if((now.hour() == 19 && now.minute() == 10 && now.second()==5)||(now.hour() == 15 && now.minute() == 1 && now.second()==5)||
 (now.hour() == 20 && now.minute() == 1 && now.second()==5))
 {
 Serial.println("Beri Pakan");
 rtcumur();
 inputumur = selisih1;
 input_suhu();
 inputsuhu = suhu;
 


 //Serial.print("Sensor suhu = ");
 //while (Serial.available()==0){
 //}
 //jarak_kiri = Serial.parseFloat();
 //Serial.println(jarak_kiri);
 //while (Serial.available()==0){
 //}
 //inputsuhu = Serial.parseFloat();
 //inputsuhu = (float) jarak_kiri;


  //Serial.print("masukan umur = ");
  //while (Serial.available()==0){
  //}
  //jarak_kanan = Serial.parseInt();
  //Serial.println(jarak_kanan);
  //while (Serial.available()==0){
  //}
  //inputumur = Serial.parseInt();
  //inputumur = (float) jarak_kanan;



  Suhu_D = 26;
  Suhu_N = 29;
  Suhu_P = 32;
  Umur_1 = 20;
  Umur_2 = 29;
  Umur_3 = 38;
  Umur_4 = 44;
  
  fuzzyfikasi_1();
  fuzzyfikasi_2();
  fuzzyfikasi_3();
  fuzzyfikasi_4();
  fuzzyfikasi_5();
  fuzzyfikasi_6();
  fuzzyfikasi_7();

  alpha_hasil[0] = alpha[0];
  alpha_hasil[1] = alpha[1];
  alpha_hasil[2] = alpha[2];
  alpha_hasil[3] = alpha[3];
  alpha_hasil[4] = alpha[4];
  alpha_hasil[5] = alpha[5];
  alpha_hasil[6] = alpha[6];

 
  evaluasi_rules();
  defuzzifikasi();
  Serial.println(pakan);
  flag = 0;
  while (flag != 1){
    keluar();
    gram_pakan();
    Serial.println(pakan);
  }
  Serial.println("Kasih pakan selesai");
 delay(1000);
 }
}
void fuzzyfikasi_1(){
  if (inputsuhu < Suhu_D){
    derajat = 1;
  }
  else if (inputsuhu >= Suhu_D && inputsuhu <= Suhu_N){
    derajat = (Suhu_N-inputsuhu)/(Suhu_N-Suhu_D);
  }
  else{
    derajat = 0;
  }
  derajat = derajat*100;
  alpha[0] = (byte) derajat;
}

void fuzzyfikasi_2(){
  if (inputsuhu >= Suhu_D && inputsuhu <= Suhu_N){
    derajat = (inputsuhu-Suhu_D)/(Suhu_N-Suhu_D);
  }
  else if (inputsuhu >= Suhu_N && inputsuhu <= Suhu_P){
    derajat = (Suhu_P-inputsuhu)/(Suhu_P-Suhu_N);
  }
  else {
    derajat = 0;
  }
  derajat = derajat*100;
  alpha[1] = (byte)derajat;
}

void fuzzyfikasi_3(){
  if (inputsuhu > Suhu_P){
    derajat = 1;
  }
  else if (inputsuhu >= Suhu_N && inputsuhu <= Suhu_P){
    derajat = (inputsuhu-Suhu_N)/(Suhu_P-Suhu_N);
  }
  else {
    derajat = 0;
  }
  derajat = derajat*100;
  alpha[2] = (byte)derajat;
}

void fuzzyfikasi_4(){
  if (inputumur < Umur_1){
    derajat = 1;
  }
  else if (inputumur >= Umur_1 && inputumur <= Umur_2){
    derajat = (Umur_2-inputumur)/(Umur_2-Umur_1);
  }
  else {
    derajat = 0;
  }
  derajat = derajat*100;
  alpha[3] = (byte)derajat;
}

void fuzzyfikasi_5(){
  if (inputumur >= Umur_1 && inputumur < Umur_2){
    derajat = (inputumur-Umur_1)/(Umur_2-Umur_1);
  }
  else if (inputumur >= Umur_2 && inputumur <= Umur_3 ){
    derajat = (Umur_3 - inputumur)/(Umur_3-Umur_2);
  }
  else {
    derajat = 0;
  }
  derajat = derajat*100;
  alpha[4] = (byte) derajat;
}

void fuzzyfikasi_6(){
  if (inputumur >= Umur_2 && inputumur <= Umur_3){
    derajat = (inputumur-Umur_2)/(Umur_3-Umur_2);
  }
  else if (inputumur >= Umur_3 && inputumur <= Umur_4){
    derajat = (Umur_4-inputumur)/(Umur_4-Umur_3);
  }
  else {
    derajat = 0;
  }
  derajat = derajat*100;
  alpha[5] = (byte) derajat;
}

void fuzzyfikasi_7(){ //jauh
  if (inputumur >= Umur_4){
    derajat = 1;
  }
  else if (inputumur >= Umur_3 && inputumur <= Umur_4){
    derajat = (inputumur-Umur_3)/(Umur_4-Umur_3);
  }
  else {
    derajat = 0;
  }
  derajat = derajat*100;
  alpha[6] = (byte) derajat;
}

void evaluasi_rules(){
  rules[0] = min(alpha_hasil[0], alpha_hasil[3]); // S
  rules[1] = min(alpha_hasil[0], alpha_hasil[4]); // S
  rules[2] = min(alpha_hasil[0], alpha_hasil[5]); // SD
  rules[3] = min(alpha_hasil[0], alpha_hasil[6]); // SD
  rules[4] = min(alpha_hasil[1], alpha_hasil[3]); // SD
  rules[5] = min(alpha_hasil[1], alpha_hasil[4]); // SD
  rules[6] = min(alpha_hasil[1], alpha_hasil[5]); // B
  rules[7] = min(alpha_hasil[1], alpha_hasil[6]); // B
  rules[8] = min(alpha_hasil[2], alpha_hasil[3]); // SD
  rules[9] = min(alpha_hasil[2], alpha_hasil[4]); // B
  rules[10] = min(alpha_hasil[2], alpha_hasil[5]); // SB
  rules[11] = min(alpha_hasil[2], alpha_hasil[6]); // SB
}

void defuzzifikasi(){
  aturan1 = (float) rules[0];
  aturan2 = (float) rules[1];
  aturan3 = (float) rules[2];
  aturan4 = (float) rules[3];
  aturan5 = (float) rules[4];
  aturan6 = (float) rules[5];
  aturan7 = (float) rules[6];
  aturan8 = (float) rules[7];
  aturan9 = (float) rules[8];
  aturan10 = (float) rules[9];
  aturan11 = (float) rules[10];
  aturan12 = (float) rules[11];

  aturan1 = aturan1/100;
  aturan2 = aturan2/100;
  aturan3 = aturan3/100;
  aturan4 = aturan4/100;
  aturan5 = aturan5/100;
  aturan6 = aturan6/100;
  aturan7 = aturan7/100;
  aturan8 = aturan8/100;
  aturan9 = aturan9/100;
  aturan10 = aturan10/100;
  aturan11 = aturan11/100;
  aturan12 = aturan12/100;

  Serial.println(alpha[0]);
  Serial.println(alpha[1]);
  Serial.println(alpha[2]);
  Serial.println(alpha[3]);
  Serial.println(alpha[4]);
  Serial.println(alpha[5]);
  Serial.println(alpha[6]);

  hasil = (aturan1*0.01)+(aturan2*0.010)+(aturan3*0.015)+(aturan4*0.015)+(aturan5*0.015)+(aturan6*0.015)+(aturan7*0.020)+(aturan8*0.020)+(aturan9*0.015)+(aturan10*0.020)+(aturan11*0.032)+(aturan12*0.032);
  jarak_kiri = rules[0] + rules[1] + rules[2] + rules[3] + rules[4] + rules[5] + rules[6] + rules[7] + rules[8] + rules[9] + rules[10] + rules[11];
  derajat = (float)jarak_kiri;
  derajat = derajat/100;
  Serial.println(hasil);
  Serial.println(derajat);
  gram = hasil/derajat; // hasil fuzzy
  Serial.print("Hasil Defuzzyfikasi = ");
  Serial.print(gram, 4);
   Serial.println();
  gram1 = gram/3;
  Serial.print("Hasil Pembagian 3 waktu = ");
  Serial.print(gram1, 4);
  Serial.println();

  //jumlah udang
  Serial.print("Masukkan Udang Yang Hidup = ");
  while (Serial.available()==0){
  }
  udang_hidup = Serial.parseInt();
  Serial.println(udang_hidup);
  while (Serial.available()==0){
  }
  udang_hidup1 = Serial.parseInt();
  udang_hidup1 = (float)udang_hidup;
  pakan = gram*udang_hidup1;
  Serial.print("Pakan Udang = ");
  Serial.println(pakan, 4); //hasil pakan
  delay (2000);
  pakan = gram1*udang_hidup1;
  Serial.print("Pakan Udang 3 waktu = ");
  Serial.println(pakan, 4); //hasil pakan
  delay (2000);
  //return(pakan);
}

void input_suhu(){
  DS18B20.setResolution(12);
  DS18B20.requestTemperatures();
  Serial.print("Suhu Celsius: ");
  suhu = DS18B20.getTempCByIndex(0);
  Serial.println(suhu);
  delay(500);
}


void keluar(){
  servo1.write(180);
  delay(100);
  servo1.write(0);
  delay(100);
}

void gram_pakan(){
  lcd.setCursor(0, 1);
  lcd.print("Berat: ");
  units = scale.get_units(),10;
  pakan2 = pakan+1.00;
  if ((units > pakan)&&(units < pakan2)) {
  flag = 1;
  lcd.setCursor(6, 1);  
  lcd.print(units);
  lcd.setCursor(10,1);
  lcd.print(" gram");
  delay(1000);
    servo.write(9);
    delay(5000);
    servo.write(100); 
}
  if (units < 0.99)
  {
    units = 0.00;    
}
  //y = 0.0038x2 + 0.9537x + 0.1572
  //akhir = (0.0038*(pow(units,2))+(0.9537*units)+(0.1572));
  //ounces = units * 0.035274;
  ounces = units * akhir;
  lcd.setCursor(6, 1);
  lcd.print(units);
  lcd.setCursor(10,1);
  lcd.print(" gram");
  delay(500);
}

void rtcumur(){
  DateTime now = rtc.now();
    //Serial.print(now.year(), DEC);
    //Serial.print('/');
    //Serial.print(now.month(), DEC);
    //Serial.print('/');
    //Serial.print(now.day(), DEC);
    //Serial.print(" (");
    //Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    //Serial.print(" ");
    //Serial.print(now.hour(), DEC);
    //Serial.print(':');
    //Serial.print(now.minute(), DEC);
    //Serial.print(':');
    //Serial.print(now.second(), DEC);
    //Serial.println();

    //Serial.print(" since midnight 1/1/1970 = ");
    //Serial.print(now.unixtime());
    //Serial.print("s = ");
    //Serial.print(now.unixtime() / 86400L);
    //Serial.println("d");

    // calculate a date which is 7 days, 12 hours, 30 minutes, 6 seconds into the future
    DateTime future (2022, 6, 1, 0, 0, 0);

    //Serial.print(" now + 11d + 12h + 30m + 6s: ");
    //Serial.print(future.year(), DEC);
    //Serial.print('/');
    //Serial.print(future.month(), DEC);
    //Serial.print('/');
    //Serial.print(future.day(), DEC);
    //Serial.print(' ');
    //Serial.print(future.hour(), DEC);
    //Serial.print(':');
    //Serial.print(future.minute(), DEC);
    //Serial.print(':');
    //Serial.print(future.second(), DEC);
    //Serial.println();

    //Serial.print("Temperature: ");
    //Serial.print(rtc.getTemperature());
    //Serial.println(" C");
    ///Serial.print("Tanggal Sekarang: ");
    hari = now.day();
    ///Serial.println(hari);
    ///Serial.print("Bulan Sekarang: ");
    bulan = now.month();
    ///Serial.println(bulan);
    ///Serial.print("Tahun Sekarang: ");
    tahun = now.year();
    ///Serial.println(tahun);
    
    ///Serial.print("Jumlah Hari: ");
    umur =(30.443*(bulan-1) - 1.0095+hari);
    ///Serial.print(umur);
    ///Serial.println(" Hari");

    //Serial.print("Tanggal pertama: ");
    hari2 = future.day();
    ///Serial.println(hari2);
    ///Serial.print("Bulan pertama: ");
    bulan2 = future.month();
    ///Serial.println(bulan2);
    ///Serial.print("Tahun pertama: ");
    tahun2 = future.year();
    ///Serial.println(tahun2);
      selisih = tahun2-tahun; // ini yang di tambah
      if(selisih == 1) { // ini yang di tambah
        bulan2 = bulan2+12; // ini yang di tambah
      }
    ///Serial.print("Jumlah Hari: ");
    umur2 =(30.443*(bulan2-1) - 1.0095+hari2);
    //Serial.print(umur2);
    //Serial.println(" Hari");

    Serial.print("Umur Udang Sekarang: ");
    selisih1 = umur-umur2;
    Serial.print(selisih1);
    Serial.println(" Hari");
    delay(1000);
}
