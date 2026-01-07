# A web-friendly WASM build of jpegli

## Build steps (Linux, tested on Ubuntu)

* Install build tools dependencies:

```bash
sudo apt install cmake doxygen ninja-build libgimp2.0-dev
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
./emsdk install latest

# Make the "latest" SDK "active" for the current user. (writes ~/.emscripten file)
./emsdk activate latest
```

* Clone this repo and install its dependencies:

```bash
cd $OPT

# Get the jpegli repo.
git clone https://github.com/apenchev/jpegli.git

# Enter that directory.
cd jpegli

# Download and install dependencies.
sudo ./deps.sh
```

* Each time before building jpegli, you need to login as root and set up the emsdk environment:

```bash
cd $OPT

# Login as root
sudo su

# Set up emsdk environment.
source emsdk/emsdk_env.sh

# Enter the jpegli directory.
cd jpegli

# Build jpegli.
BUILD_TARGET=wasm32 ENABLE_WASM_SIMD=1 SKIP_TEST=1 emconfigure ./ci.sh release
```

* Once the build is finished, you can log out from root:

```bash
exit
```

* You should get both cjpegli.wasm and cjpegli.js inside of the /build-wasm32/tools/ folder
* You can copy cjpegli.wasm and cjpegli.js to the /wasm_demo/ folder and test the encoder by running /wasm_demo/index.html
* index.html MUST RUN BE ON A LOCAL SERVER (http://localhost/), otherwise the browser content policy will refuse to load the .wasm file
