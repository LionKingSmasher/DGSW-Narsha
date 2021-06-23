#include <SPI.h>
#include <SD.h>

File myFile;

char buffer[100];

void setup() {
  Serial.begin(115200);

  Serial.print("Initializing SD card...");
  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    while(1);
  }
  Serial.println("initialization done.");
  
  myFile = SD.open("tiger.bmp");
  if (myFile) {
    //myFile.seek(0x40);
    myFile.read(buffer, 16);
    // for(int i=0;i<16;i++) Serial.println(buffer[i], HEX);
    for(int i=0;i<16;i++) Serial.println((char)buffer[i]);

    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}

void loop() {


}
