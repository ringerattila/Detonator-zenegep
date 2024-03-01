# Detonator

##Zenegép
PC-alapú internetes zenegép. (2004.-től fejlesztve, 2018-tól "internetesítve"). A PC-n Delphi nyelven írt program fut, soros porton kommunikál a kiegészítő hardverrel, a zenelejátszás Winamp-pal történik. Az adatbázis MySql-alapú, a Delphi a Lazarus csomag segítságével éri el.
##Mikrovezérlő
A kiegészítő hardver végzi a pénzkezelést, a nyomógombok kezelését, és a számítógéptől függetlenül kezeli a számlálókat. Microchip mikrovezérlőt tartalmaz, C-nyelven készült a firmware.
##Mobilapplikáció
A zenegéphez mobil applikáció (PWA) tartozik. Egy YouTube-on kiválasztott zenét meg lehet osztani az applikációval (megjelenik a megosztási célok között), és tetszőleges zenegépre be lehet küldeni a zenét. A mobilalkalmazás az arra jogosultak számára lehetőséget biztosít a zenegép teljes távfelülgyeletére, beállításainak kezelésére.
A rendszer lelke PHP-alapú (CodeIgniter) backend szerver webalkalmazás, ami a zenegépekkel is kommunikál (API, MySQL), és a mobilalkalmazással is.
