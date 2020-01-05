# ESP32-cam (Ai-Thinker) beer tap
Counter of content of tapped beer with picture taking saving on SD card based on ESP32-CAM module.
It's solution for parties where people are tapping own beers (and there is no Wi-Fi - but this feature is on TO DO list)

## Functions
Every time someone starts to tap a beer is taken picture and it's counting litres of tapped beer. When the float stops (there are no more impulses from sensor with possible pause 1 second) then the name of taken picture file and content (in litres) are saved in logs.txt file on SD card. 

## HW
* ESP32-CAM (Ai-Thinker) board
* YF-S401 Flow sensor
* 4GB microSD card (4GB is officialy supported, but 16GB worked as well)
* some (at least 8) dupont wire 
* FTDI programmer
* case: https://www.thingiverse.com/thing:3406029 

## Toubleshooting
Manual for Push-fit tubes: http://download.svet-piva.cz/navod-rychlospojky.pdf  
If the beer is leaking around the connection tube-sensor, than use cable tie (fofrpásky)  
If there is camera error, than use 2A power supply, it should solve the problem

![how to connect flow sensor](https://raw.githubusercontent.com/Wixi18/esp32-cam-beer-tap/master/howToConnectFlowSensor.png)

![in action](https://raw.githubusercontent.com/Wixi18/esp32-cam-beer-tap/master/esp32_cam_inAction.jpg)

