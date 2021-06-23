#include <SPI.h>
#include <SD.h>

#define BUFFER_LEN 1024

File myFile;

char buffer[1024];
char gcode_buf[50][50];
int gcode_index = 0;
int buf_index = 0;
int num_gcode = 0;
unsigned int buf_total_len = 0;
unsigned int buf_len = 0;

void setup() {
  Serial.begin(115200);

  Serial.print("Initializing SD card...");
  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    while(1);
  }
  Serial.println("initialization done.");
  
  myFile = SD.open("gcode.gco");
  
      //myFile.seek(buf_total_len);
      // myFile.read(buffer, BUFFER_LEN);
      // for(int i = 0; i < BUFFER_LEN; i++){
      //   gcode_buf[gcode_index][buf_index++] = buffer[i];
      //   if(buffer[i] == '\n'){
      //     buf_total_len += buf_index;
      //     buf_len += buf_index;
      //     Serial.println(gcode_buf[gcode_index]);
      //     for(int k=0; k < 50; k++) gcode_buf[gcode_index][k] = 0;
      //     buf_index = 0;
      //     if(gcode_index == 50) gcode_index = 0;
      //     num_gcode++;
      //     if(buf_len >= 1000) {
      //       buf_len = 0;
      //       break;
      //     }
      //     delay(300);
      //   }
      // }
    // while(1){
    //   char tmp_read = myFile.read();
    //   //myFile.seek(buf_total_len);
    //   myFile.read(buffer, BUFFER_LEN);
    //   for(int i = 0; i < BUFFER_LEN; i++){
    //     gcode_buf[gcode_index][buf_index++] = buffer[i];
    //     if(buffer[i] == '\n'){
    //       buf_total_len += buf_index;
    //       buf_len += buf_index;
    //       Serial.println(gcode_buf[gcode_index]);
    //       for(int k=0; k < 50; k++) gcode_buf[gcode_index][k] = 0;
    //       buf_index = 0;
    //       if(gcode_index == 50) gcode_index = 0;
    //       num_gcode++;
    //       if(buf_len >= 1000) {
    //         buf_len = 0;
    //         break;
    //       }
    //       delay(300);
    //     }
    //   }
    // }
    // for(int i=0;i<16;i++) Serial.println(buffer[i], HEX);
  if(myFile){
    
  } else {
    Serial.println("error opening test.txt");
  }
}

void loop() {
  if (myFile) {
    if(myFile.available()){
      while(myFile.available()){
        char tmp_read = myFile.read();
        gcode_buf[gcode_index][buf_index++] = tmp_read;
        if(tmp_read == '\n'){
          Serial.print(gcode_buf[gcode_index]);
          buf_index = 0;
          gcode_index++;
          if(gcode_index == 50) gcode_index = 0;
          delay(1000);
        }
      }
    }
  }
  else{
    myFile.close();
  }
}