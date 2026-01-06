# A web-friendly WASM build of jpegli

This repository contains a JPEG encoder and decoder implementation that is
API and ABI compatible with libjpeg62.

## Usage

Building stpes Ubuntu/Debian (including WSL2 on Windows):

* Install dependencies by running:
  
      sudo ./deps.sh

* Install CMake and Emscripten SDK by following the instructions on https://github.com/apenchev/jpegli/blob/main/doc/building_wasm.md but <ins>DO NOT RUN</ins> the last step in the docs:

      BUILD_TARGET=wasm32 emconfigure ./ci.sh release

      BUILD_TARGET=wasm32 ENABLE_WASM_SIMD=1 emconfigure ./ci.sh release

* Instead, you first need to log on as root with:

      sudo su

* Onced logged on as root, you need to configure the Emscripten SDK again with:

      source $OPT/emsdk/emsdk_env.sh

* Finally, you can run the build step:

      BUILD_TARGET=wasm32 ENABLE_WASM_SIMD=1 SKIP_TEST=1 emconfigure ./ci.sh release

* You should get both cjpegli.wasm and cjpegli.js inside of the /build-wasm32/tools/ folder

* You can copy cjpegli.wasm and cjpegli.js inside of the /wasm_demo/ folder and test if the encode works by running /wasm_demo/index.html ON A SERVER - if not running on a local server (such as Apache), the browser content policy will refuse to load the .wasm
