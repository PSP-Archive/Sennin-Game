make
goto EXIT


c:\ps2dev\gcc\ee\bin\ee-gcc -march=r4000 -O3  -mgp32 -mlong32 -c main.c
c:\ps2dev\gcc\ee\bin\ee-gcc -march=r4000 -O3  -mgp32 -mlong32 -c pg.c
c:\ps2dev\gcc\ee\bin\ee-gcc -march=r4000 -O3  -mgp32 -mlong32 -c font.c
c:\ps2dev\gcc\ee\bin\ee-gcc -march=r4000 -O3  -mgp32 -mlong32 -c utility.c
c:\ps2dev\gcc\ee\bin\ee-gcc -march=r4000 -O3  -mgp32 -mlong32 -c _clib.c
c:\ps2dev\gcc\ee\bin\ee-gcc -march=r4000 -O3  -mgp32 -mlong32 -c game.c
c:\ps2dev\gcc\ee\bin\ee-gcc -march=r4000 -O3  -mgp32 -mlong32 -c enemy.c
c:\ps2dev\gcc\ee\bin\ee-gcc -march=r4000 -O3  -mgp32 -mlong32 -c sennindata.c
c:\ps2dev\gcc\ee\bin\ee-gcc -march=r4000 -O3  -mgp32 -mlong32 -c sound.c
c:\ps2dev\gcc\ee\bin\ee-gcc -march=r4000 -O3  -mgp32 -c -xassembler -O -o startup.o startup.s
c:\ps2dev\gcc\ee\bin\ee-ld -O0 startup.o main.o utility.o pg.o game.o enemy.o sennindata.o sound.o _clib.o font.o -M -Ttext 8900000 -q -o out > hellopsp.map
outpatch
@echo you got outp as psp elf
elf2pbp outp "Sennin Game 2005"


:exit
