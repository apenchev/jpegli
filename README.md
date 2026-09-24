# A web-friendly WASM build of jpegli

## Linux/WSL 2 build steps (tested on Ubuntu 26.04)

* Install build tools dependencies:

```bash
sudo apt install cmake ninja-build pkgconf
```

* [Emscripten SDK](https://emscripten.org/) is required for building
WebAssembly artifacts. To install it, follow the
[Download and Install](https://emscripten.org/docs/getting_started/downloads.html)
guide:

```bash
# Enter the home directory.
cd $OPT

# Get the emsdk repo.
git clone https://github.com/emscripten-core/emsdk.git

# Enter that directory.
cd emsdk

# Download and install the latest SDK tools.
./emsdk install latest

# Make the "latest" SDK "active" for the current user. (writes .emscripten file)
./emsdk activate latest
```

* Clone this repo and install its dependencies:

```bash
# Enter the home directory.
cd $OPT

# Get the jpegli repo.
git clone https://github.com/apenchev/jpegli.git

# Enter that directory.
cd jpegli

# Download and install dependencies.
./deps.sh
```

* Each time before building jpegli, you need to set up the emsdk environment beforehand:

```bash
# Enter the home directory.
cd $OPT

# Set up emsdk environment.
source emsdk/emsdk_env.sh

# Enter the jpegli directory.
cd jpegli

# Build jpegli.
BUILD_TARGET=wasm32 ENABLE_WASM_SIMD=1 SKIP_TEST=1 emconfigure ./ci.sh release
```

* Both `/build-wasm32/tools/cjpegli.wasm` and `/build-wasm32/tools/cjpegli.js` will be generated
* You can copy both files into the `/wasm_demo/` folder to start testing `cjpegli.wasm`
* Load `/wasm_demo/index.html` from a server (like http://localhost/wasm_demo/) or the browser's security policy will refuse to load *cjpegli.wasm*
