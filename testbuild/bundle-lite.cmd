@echo off
cd ..
call build.bat x86 Release Main

cd testbuild
copy ..\bin\mpc-hc_x86\mpc-hc.exe .

robocopy "..\bin\mpc-hc_x86\Lang" "Lang" /S /XO

7za a mpc-hc.7z mpc-hc.exe
pause