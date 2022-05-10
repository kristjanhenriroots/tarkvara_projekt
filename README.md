# tarkvara_projekt
# FOR GUI VERSION SEE JÖRGEN BRANCH
TalTech Tarkvara Projekt 2022

Hi,

This is the repo for the Software Project course at TalTech for 2022 and its only here to hold and share code more conveniently.
It is intended to be a study project exclusively, therefore things can very likely be broken and there is no guarantee of it working at all :P

The final goal is to create a maze creating and solving program with a simple UI using different algorithms, timing and comparing them to eachother.

To run the code it is mandatory to download all .c files and compile them together (see the suggested command below, but be wary, the final filenames 
and file count is subject to change and needs an according change to the command).

Supported features:
  * Creating mazes using 3 different generation algorithms including imperfect ones
  * Supported sizes 7 - 499 (can exceed at your own risk)
  * Solving mazes with 3 different algorithms, comparing the solution times and path lenghts
  * Saving mazes in TXT, SVG and BMP file formats
  * Reading mazes in TXT and SVG file formats
  * Customizing bitmaps with 14 different colors and fades between any of them in a selected direction (Completely random colors supported now as well)

Tere,

See on TalTech aine Tarkvara projekt aine raames loodud repo, et mugavamalt koodi hoida ja omavahel jagada.
Projekt on loodud õpieesmärgil, mistõttu võivad olla asjad katki või üldse mitte töötada.

Loodud programmi lõppeesmärk on luua lihtsa kasutajaliidesega labürintide loomise ning lahendamise programm, kasutades
erinevaid algoritme ning võrrelda neid omavahel.

Programmi kasutamiseks on vajalik alla tõmmata kõik .c programmifailid ning kompileerida need kokku

Terminali käsk näeb välja midagi sellist:

```shell
gcc main.c ellermaze.c treemaze.c dead.c findExits.c rec.c manageFiles.c mazeBMP.c bfs.c -o maze -Wall
```

NB! Failinimed ning nende arv võib muutuda, käsku tuleb muuta vastavalt.
