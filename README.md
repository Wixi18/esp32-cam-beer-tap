# About
Counter of content of tapped beer with picture taking saving on SD card based on ESP32-CAM module.
It's solution for parties where people are tapping own beers (and there is no Wi-Fi)

## Functions
Every time someone starts to tap a beer is taken picture and it's counting litres of tapped beer. When the float stops (there are no more impulses from sensor with possible pause 1 second) then the name of taken picture file and content (in litres) are saved in logs.txt file on SD card. 

## Instructions for Zhoufik
* Připojit senzor průtoku na hadici, kteoru bude pivo protékat a hadici, která na senzor již byla připojená, připojit do výčepu. 
* Přidělat samotnou krabičku (ductapou, fofrpáskama) tak, aby byla 2 až 3 metry od výčepu a směřovala k němu fotosenzorem.
* Vhodnost umístění senzoru ověříme tím, že nasimulujeme točení piva a při tomto úkonu by se fotosenzor měl na nás "dívat".
* Poslední krok je propojení zdroje (černý kabel) do elektrické sítě.
* Funkčnost zařízení ověříme tím, že zkontrolujeme, zda LED po stranách krabičky slabě červeně svítí.

## Toubleshooting
Pokud spoj hadice-senzor protéká, tak spoj upevníme fofrpáskama.

![senzor průtoku](https://raw.githubusercontent.com/Wixi18/esp32-cam-beer-tap/master/flow_sensor.JPG)
Senzor průtoku
