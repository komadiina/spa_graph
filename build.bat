@echo off
set filename=%1

echo Building file: %filename%.cpp (%filename%.exe), args = { -g, -std=c++23, -Iincl/ }
g++ %filename%.cpp -o %filename%.exe -g -std=c++23 -Iincl/
echo Finished.