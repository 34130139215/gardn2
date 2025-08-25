#include <Shared/Config.hh>

#define STR1(arg) #arg
#define ARG_TO_STR(arg) STR1(arg)

extern const uint64_t VERSION_HASH = 0x1A2B3C4D5E6F7890ULL; 
extern const uint32_t SERVER_PORT = 9001;
extern const uint32_t MAX_NAME_LENGTH = 16;
extern std::string const WS_URL = "wss://gardn.pro/";
// git clone https://github.com/emscripten-core/emsdk.git /workspaces/gardn/emsdk && cd /workspaces/gardn/emsdk && ./emsdk install latest && ./emsdk activate latest && source /workspaces/gardn/emsdk/emsdk_env.sh
// cd emsdk && ./emsdk activate latest && source /workspaces/gardn/emsdk/emsdk_env.sh
