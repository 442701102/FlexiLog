mkdir ..\build\win64
pushd ..\build\win64
cmake -G "Visual Studio 16 2019" -A x64 ../..
cmake --build . --config Release
cmake --install . --config Release
popd