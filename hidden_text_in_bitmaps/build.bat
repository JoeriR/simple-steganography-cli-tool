C:\\mingw-w64\\x86_64-8.1.0-win32-seh-rt_v6-rev0\\mingw64\\bin\\g++ -static-libgcc -static-libstdc++ -o htbwrite writer.cpp
C:\\mingw-w64\\x86_64-8.1.0-win32-seh-rt_v6-rev0\\mingw64\\bin\\g++ -static-libgcc -static-libstdc++ -o htbread reader.cpp
htbwrite.exe "grey_square.bmp" "extra hidden text"
htbread.exe "grey_square_hidden.bmp"