@echo off
if not exist %1.c goto hey
%WATCOM%\binw\wcc386 -d2 %1.c
rem wcc386 -oatix %1.c
if not exist %1.err goto success
goto fin
:hey
echo %1.c does not exist ?!
goto fin
:success
%WATCOM%\binw\wlink debug all f %1 sys pmodew
:fin
