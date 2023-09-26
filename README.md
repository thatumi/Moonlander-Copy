# Prog-1-Hazi---Moonlander-

https://github.com/thatumi/Prog-1-Hazi---Moonlander-/assets/49487499/d7aa1f32-e95a-419c-aa39-70ebf6a63111

Egy Moonlander koppintás, amit Programozás alapjai 1 (BME-VIK) házinak adtam be. SDL 2 megléte kell az elindításhoz!!!

## Játékszabályok: 
 A játékban a felhasználó egy űrhajót irányít. A játék célja, hogy a játékos 
megfelelően le tudjon szállni, miközben az űrhajóját navigálja. A sikeres leszállás 
feltételei a következők: 
 - Nem szabad ferde terepre leszállni. 
- Bekapcsolt hajtóművel nem szabad túl közel kerülni a talajhoz. 
- A leérkezés pillanatában nem lehet túl nagy a vízszintes irányú sebesség 
- A leérkezés pillanatában nem lehet túl nagy a függőleges irányú sebesség. 
- Ha elfogy az üzemanyag, a holdkomp lezuhan. 



## Nézetek: 
 Két fajta nézet van, a főmenü, ahol új játékot lehet indítani, vagy kilépni a 
programból, és maga a játéknézet. A játéknézetbe a főmenüből, az új játék 
indításával lehet eljutni. A játék indulásakor eleve a főmenübe érkezünk, illetve az 
elkezdett játék végén is ide jutunk el. 
## Pálya: 
 Játék által generált 2D-s pályák vannak. Ezek a pályák hepehupások, viszont 
minden esetben van egy leszállásra alkalmas sík felület. 
Mentés: 
 A játékban nincs mentés, mivel a játékmenet hossza miatt nem lenne értelme. 
## Irányítás/Bemenet: 
 Az űrhajót a jobbra, balra fel nyilakkal és a szóközzel lehet irányítani. A szóköz 
a hajtóművet kikapcsolja, a felfele nyíl lenyomásával a hajtómű erősebben visz 
felfele. A jobbra balra nyilakkal az oldalirányú gyorsulást lehet változtatni. 
## HUD: 
 A játéknézetben jobb felül helyezkedik el az űrhajó műszerfala, ahol láthatjuk 
az űrhajó magasságát, az x és y irányú gyorsaságát, az üzemanyagszintet, valamint 
a kontaktfényt, ami jelzi, hogy elég közel vagyunk-e már a talajhoz, hogy a 
hajtóművet ki lehessen kapcsolni.
