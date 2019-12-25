# ESP32-CAM Beer tap
ÉSP32-CAM module based counter of tapped beers with picture

## Instructions for Zhoufik
Připojit senzor průtoku na hadici, kteoru bude pivo protékat a hadici, která na senzor již byla připojená, připojit do výčepu. 
Přidělat samotnou krabičku (ductapou, fofrpáskama) tak, aby byla 2 až 3 metry od výčepu a směřovala k němu fotosenzorem.
Vhodnost umístění senzoru ověříme tím, že nasimulujeme točení piva a při tomto úkonu by se fotosenzor měl na nás "dívat".
Poslední krok je propojení zdroje (černý kabel) do elektrické sítě.
Funkčnost zařízení ověříme tím, že zkontrolujeme, zda LED po stranách krabičky slabě červeně svítí.

## Toubleshooting
Pokud spoj hadice-senzor protéká, tak spoj upevníme fofrpáskama.

![senzor průtoku](https://raw.githubusercontent.com/Wixi18/esp32-cam-beer-tap/master/flow_sensor.JPG)
Senzor průtoku
