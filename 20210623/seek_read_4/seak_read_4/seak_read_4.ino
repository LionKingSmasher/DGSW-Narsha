#include <SPI.h>
#include <SD.h>

enum{
  NOT_FULL = 0,
  FULL
};

enum{
  EMPTY = 0,
  NOT_EMPTY
};

#define BUFFER_LEN 1024
#define GCODE_BUF_LEN 100

File myFile;

char buffer[1024];
char gcode_buf[GCODE_BUF_LEN][50];
int gcode_index = 0;
int gcode_buf_head = 0;
int gcode_buf_tail = -1;
int buf_index = 0;
int num_gcode = 0;
unsigned int buf_total_len = 0;
unsigned int buf_len = 0;
char gcode_buf_full = 0;

char buf_full_or_not(){
  if(gcode_buf_head == gcode_buf_tail){

  }
  if(gcode_buf_head > gcode_buf_tail){
    int tmp_buf_len = GCODE_BUF_LEN - (gcode_buf_head - gcode_buf_tail);
    if(tmp_buf_len == 0) return NOT_FULL;
    else if(tmp_buf_len > 0) return FULL;
  }
  else if(gcode_buf_head < gcode_buf_tail) {
    int tmp_head = gcode_buf_head + GCODE_BUF_LEN;
    int tmp_buf_len = GCODE_BUF_LEN - (tmp_head - gcode_buf_tail);
    if(tmp_buf_len > 0) return NOT_FULL;
    else return FULL;
  }
}

char buf_is_empty(){
  if(gcode_buf_head == gcode_buf_tail)
    return EMPTY;
  else
    return NOT_EMPTY;
}

char buf_is_full(){
  int tmp_buf_len = gcode_index;
}

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

int buff_free_num(){
  int temp_buf_len;
  if(gcode_buf_head >= gcode_buf_tail)
    temp_buf_len = GCODE_BUF_LEN - (gcode_buf_head - gcode_buf_tail);
  else if(gcode_buf_tail > gcode_buf_head)
    temp_buf_len = (GCODE_BUF_LEN + gcode_buf_head) - gcode_buf_tail;
  return temp_buf_len;
}

void loop() {
  if (myFile) {
    if(myFile.available()){
      int num_of_free_buff = buff_free_num();
      Serial.println(num_of_free_buff);
      if(num_of_free_buff > 1){
        if(gcode_buf_full == 0){
          while(myFile.available()){
            char tmp_read = myFile.read();
            gcode_buf[gcode_buf_head][buf_index++] = tmp_read;
            if(tmp_read == '\n'){
              String print_buf = "[" + String(gcode_buf_head) + "]" + gcode_buf[gcode_buf_head];
              // Serial.print(gcode_buf[gcode_index]);
              Serial.println(print_buf);

              buf_index = 0;

              gcode_buf_head++;
              if(gcode_buf_head == GCODE_BUF_LEN) gcode_buf_head = 0;
              if(gcode_buf_head == gcode_buf_tail){
                gcode_buf_full = 1;
                gcode_buf_head++;
                if(gcode_buf_head) 
                  gcode_buf_head = 0;
              }
              delay(10);
            }
          }
        }
      }
    }
  }
  else{
    myFile.close();
  }

  if(buf_is_empty() == NOT_EMPTY){
    String print_buf = "[" + String(gcode_buf_head) + "]" + gcode_buf[gcode_buf_head];
    // Serial.print(gcode_buf[gcode_index]);
    Serial.println(print_buf);
  }
}