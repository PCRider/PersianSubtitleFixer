set QTDIR=C:\Qt\5.11.1\winrt_x64_msvc2017
set PATH=%QTDIR%\bin;C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build;C:\Qt\Tools\QtCreator\bin;%PATH%
call qtenv2
call vcvarsall.bat x86_amd64
cd %~dp0
qmake.exe PersianSubtitleFixer.pro -spec win32-msvc "CONFIG+=debug" "CONFIG+=qml_debug"
jom.exe
qmake.exe PersianSubtitleFixer.pro -spec win32-msvc "CONFIG+=release" "CONFIG+=qml_release"
jom.exe