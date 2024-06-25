#include <LiquidCrystal.h>

#define LED_PIN 13  // Alarm LED'inin bağlı olduğu digital pin
#define BUZZER_PIN 12  // Buzzer'ın bağlı olduğu digital pin

#define EEPROM_ADDR 0  // EEPROM adresi
#define MAX_ALARM_COUNT 255  // Maksimum alarm sayısı

int alarmCount = 0;  // Alarm sayacı
bool isDark = false;  // Karanlık durumu

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // LCD pin konfigürasyonu

void setup() {
  pinMode(LED_PIN, OUTPUT);  // LED pinini çıkış olarak ayarla
  pinMode(BUZZER_PIN, OUTPUT);  // Buzzer pinini çıkış olarak ayarla

  lcd.begin(16, 2);  // LCD'yi başlat

  // Timer1 ayarlamaları
  cli();  // Kesmeler durduruldu
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;  // (16*10^6) / (1*1024 - 1)
  TCCR1B |= (1 << WGM12);  // CTC modu ayarlandı
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler ayarlandı (1024'e bölme)
  TIMSK1 |= (1 << OCIE1A);  // Output Compare A kesmesi aktif hale getirildi
  sei();  // Kesmeler aktif hale getirildi

  alarmCount = EEPROM.read(EEPROM_ADDR);  // EEPROM'dan alarm sayacını oku

  // LCD'ye başlangıç bilgisini yazdır
  lcd.print("Alarm Sayisi: ");
  lcd.print(alarmCount);
}

void loop() {
  // Ana döngüde bir şey yapmanıza gerek yok
}

// Timer1 Output Compare A kesmesi
ISR(TIMER1_COMPA_vect) {
  int sensorValue = analogRead(A0);  // LDR sensöründen değeri oku

  if (sensorValue < 500) {  // Eğer ortam karanlık ise
    if (!isDark) {  // Önceden karanlık değilse
      alarm();  // Alarmı etkinleştir
      isDark = true;  // Karanlık durumunu güncelle
    }
  } else {  // Eğer ortam aydınlık ise
    isDark = false;  // Karanlık durumunu güncelle
  }
}

void alarm() {
  alarmCount++;  // Alarm sayacını artır
  if (alarmCount > MAX_ALARM_COUNT) {
    alarmCount = MAX_ALARM_COUNT;  // Maksimum alarm sayısını aşarsa sınırla
  }

  EEPROM.write(EEPROM_ADDR, alarmCount);  // Alarm sayacını EEPROM'a kaydet

  // LCD'ye alarm bilgisini güncelle
  lcd.clear();
  lcd.print("Alarm Sayisi: ");
  lcd.print(alarmCount);

  digitalWrite(LED_PIN, HIGH);  // LED'i yak
  digitalWrite(BUZZER_PIN, HIGH);  // Buzzer'ı aktif et

  delay(1000);  // 1 saniye beklet

  digitalWrite(LED_PIN, LOW);  // LED'i söndür
  digitalWrite(BUZZER_PIN, LOW);  // Buzzer'ı pasif et
}
