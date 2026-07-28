# A web-friendly WASM build of jpegli

## Linux/WSL build steps (tested on Ubuntu 26.04)

* Install build tools dependencies:

```bash
sudo apt install cmake ninja-build pkgconf
```

* [Emscripten SDK](https://emscripten.org/) is required for building
WebAssembly artifacts. To install it, follow the
[Download and Install](https://emscripten.org/docs/getting_started/downloads.html)
guide:

```bash
cd $OPT

# Get the emsdk repo.
git clone https://github.com/emscripten-core/emsdk.git

# Enter that directory.
cd emsdk

# Download and install the latest SDK tools.
./emsdk install 6.0.2

# Make the "latest" SDK "active" for the current user. (writes ~/.emscripten file)
./emsdk activate 6.0.2
```

* Clone this repo and install its dependencies:

```bash
cd $OPT

# Get the jpegli repo.
git clone https://github.com/apenchev/jpegli.git

# Enter that directory.
cd jpegli

# Download and install dependencies.
./deps.sh
```

* Each time before building jpegli, you need to login as root and set up the emsdk environment:

```bash
cd $OPT

# Set up emsdk environment.
source emsdk/emsdk_env.sh

# Enter the jpegli directory.
cd jpegli

# Build jpegli.
BUILD_TARGET=wasm32 ENABLE_WASM_SIMD=1 SKIP_TEST=1 emconfigure ./ci.sh release
```

* You should get both cjpegli.wasm and cjpegli.js inside of the /build-wasm32/tools/ folder
* You can copy cjpegli.wasm and cjpegli.js to the /wasm_demo/ folder and test the encoder by running /wasm_demo/index.html
* index.html MUST BE RUN ON A SERVER (such as http://localhost/), otherwise the browser's content policy will refuse to load the .wasm file
