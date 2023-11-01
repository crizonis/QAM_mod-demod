mkdir build
mkdir build/tmp
cd build/tmp
cmake ../../
cmake --build ./ --config Release
mv ./Release/main.exe ../
cd ../
rm -rf tmp