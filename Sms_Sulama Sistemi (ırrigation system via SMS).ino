#include<Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial sim800(3, 2);//telNomodul pinleri
Servo motor; //motor hareketi için motor nesnesi
int seviye0=0;
int seviye1=20;
int seviye2=40;
int seviye3=60;
int seviye4=90;

String telNo="05413065471";//gelen sms i kontrol için numarayı giriyoruz
int i; //
void setup()
{
 Serial.begin(9600);
 sim800.begin(9600);
 motor.attach(4); 
 
 // настройка приема сообщений
 
 sim800.print("AT+CMGF=1\r"); // SMS modunu text olarak ayarlıyoruz
 delay(500); // 
 sim800.print("AT+IFC=1, 1\r"); // veri akışının kontrolu için verilen komut
 delay(500);
 sim800.print("AT+CPBS=\"SM\"\r"); // sim rehberine erişimi açtık
 delay(500); 
 sim800.print("AT+CNMI=1,2,2,1,0\r"); // mesj bilgilerini içerir +CMT: "<TEL NUMARASI>","","<TARİH>","<SAAT>","<KAREKTER SINIRI YOK>"
 // TEL NUMARASINDAN SONRA MESJ İÇERİĞİNE ERİŞİLİR
 delay(700);
}
 
String mesaj = ""; // GELEN MESAJ İÇİN  BOŞ BİR STRİNG TANIMLIYORUZ

boolean isStringMessage = false; //ALINACAK MESAJLARA İZİN VERİLDİ


void loop()
{
 if (!sim800.available())// VERİ OLUP OLMADIĞI KONTROLÜ
 return;
 
 char bittiMi = sim800.read(); // DEĞİŞKEN GELEN MESAJA EŞİTLENDİ
  
 if ('\r' == bittiMi) // MESAJIN BİTİP BİTMEDİĞİ KONTROL EDİLİR
 {
 if (isStringMessage) { // MESAJ GEÇERLİ İSE

 if (!mesaj.compareTo("kapat")) { // GELEN MESAJ A'YA EŞİTSE
    motor.write(seviye0);          // MOTORU AÇ
 } 
 if (!mesaj.compareTo("az")) { // GELEN MESAJ B'YE EŞİTSE MOTORU KAPAT
   motor.write(seviye1);
 }
 if (!mesaj.compareTo("orta")) { // GELEN MESAJ B'YE EŞİTSE MOTORU KAPAT
   motor.write(seviye2);
 }
 if (!mesaj.compareTo("normal")) { // GELEN MESAJ B'YE EŞİTSE MOTORU KAPAT
   motor.write(seviye3);
 }
 if (!mesaj.compareTo("cok")) { // GELEN MESAJ B'YE EŞİTSE MOTORU KAPAT
   motor.write(seviye4);
 }    
  isStringMessage = true;// true yapabilirsin
     
  } 
 
 else {
 if (mesaj.startsWith("+CMT"))
 { // GELEN VERİ +CMT İLE BAŞLIYORSA 
   for (i = 0; i <= 1; i++)
   { // ÜSTTE TELEFONU İSTERSEK DİZİ YAPIP BİRKAÇ TEL EKLEYEBİLİRİZ
     if (mesaj.indexOf(telNo)) //MESAJ NUMARASI ADMİN NUMARALARINA EŞİT İSE
     { 
      Serial.println(telNo);
      isStringMessage = true;
      break;
     }
   } 
 
  }
 }
 mesaj = ""; 
 } 
 
 else if ('\n' != bittiMi) { //  игнорируем второй символ в последовательности переноса строки: \r\n , и 
 mesaj += String(bittiMi); //дополняем текущую команду новым сиволом
 }
}
