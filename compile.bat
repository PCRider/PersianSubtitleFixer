set QTDIR=C:\Qt\5.11.1\mingw53_32
set PATH=%QTDIR%\bin;C:\Qt\Tools\mingw530_32\bin;C:\Qt\Tools\QtCreator\bin;%PATH%

qmake.exe PersianSubtitleFixer.pro -spec win32-g++ "CONFIG+=qtquickcompiler"
mingw32-make.exe 
windeployqt --qmldir . ./release/PersianSubtitleFixer.exe
cd release
del *.o
del *.h
del *.cpp
cd ..
qmake.exe PersianSubtitleFixer.pro -spec win32-g++ "CONFIG+=debug" "CONFIG+=qml_debug"
mingw32-make.exe 
windeployqt --qmldir . ./debug/PersianSubtitleFixer.exe
cd debug
del *.o
del *.h
del *.cpp
cd ..