#include <SPI.h>
#include <SD.h>

enum{
  NOT_FULL = 0,
  FULL
};

enum{
  NOT_EMPTY = 0,
  EMPTY
};

File myFile;

#define BUFFER_LEN  1024
#define GCODE_BUF_LEN  100

char buffer[BUFFER_LEN];
char gcode_buf[GCODE_BUF_LEN][50];
int buf_index = 0;
int gcode_index = 0;
int gcode_buf_head = 0;
int gcode_buf_tail = 0;
int free_buf_num = GCODE_BUF_LEN;
int can_use_buff = 0;
unsigned int buf_total_len = 0;
unsigned int buf_len = 0;
int num_gcode = 0;
char gcode_buf_full = 0;

char buf_full_or_not()
{
  if(gcode_buf_head == gcode_buf_tail)
  {

  }
  else if(gcode_buf_head > gcode_buf_tail)
  {
    int tmp_buf_len = GCODE_BUF_LEN - (gcode_buf_head - gcode_buf_tail);
    if(tmp_buf_len == 1) return FULL;
    else if(tmp_buf_len > 1) return NOT_FULL;
  }
  else if(gcode_buf_head < gcode_buf_tail)
  {
    int tmp_head = gcode_buf_head + GCODE_BUF_LEN;
    int tmp_buf_len = GCODE_BUF_LEN - (tmp_head - gcode_buf_tail);
    if(tmp_buf_len > 0) return NOT_FULL;
    else return FULL;
  }

}

char buf_is_empty()
{
  if(gcode_buf_head == gcode_buf_tail)
    return EMPTY;
  else
    return NOT_EMPTY;
}

char buf_is_full()
{
  if(gcode_buf_head == gcode_buf_tail)
    return EMPTY;
  else
    return NOT_EMPTY;
}


int buff_free_num()
{
    int temp_buf_len;
    if(gcode_buf_head >= gcode_buf_tail) 
      temp_buf_len= GCODE_BUF_LEN - (gcode_buf_head - gcode_buf_tail);
    else if(gcode_buf_tail > gcode_buf_head)
      temp_buf_len= (GCODE_BUF_LEN + gcode_buf_head) - gcode_buf_tail;

    return temp_buf_len;
}


void setup() {
  Serial.begin(115200);

  Serial.print("Initializing SD card...");
  if (!SD.begin(31)) {
    Serial.println("initialization failed!");
    while(1);
  }
  Serial.println("initialization done.");
  
  myFile = SD.open("gcode.gco");
  if (myFile) {


  } else {
    Serial.println("error opening test.txt");
  }
}

int tmp_toggle = 0;

void loop() {
    if (myFile)
    {
      if(myFile.available())
      {
        // num_of_free_buff = buff_free_num();
        // Serial.println(num_of_free_buff);

        if(free_buf_num > 0)
        {
          char tmp_read = myFile.read();
          gcode_buf[gcode_buf_head][buf_index++] = tmp_read;

          Serial.print("Head[" + String(gcode_buf_head) + "]: ");
          Serial.println(gcode_buf[gcode_buf_head]);

          if(tmp_read == '\n')
          {
            buf_index = 0;

          //  if(num_of_free_buff > 1)
            {
              gcode_buf_head++;
              free_buf_num = GCODE_BUF_LEN - gcode_buf_head;
              if(gcode_buf_head == GCODE_BUF_LEN) {
                gcode_buf_head = 0;
              }
            }




            // if(gcode_buf_head == gcode_buf_tail)
            // {
            //   gcode_buf_full = 1;
            //   gcode_buf_head--;
            //   if(gcode_buf_head < 0) gcode_buf_head = GCODE_BUF_LEN - 1;
            // }
            

            delay(1);
          }
        }
      }
      else
      {
        myFile.close();
      }
    }

    if(tmp_toggle == 1){
      tmp_toggle = 0;
      if(gcode_buf_head >= gcode_buf_tail){
        can_use_buff = gcode_buf_head - gcode_buf_tail;
      }
      else if(gcode_buf_head < gcode_buf_tail){
        can_use_buff = (GCODE_BUF_LEN + gcode_buf_head) - gcode_buf_tail;
      }

      if(can_use_buff > 0){
        Serial.print("Tail[" + String(gcode_buf_tail) + "]: ");
        Serial.println(gcode_buf[gcode_buf_tail]);
        gcode_buf_tail++;
        if(gcode_buf_tail == 100) while(1);
      }
    }
    else if(tmp_toggle == 0){
      tmp_toggle=1;
    }
    
    // if(buf_is_empty() == NOT_EMPTY)
    // {
    //   String print_buf = "[" + String(gcode_buf_head) + "]" + gcode_buf[gcode_buf_head];
    //   //Serial.print(gcode_buf[gcode_index]);
    //   Serial.print(print_buf);
    // }

}
