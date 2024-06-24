# 3D World Bedroom menggunakan C++ dan Libray Freeglut 

### Kebutuhan Dependencies
1. GCC versi 6 ke atas
2. Dev C++
3. Library Freeglut 3.0
4. Linker

### Konfigurasi Dependencies

1. Ekstrak library Freeglut 3.0

2. Copy semua file di direktori (freeglut\include\GL) paste ke direktori (C:\Program Files (x86)\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include\GL)

3. Copy semua file di direktori (freeglut\lib\x64) paste ke direktori (C:\Program Files (x86)\Dev-Cpp\MinGW64\x86_64-w64-mingw32\lib)

4. Copy semua file di direktori (freeglut\bin\x64) paste ke direktori (C:\Windows\System32)

5. Copy file stb_image.h ke direktori C:\Program Files (x86)\Dev-Cpp\MinGW64\x86_64-w64-mingw32\include

6. Buka folder projek di Dev C++

7. Setting linker di tab Project -> Project Options -> Parameters. Pada textbox Linker pastekan dengan:
-lopengl32
-lfreeglut 
-lglu32

8. Compile and Run
