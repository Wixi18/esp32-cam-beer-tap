#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "fd_forward.h"
#include "fr_forward.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "dl_lib.h"
#include "driver/rtc_io.h"
#include <EEPROM.h>            // read and write from flash memory

#include "FS.h"
#include "SPI.h"


// define the number of bytes you want to access
#define EEPROM_SIZE 1

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


#define LED_BUILTIN 4

int pictureNumber = 0;

String picturePath;

#define SENSOR  16
#define ESP_INTR_FLAG_DEFAULT 0

long runningTime = 0;
long executeTime = 0;
int pulseCount = 0;
int oldPulseCount = 0;



static void IRAM_ATTR pulseCounter(void * arg) {
  pulseCount++;
}



void takePicture(){
  
 camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 


  
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }


   
  
  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  camera_fb_t * fb = NULL;
  
  // Take Picture with Camera
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera capture failed");
    return;
  }


  
  
  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  pictureNumber = EEPROM.read(0) + 1;

  // Path where new picture will be saved in SD Card
  String path = "/picture" + String(pictureNumber) +".jpg";

  picturePath = path;

  fs::FS &fs = SD_MMC; 
  Serial.printf("Picture file name: %s\n", path.c_str());
  
  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file in writing mode");
  } 
  else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Saved file to path: %s\n", path.c_str());
    EEPROM.write(0, pictureNumber);
    EEPROM.commit();
  }
  file.close();
  esp_camera_fb_return(fb); 

   // DE-Init Camera
  esp_camera_deinit();
 
  
  
  
  }


double beerCount(){ //if not new counts of pulses (with 1 second space to check for more pulses)
    runningTime = millis();
    pulseCount = 1;
    oldPulseCount = 1;

    executeTime = runningTime;

    while ((executeTime + 1000) > runningTime ){ // if there is no longer delay than 1 sec between pulses -> keep counting
      runningTime = millis();

      if (oldPulseCount < pulseCount){
          executeTime = runningTime;
          oldPulseCount = pulseCount;
      }

    }

    float toLitres = 0;
    int pulseConstant = 5880; //YF-S401 send 5880 pulses for 1 litre
    toLitres = (float)pulseCount / (float)pulseConstant;

    return toLitres;

}




void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}



void setup(){
  Serial.begin(115200);
  Serial.print("-----------------Up And Running------------------");
  
  Serial.print("\n");
  pinMode(SENSOR, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, HIGH);
  
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  
  gpio_isr_handler_add(GPIO_NUM_16, &pulseCounter, (void *) 16);
  
  gpio_set_intr_type(GPIO_NUM_16, GPIO_INTR_POSEDGE);


  

  
  //Serial.println("Starting SD Card");



  
  if(!SD_MMC.begin()){
    Serial.println("SD Card Mount Failed");
    return;
  }
  
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    return;
  }

  Serial.println("REBOOT write to logs.txt");
  appendFile(SD_MMC, "/logs.txt", "REBOOT \n");


}

void loop(){
  

  if (pulseCount > 0 ){

     takePicture();
     Serial.print("\n");
     Serial.print(picturePath);
     Serial.print("\n");

     double countedBeer = beerCount();
     
     Serial.print(countedBeer);
     Serial.print(" l");
     Serial.print("\n");


     
     String toWrite = String(countedBeer, 4) + "," + picturePath + ";\n";
     
     
     
     
     appendFile(SD_MMC, "/logs.txt", toWrite.c_str());
     pulseCount = 0;

     
  }




  


}
