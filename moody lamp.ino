/* Dalam projek ini digunakan sensor suara bertipe GY-MAX4466 microphone
   , RGB Common Cathode, dan Arduino UNO R3 
   6 November 2022 9:36 PM */

int micPin = A0;        // 1 ---> pin microphone sebagai output dari microphone. yang kemudian sinyalnya ditangkap arduino melalui pin analog (A0)
int micValue;

int red = 9;            // 2 ---> inisialisasi pin merah di digital D9, pin hijau di D10, dan pin biru di D11
int green = 10;
int blue = 11;

int readDiff;            // 3 ---> (bacaan analog - base) *base akan dibahas di poin 4*          
int base = 340;          // 4 ---> output analog jika tidak ada suara sama sekali (digunakan sebagai patokan)
int highestReadDiff = 0; // 5 ---> variabel untuk menyimpan angka output analog tertinggi

unsigned long myTime;
int signalMax = 0;
int signalMin = 1024;
int peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
double volts;
double first;
double scnd;

void setup() {
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  
  analogWrite(green, 255);                      // 6 ---> base color adalah hijau yang berarti hanya warna hijau murni yang menyala di saat tidak ada suara sama sekali
  analogWrite(blue, 0);
  analogWrite(red, 0);

  myTime = millis();                            // 7 ---> inisialisasi variabel "myTime" ke millis(). millis adalah waktu yang telah berlalu semenjak arduino dinyalakan (dalam millisecond)
  
}

void loop() {

  readDiff = (abs(analogRead(micPin) - base));  // 8 ---> output analog dikurang dengan base (misal output analognya 366 maka readDiff menjadi 366 - 340 = 24)

  if (readDiff > highestReadDiff) {             // 9 ---> jika readDiff lebih besar dari angka output analog tertinggi yang sebelumnya maka ...
    if ((readDiff*3) < 256) {                   // 10 ---> jika readDiff dikalikan 3 lebih kecil dari 256 (karena angka maksimal dalam 16 juta kombinasi warna RGB adalah 255 R, 255 G, dan 255 B (24-bit color)
      analogWrite(blue, readDiff*3);            // 11 ---> warna biru dinaikkan sebesar readDiff dikalikan 3
      analogWrite(green, 255 - (readDiff*3));   // 12 ---> warna hijau diturunkan sebanyak 255 - (readDiff*3)
    }
    else if ((readDiff*3) < 512) {              // 13 ---> jika readDiff dikalikan 3 lebih kecil dari 512
      analogWrite(green, 0);                    // 14 ---> warna hijau dihilangkan / dijadikan 0
      analogWrite(blue, 512 - (readDiff*3));    // 15 ---> warna biru diubah menjadi 512 - (readDiff*3) (karena dalam condition ke-2 ini angka readDiff sudah pasti > 255)
      analogWrite(red, (readDiff*3) - 256);     // 16 ---> warna merah mulai dimunculkan dengan rumus (readDiff*3 - 256)
    }
    else {                                      // 17 ---> jika readDiff lebih dari 512
      analogWrite(red, 255);                    // 18 ---> akan hanya muncul 1 warna yaitu merah murni (tanpa kombinasi hijau maupun biru)
      analogWrite(green, 0);
      analogWrite(blue, 0);
    }

    highestReadDiff = readDiff;                 // 19 ---> karena seperti pada poin ke-9, readDiff lebih besar dari highestReadDiff, maka rekor highestReadDiff yg sebelumnya digantikan oleh readDiff yg skrg
    myTime = millis();                          // 20 ---> variabel myTime digantikan dengan millis() yang baru
  }

  if ((millis() - myTime) > 250) {              // 21 ---> asumsikan jika tidak ada rekor highestReadDiff yang baru dalam 0.25 detik (250 ms) belakangan, maka otomatis variabel highestReadDiff diisi angka 0
    highestReadDiff = 0;                        

  }

  micValue = analogRead(micPin);                
  //Serial.println(micValue, DEC);                // 22 ---> bagian ini hanya berfungsi untuk menampilkan angka bacaan analog (misal 344, 376, 352, dll) ke console, agar bisa dilihat grafiknya di serial plotter
                                                // 23 ---> selanjutnya program akan terus menerus melakukan forever loop mulai dari void loop() (poin ke-8 sampai ke-22) beda dengan void setup() yang hanya dilakukan 1x

  
  double volts = ((peakToPeak * 3.3) / 1024) * 0.707;  // convert to RMS voltage
  double first = log10(volts/0.00631)*20;
  double scnd = first + 94 - 44 - 25;
  Serial.println(scnd);
}

