

cd bin

master.exe -n 10 5 2 3


rem EQU - equal
rem NEQ - not equal
rem LSS - less than
rem LEQ - less than or equal
rem GTR - greater than
rem GEQ - greater than or equal

rem pause
:step

player.exe
master.exe -c

rem pause

IF %ERRORLEVEL% GEQ 10 GOTO :step
