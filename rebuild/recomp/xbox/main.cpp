// Entry point of the recompiled original-Xbox game (Linux, Android): settings, the disc, then
// bootGame (boot.cpp) and the video loop.
//
//   FableXbox --game <disc image (.iso) | folder with default.xbe and Data/> [--hdd <folder>]
//             [--extract-to <folder>]   (a disc image is extracted there once; default <hdd>/../game)
#include "disc.hpp"
#include "gpu.hpp"
#include "input.hpp"
#include "settings.hpp"
#include "pc_textures.hpp"
#include "xhost.hpp"

#include <SDL.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
#include <sys/stat.h>

namespace xb {
void videoMain();  // runs on the main thread (window, GPU presentation); returns on quit
void bootGame(std::vector<uint8_t> xbe, const std::string& game, const std::string& hdd);  // boot.cpp
} // namespace xb

using namespace xb;

#if defined(__ANDROID__)
namespace xb { void androidInit(); }
#endif

namespace xb {
const char* g_dataDir = ".";
static char** g_argv;
#if defined(__ANDROID__)
void androidRelaunch();
#endif
void hostRelaunch() {
    fflush(nullptr);
#if defined(__ANDROID__)
    androidRelaunch();  // a new game process, started from the app's main one
    _exit(0);
#else
    execv("/proc/self/exe", g_argv);
    die("relaunch failed");
#endif
}
}  // namespace xb

int main(int argc, char** argv) {
    xb::g_argv = argv;
#if defined(__ANDROID__)
    xb::androidInit();
#endif
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");  // SIGINT/SIGTERM keep their default action
    std::string game, hdd, config, extractTo;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "--game" && i + 1 < argc) game = argv[++i];
        else if (a == "--hdd" && i + 1 < argc) hdd = argv[++i];
        else if (a == "--config" && i + 1 < argc) config = argv[++i];
        else if (a == "--extract-to" && i + 1 < argc) extractTo = argv[++i];
    }
    if (const char* e = getenv("XBOX_LOG")) g_logLevel = atoi(e);
    if (game.empty()) {
        fprintf(stderr, "usage: %s --game <disc image (.iso) | extracted disc folder> [--hdd <folder>]\n", argv[0]);
        return 2;
    }
    if (hdd.empty()) hdd = game + "/../xbox_hdd";
    // <hdd>/../fable_xbox.ini, computed lexically: the hdd folder may not exist yet.
    {
        static const std::string data = std::filesystem::path(hdd).lexically_normal().parent_path().string();
        g_dataDir = data.empty() ? "." : data.c_str();
    }
    loadSettings(config.empty() ? (std::filesystem::path(hdd).lexically_normal().parent_path() / "fable_xbox.ini").string() : config);
    {
        static const std::string rec = (std::filesystem::path(hdd).lexically_normal().parent_path() / "FableXbox_input.txt").string();
        input::setRecordFile(rec.c_str());
    }
    profilerStart();
    if (!std::filesystem::is_directory(game)) {
        // A disc image: extract the game files once (to --extract-to, default <hdd>/../game), run from there.
        if (extractTo.empty()) extractTo = (std::filesystem::path(hdd).lexically_normal().parent_path() / "game").string();
        if (!disc::extracted(game, extractTo)) {
            if (!disc::open(game)) die("%s is not an Xbox disc image (or a folder with default.xbe)", game.c_str());
            printf("extracting %s to %s\n", game.c_str(), extractTo.c_str());
            int lastPct = -1;
            if (!disc::extract(game, extractTo, [&](uint64_t done, uint64_t total, const std::string&) {
                    const int pct = total ? static_cast<int>(done * 100 / total) : 100;
                    if (pct != lastPct) printf("\r  %d%%", lastPct = pct), fflush(stdout);
                }))
                die("extracting %s failed", game.c_str());
            printf("\n");
        }
        game = extractTo;
    }
    std::vector<uint8_t> xbe;
    {
        std::ifstream in(game + "/default.xbe", std::ios::binary);
        if (!in) die("cannot open %s/default.xbe", game.c_str());
        xbe.assign(std::istreambuf_iterator<char>(in), {});
    }
    {  // Shader and pipeline caches next to the saves (<hdd>/../cache), unless set by the caller.
        const auto cache = std::filesystem::path(hdd).lexically_normal().parent_path() / "cache";
        std::error_code ec;
        std::filesystem::create_directories(cache, ec);
        if (!getenv("FABLE_CACHE_DIR") && !ec) setenv("FABLE_CACHE_DIR", cache.string().c_str(), 1);
    }
    pctex::init(game.c_str(), getenv("FABLE_CACHE_DIR"));
    bootGame(std::move(xbe), game, hdd);
    videoMain();
    return 0;
}
