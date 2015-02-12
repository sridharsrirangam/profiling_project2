@echo off
set PATH=%PATH%;C:\cygwin64\bin
@echo on
getregions.exe ../obj/*.axf -c -o../src/region.c