

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);//LCD ekranın uçlarına bağlanan dijital pinler
int SicaklikPin = A0; // Sıcaklık Sensörü A0 pinine bağlıdır.
int OkunanDeger;   // A0’den okunacak değer 0-1023 aralığında LM35’in vereceğı en büyük okuma değeri 1V=205 olacaktır.
int Sicaklik; // sıcaklık sensöründen ölçülen değerin hesaplamalar lcd ekrana yansıtılacak değeri tutar.

void setup() {
  Serial.begin(9600); //start serial communication   
}

void loop() {
  lcd.clear();
  OkunanDeger = analogRead(SicaklikPin);//Sıcaklık sensöründen ortam sıcaklığı okunur.
  Sicaklik = map (OkunanDeger, 0, 205, 0, 100); //(0-1v 0-205 okuma değerini, 0-100 sıcaklığa dönüştür.)
  lcd.setCursor(0,1);//İmleç konumunun sütun, satır koordinatları tanımlanır.
  lcd.print(Sicaklik);// Sıcaklık değerini Lcd ekrana yazdırır.
  Serial.println(Sicaklik);
  lcd.print(" C "); // değerin yanına DERECE olarak string ifade yazılır,
  delay (500); // Bu işlemi 1 saniyede tekrarlaması için döndürülür.
}
