@echo off
if not exist %1.c goto hey
rem wcc386 -od -s %1.c
rem wcc386 %1.c
rem wcc386 -ohatmirpk -s %1.c
rem wcc386 -oatix %1.c
wcc386 -ohatmirpk -s -fp5 main.c
if not exist %1.err goto success
del %1.err
rem del %1.obj
goto fin
:hey
echo %1.c does not exist ?!
goto fin
:success
wlink f %1 sys pmodew
rem del %1.obj
:fin
