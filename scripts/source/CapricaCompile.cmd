@echo off

rem skk.50@outlook.com September 2023
rem CapricaCompile.cmd

set IMPORT="C:\Steam\steamapps\common\Skyrim Special Edition\Data\Scripts\Source"
set OUTPUT=E:\Data\EstrusForSkyrimNG\scripts
set FLAGS="C:\Steam\steamapps\common\Skyrim Special Edition\TESV_Papyrus_Flags.flg"
set SCRIPTPATH=E:\Data\EstrusForSkyrimNG\scripts\source
set SCRIPTNAME=MyDefaultScript.psc

rem Notepad++ needs current working directory to be where Caprica.exe is 
cd "%SCRIPTPATH%"

rem %1 is the first command line parameter passed which will override hardcoded SCRIPTNAME Notepad++ passes $(FILE_NAME)
if [%1] == [] goto START
set SCRIPTNAME=%1

:START
cls
echo ****************************************************************
echo Caprica Starfield DEBUG compile 002
echo.

if [%SCRIPTNAME%] == [] goto GETSCRIPTNAME
goto COMPILE

:GETSCRIPTNAME
 set /p SCRIPTNAME="SCRIPTFILE Name (include.psc): "
 echo.

:COMPILE
rem set SCRIPTFILE=%SCRIPTPATH%\%SCRIPTNAME%
 set SCRIPTFILE=%SCRIPTNAME%

 echo IMPORT:     "%IMPORT%"
 echo OUTPUT:     "%OUTPUT%"
 echo FLAGS:      "%FLAGS%"
 echo SCRIPTPATH: "%SCRIPTPATH%"
 echo SCRIPTNAME: "%SCRIPTNAME%"
 echo SCRIPTFILE: "%SCRIPTFILE%" 
 echo.

 caprica.exe --game starfield --import "%IMPORT%" --flags "%FLAGS%" --output "%OUTPUT%" "%SCRIPTFILE%" 

:END
 echo.
 pause
