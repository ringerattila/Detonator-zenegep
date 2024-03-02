# Detonator

## Zenegép
PC-alapú internetes zenegép. (2004.-től fejlesztve, 2018-tól "internetesítve"). A PC-n Delphi nyelven írt program fut, RS-232 soros porton kommunikál a kiegészítő hardverrel, a zenelejátszás Winamp-pal történik. A zenei szerver-adatbázis MySql-alapú, a Delphi a Lazarus csomag segítségével éri el.
## Mikrovezérlő
A kiegészítő hardver végzi a pénzkezelést, a nyomógombok kezelését, és a számítógéptől függetlenül kezeli a számlálókat. Microchip mikrovezérlőt tartalmaz, C-nyelven készült a firmware.
## Mobilapplikáció
A zenegéphez mobil applikáció (PWA) tartozik. Egy YouTube-on kiválasztott zenét meg lehet osztani az applikációval (megjelenik a megosztási célok között), és tetszőleges zenegépre be lehet küldeni a zenét. A mobilalkalmazás az arra jogosultak számára lehetőséget biztosít a zenegép teljes távfelülgyeletére, beállításainak kezelésére.
## Backend webalkalmazás
A rendszer lelke PHP-alapú (CodeIgniter) backend szerver webalkalmazás, ami a zenegépekkel is kommunikál (API, MySQL), és a mobilalkalmazással is. Kezeli a zenegép tulajdonosok adatait (ügyféltörzs), a rendszer felhasználóit (felhasználótörzs), a zenegépek beállításai, naplózásait, és üzemeltetési adatait is. Nyilvántartja a zenegépekhez rendelt zenei albumokat, a YouTube-ról beküldött zeneszámok adatait, és az aktuális zenei ranglistákat.
