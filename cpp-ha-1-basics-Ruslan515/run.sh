rm -rf build; mkdir build; cd build
cmake ..
cmake --build .
ctest -R $1
