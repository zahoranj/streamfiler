# streamfiler - TCP/IP stream fogadó és lemezre író alkalmazás
A streamfiler egy porton figyelő alkalmazás, ami a csatlakozott connection tartalmát lemezre írja. A kiírt fájl neve megegyezik az érkezési időponttal (milisecundum pontossággal). Egyszerre több connection-t tud fogadni paraméterezéstől függően. *(Előkészületben: Ha .bmp vagy .png adattartalmat érzékel a kapcsolaton, akkor ugyanolyan filenévvel és .bmp kiterjesztéssel egy hisztogram képet hoz létre belőle extraként)*
## Fordítási és futtatási környezet
A program használatához **64 bites Linux** operációs rendszer, fordításához a GCC/**g++** fordító **C++11**-et támogató verziója (4.8.1-től felfelé) szükséges.
## Fordítás menete
Fordítása a program főkönyvtárából a mellékelt `Makefile` segítségével a `make` parancs kiadásával történhet:
```
make
```
## Futtatás menete
Fordításkor a `dist` könyvtárban keletkezik egy `streamfiler` nevű futtatható állomány, ez indítandó a leírásának megfelelően.
További infó a `-h` kapcsolóval indítva:
```
streamfiler -h
```
## Tesztelés
Tesztelni legegyszerűbben az `nc` (https://hu.wikipedia.org/wiki/Netcat) programmal lehet (*nc* vagy *netcat* csomag, a legtöbb linuxon megtalálható) tetszőleges fájl a figyelt portra küldve:
```
nc localhost 12345 < sample.png
```
## Dokumentálás
A **doxygen** (https://github.com/doxygen) programmal lehet egy áttekintést adó dokumentációt készíteni a kódról. Telepíteni kell hozzá a *doxygen* és a *graphviz* csomagokat.
A doxygen futtatása a program főkönyvtárából a mellékelt `Doxyfile` segítségével a `doxygen` parancs kiadásával történhet:
```
doxygen
```
A futtatás eredményének megtekintéséhez a `dox` könyvtárba létrejövő állományokból az `index.html`-t kell megnyitni
