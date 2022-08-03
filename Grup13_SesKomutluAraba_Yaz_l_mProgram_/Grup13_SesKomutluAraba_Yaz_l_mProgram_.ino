#include <AFMotor.h> // Adafruit Motor Shield Kitaplığı. AFMotor kütüphane indirip yüklemelisiniz
#include <Servo.h>   //Servo kütüphanesi.
String voice; //bluetooth sensörü için algılanan sesi tutar.

AF_DCMotor motor1 (3, MOTOR12_1KHZ); // 1kHz PWM frekansına ayarlanmış Motor Sürücü Kalkanı üzerindeki M3 çıkışını kullanarak motor 1 oluşturma
AF_DCMotor motor2 (4, MOTOR12_1KHZ); // 1kHz PWM frekansına ayarlanmış Motor Sürücü Kalkanı üzerindeki M1 çıkışını kullanarak motor 2 oluşturma
Servo myServo; //servo tanımlama.
int LED1 = A0; //LED 1 pin tanımlama.
int LED2 = A1; //LED 2 pin tanımlama.
int LDRPin = A4;//Işık sensörü.
int buzzerPin = A2; // buzzer pini tanımlama
int trigPin = 8; //Mesafe sensörünün trig ucunu tanımlama
int echoPin = 9; / Mesafe sensörünün echo ucunu tanımlama
long zaman;
long mesafe;

void setup() {
  Serial.begin(9600); // seri iletişimi başlatma
  myServo.write(90); // servo pozisyonu 90 derece ayarlama
  pinMode(LED1, OUTPUT); //LED 1 pin tanımlama.
  pinMode(LED2, OUTPUT); // LED 2 pin tanımlama.
  pinMode(buzzerPin, OUTPUT); // buzzer pinini tanımlama.
  pinMode(trigPin, OUTPUT); // mesafe sensörünün trig ucunu çıkış olarak tanımlama.
  pinMode(echoPin, INPUT); // mesafe sensörünün echo ucunu giriş olarak tanımlama.

}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);// trig pini 5 mikro saniye boyunca LOW konumuna tutma.
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);// 10 mikro saniye sonunda trig pin, tekrardan HIGH konumuna getirilir
  digitalWrite(trigPin, LOW);
  zaman = pulseIn(echoPin, HIGH); //Echo pini HIGH konumuna gelme süresi pulseIn fonksiyonuyla ölçme
  mesafe = (zaman / 29.1) / 2; //Ölçülen süre sesin havadaki yayılma hızı 29’a bölünür.
  //Geliş gidiş mesafesini hesaplanır, tek mesafenin hesaplanması için 2’ye bölünerek uzaklık ölçülür
  Serial.print("Uzaklik ");
  Serial.print(mesafe);
  Serial.println(" cm");
  delay(500);
  int isik_degeri = analogRead(LDRPin);//
  Serial.println(isik_degeri);
  delay(500);
  while (Serial.available()) { // Okunacak bir bayt olup olmadığını kontrol etme
    delay(10); // kararlı hale getirmek için gecikme eklendi
    char c = Serial.read(); // Seri okuma gerçekleştirin
    if (c == '#') {
      break; // Kelimeden sonra # algılandığında döngüden çık    }
      voice += c; // ses kısaltması = ses + c
    }


    if (voice.length() > 0) { //ses komutundan gelen bilgiyi kontrol etme
      if (voice == "*ileri git") { //komut ileri git ise
        if (mesafe > 20) { //mesafesi 20 cm’den büyük ise
          motor1.run(FORWARD); //motor1 ileri
          motor1.setSpeed(180); //hız 180
          motor2.run(FORWARD); //motor2 ileri
          motor2.setSpeed(180);
          delay(2000); //2 saniye boyunca fonksiyonu tekrarlaması
          motor1.run(RELEASE); //motor1 dur
          motor2.run(RELEASE); //motor2 dur
        }
        else { //mesafe 20 cm’den küçük ise araba hareket etmez.
          Serial.println("Mesafe kısa"); //Serial ekrana mesafe kısa yazması.
        }
      }
      else if (voice == "*geri git") { //komut geri git ise
        motor1.run(BACKWARD); //motor1 geri
        motor1.setSpeed(180); //hız 180
        motor2.run(BACKWARD); //motor2 geri
        motor2.setSpeed(180); //hız 180
        delay(2000); //2 saniye boyunca fonksiyonu tekrarlaması
        motor1.run(RELEASE); //motor1 dur
        motor2.run(RELEASE); //motor2 dur

      }
      else if (voice == "*sola dön") {//komut sola dön ise
        myServo.write(135); //servo motorun 135 dereceye gelmesi
        delay(1000);//1 saniye beklemesi
        myServo.write(90);//servo motorun 90 dereceye gelmesi
        delay(1000);
        if (mesafe > 10) {//mesafe 10 cm’den büyükse
          motor1.run(FORWARD);
          motor1.setSpeed(200);
          motor2.run(BACKWARD);
          motor2.setSpeed(200);
          delay(300);
          motor1.run(RELEASE);
          motor2.run(RELEASE);
        }
        else {//mesafe 10 cm’den küçükse
          Serial.println("Mesafe kısa");
        }
      }
      else if (voice == "*sağa dön") {//komut sağa dön ise
        myServo.write(45); //servo motorun 45 dereceye gelmesi
        delay(1000);
        myServo.write(90);//servo motorun 90 dereceye gelmesi
        delay(1000);
        if (mesafe > 10) {
          motor1.run(BACKWARD);
          motor1.setSpeed(200);
          motor2.run(FORWARD);
          motor2.setSpeed(200);
          delay(300);
          motor1.run(RELEASE);
          motor2.run(RELEASE);
        }
        else {//mesafe 10 cm’den küçükse
          Serial.println("Mesafe kısa");
        }
      }

      else if (voice == "*far aç") {//komut far aç ise
        if (isik_degeri < 30)//ışık değeri 30’dan küçük ise
        {
          LED_on();//led on fonksiyonuna gider
        }
        else {
          )//ışık değeri 30’dan büyük ise
          LED_off();//led off fonksiyonuna gider
        }
      }
      else if (voice == "*far kapat") { //komut far kapat ise
        LED_off();//led off fonksiyonuna gider
      }
      else if (voice == "*korna çal") {//komut korna çal ise
        buzzer_on();//buzzer on fonksiyonuna gider
      }
      else if (voice == "*dur") {//komut dur ise
        stop_car();//stop car fonksiyonuna gider
      }

      voice = ""; // Başlattıktan sonra değişkeni sıfırlama
    }
  }

  void LED_on ()//ledlerin yakılması
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  }

  void LED_off ()//ledlerin kapatılması
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }

  void buzzer_on ()//8 ms buzzerin çalması
  {
    tone(buzzerPin, 100);
    delay(800);
    noTone(buzzerPin);
  }
  void stop_car ()//motorların durması
  {
    motor1.run(RELEASE);
    motor2.run(RELEASE);
  }
