#!/bin/bash


# have to run the emcc command like this, with the full path. not sure why system isn't seeing it as an execuatble, or in the path.
# /home/eischeid/web-assembly/emsdk/fastcomp/emscripten/emcc.py test-wasm.cpp -s WASM=1 -o hello.html

# generate with html template
#  /home/eischeid/web-assembly/emsdk/fastcomp/emscripten/emcc.py -o prng-wasm.html mt19937-64.c prng-utils-mt19937.c -O3 -s WASM=1  --shell-file templates/shell_minimal2.html -s NO_EXIT_RUNTIME=1 -s MALLOC='dlmalloc' -s FILESYSTEM=0 -s MODULARIZE=1 -s EXPORT_ES6=1 -s ENVIRONMENT='web' -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']"



# use default html generation
#  /home/eischeid/web-assembly/emsdk/fastcomp/emscripten/emcc.py -o prng-wasm.html mt19937-64.c prng-utils-mt19937.c -O3 -s WASM=1  -s EXPORT_NAME='PrngEnscriptenModule' -s NO_EXIT_RUNTIME=1 -s MALLOC='dlmalloc' -s FILESYSTEM=0 -s MODULARIZE=1 -s EXPORT_ES6=1 -s ENVIRONMENT='web' -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']"


# NOTE: for settings options see: https://github.com/emscripten-core/emscripten/blob/incoming/src/settings.js

echo "- compiling to wasm file"

# use default js only generation
# It seems -O3 optimization may accidentally slice _malloc() from generated code, so we use 02 here
  /home/eischeid/web-assembly/emsdk/fastcomp/emscripten/emcc.py -o prng-wasm.js mt19937-64.c prng-utils-mt19937.c -O1 -s WASM=1 -s EXPORT_NAME='PrngEnscriptenModule' -s NO_EXIT_RUNTIME=1 -s MALLOC='dlmalloc' -s FILESYSTEM=0 -s MODULARIZE=1 -s EMIT_EMSCRIPTEN_METADATA=1 -s EXPORT_ES6=1 -s ENVIRONMENT='web' -s FILESYSTEM=0 -s EXPORTED_FUNCTIONS="['_main', '_malloc']"  -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']"

echo "  - Done"

# -s EXPORTED_FUNCTIONS="['_main', '_malloc']".


 # /home/eischeid/web-assembly/emsdk/fastcomp/emscripten/emcc.py -o prng-wasm.wasm mt19937-64.c prng-utils-mt19937.c -O3 -s -s WASM=1 -s NO_EXIT_RUNTIME=1  -s EMIT_EMSCRIPTEN_METADATA=1

echo "- converting wasm to javascript byte array"
node wasm-to-bytes.js prng-wasm.wasm
