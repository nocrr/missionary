@echo off
if not exist main_mus.c goto hey
wcc386 -ohatmirpk -s -fp5 -ie:\watcom\midas\src\midas main_mus.c

rem wcc386 -oaxtenh -ms -s -fp5 -i\nocrr\midas\src\midas main_mus.c
rem %WATCOM%\binw\wpp386 -oahprktbi -s -fp3 %1.c
if not exist main_mus.err goto success
goto fin
:hey
echo main_mus.c does not exist ?!
goto fin
:success
wlink f main_mus sys pmodew library e:\WATCOM\midas\lib\dos\wcretail\midas.lib
:fin
