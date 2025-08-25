
# Requirements
Please download the latest version of [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) and [CMake](https://cmake.org/download/). You will also need the latest version of gcc/g++ (>=c++20).

## Codespaces
Go [here](#codespace-setup) for a tutorial on how to build and run the game in the web-based version of Github Codespaces.

# Installation

## Native server (more performant):
```
> git clone --recurse-submodules https://github.com/trigonal-bacon/gardn.git
```
You will need to compile uWebSockets first. For in-depth complation options please visit the [uWebSockets installation page](https://github.com/uNetworking/uWebSockets/tree/master).
```
> cd gardn/Server/uWebSockets
> make
```
Then,
```
> cd gardn/Server
> mkdir build
> cd build
> cmake ..
> make
> ./gardn-server
```

## WebAssembly Server (doesn't require uWebSockets, but requires [Node.js](https://nodejs.org/en/download))
```
> git clone https://github.com/trigonal-bacon/gardn.git
> cd gardn/Server
> mkdir build
> cd build
> cmake .. -DWASM_SERVER=1
> make
> npm install ws fs http
> node ./gardn-server
```

## Client:
```
cd gardn/Client
mkdir build
cd build
cmake ..
make
```
Then move the outputted ``wasm`` and ``js`` files into Client/public (or optionally ``Server/build`` if you're running the wasm server; make sure to move the ``html`` file as well).

The server is served by default at ``localhost:9001``. You may change the port by modifying ``Shared/Config.cc``

# Codespace Setup
For Github Codespaces, we will use a WASM server. UWS works aswell, however I find myself sometimes having to install extra packages so we shall not use it.

Run this command to compile the Server and Client. In Codespaces, choose this repository. It should automatically have you in the `gardn` directory.
```bash
git clone https://github.com/emscripten-core/emsdk.git /workspaces/gardn/emsdk && cd /workspaces/gardn/emsdk && ./emsdk install latest && ./emsdk activate latest && source /workspaces/gardn/emsdk/emsdk_env.sh && cd .. && cd Server && mkdir build && cd build && cmake .. -DWASM_SERVER=1 && make-j`nproc` && npm i ws fs http && cd ../.. && cd Client && mkdir build && cd build && cmake .. && make-j`nproc` && cd .. && mv build/gardn-client.js build/gardn-client.wasm public && cd .. && clear && echo "Make sure to update Shared/Config.cc with a websocket link for your codespace. Then, create 2 new terminals. In the first one, run `cd Server/build && node ./gardn-server`. In the second one, run `cd Client/public && python3 -m http.server`. Make sure to make both ports public or it won't work. If you can connect to the client but not the server, open developer tools and go to the NETWORK section. If you find it trying to fetch a websocket link over and over again, most likely your Server port isn't public, or your websocket link in Shared/Config.cc is incorrect." 
```
This is an example `WS_URL` value for a codespace. `extern std::string const WS_URL = "wss://obscure-space-waddle-g4w5rrwppjq9c5q6-9001.app.github.dev/";`. 

# Hosting 
The client may be hosted with any http server (eg. ``nginx``, ``http-server``). The wasm server automatically hosts content at ``localhost:9001`` as well.

If hosting somewhere other than ``localhost``, use the  ``WS_URL`` constant in ``Shared/Config.cc`` to specify a websocket url.

# Compilation Flags

``DEBUG`` | ``Server & Client`` | ``Default: 0`` : compiles with assertions and failsafes. <br>
``WASM_SERVER`` | ``Server only`` | ``Default : 0`` : compiles to WASM/JS instead of a native binary. <br>
``TDM`` | ``Server only`` | ``Default: 0`` : enables TDM instead of FFA.<br>
``GENERAL_SPATIAL_HASH`` | ``Server only`` | ``Default: 0`` : uses the canonical hash grid implementation instead of a uniform grid; enable this to support large entities. <br>
``USE_CODEPOINT_LEN`` | ``Server & Client`` | ``Default: 0`` : uses the number of codepoints (characters) instead of byte length for string validation and truncation - useful for non-english characters. Should be the same on both server and client.

# License
[LICENSE](./LICENSE)
