mkdir ..\build\win32
pushd ..\build\win32
cmake -G "Visual Studio 16 2019" -A Win32 ../..
cmake --build . --config Release
cmake --install . --config Release
popd