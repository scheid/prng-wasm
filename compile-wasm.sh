#!/bin/bash


# have to run the emcc command like this, with the full path. not sure why system isn't seeing it as an execuatble, or in the path.
# /home/eischeid/web-assembly/emsdk/fastcomp/emscripten/emcc.py test-wasm.cpp -s WASM=1 -o hello.html

# /home/eischeid/web-assembly/emsdk/fastcomp/emscripten/emcc.py -o prng-wasm.html mt19937-64.c prng-utils-mt19937.c -O3 -s WASM=1  --shell-file templates/shell_minimal2.html -s NO_EXIT_RUNTIME=1  -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']"

# /home/eischeid/web-assembly/emsdk/fastcomp/emscripten/emcc.py -o prng-wasm.wasm mt19937-64.c prng-utils-mt19937.c -Os -s WASM=1 -s NO_EXIT_RUNTIME=1 

emcc -o prng-wasm.wasm mt19937-64.c prng-utils-mt19937.c -Os -s WASM=1 -s NO_EXIT_RUNTIME=1 


#  -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']"

#  -s MODULARIZE=1

