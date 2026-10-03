// POSIX entry point: builds a Windows-style command line, sets up the data directory
// (registry file, saves, the virtual C: drive) and runs the host's wWinMain.
#include <windows.h>

#include <cstdlib>
#include <string>
#include <pthread.h>
#include <sys/stat.h>

#include "w32/w32.hpp"

#ifdef __ANDROID__
#include <SDL_main.h>  // main becomes SDL_main, which SDLActivity calls
void androidInit(const std::string& data);
#endif

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int);
extern "C" void w32_setCommandLine(const wchar_t* cmd);
namespace w32 {
void setDataDir(const std::string& dir);
void setRegistryFile(const std::string& path);
}  // namespace w32

static int gameMain(int argc, char** argv) {
    // Data directory: FABLE_RECOMP_DATA, else $XDG_DATA_HOME/FableRecomp, else ~/.local/share/FableRecomp.
    std::string data;
    if (const char* d = std::getenv("FABLE_RECOMP_DATA")) data = d;
    else if (const char* x = std::getenv("XDG_DATA_HOME")) data = std::string(x) + "/FableRecomp";
    else data = std::string(std::getenv("HOME") ? std::getenv("HOME") : ".") + "/.local/share/FableRecomp";
    mkdir(data.c_str(), 0755);
#ifdef __ANDROID__
    androidInit(data);
#endif
    mkdir((data + "/drive_c").c_str(), 0755);
    // The save folder the Windows installation provides (the game checks it but never creates it).
    for (const char* d : {"/Documents", "/Documents/My Games", "/Documents/My Games/Fable"}) mkdir((data + d).c_str(), 0755);
    w32::setDataDir(data);
    w32::setCDrive(data + "/drive_c");
    w32::setRegistryFile(data + "/registry.txt");

    // Paths given in POSIX form (--game, FABLE_DIR) become Windows paths on drive Z:, which is
    // what the game expects of absolute paths.
    if (const char* d = std::getenv("FABLE_DIR"); d && d[0] == '/') {
        const std::string w = w32::toUtf8(w32::toWindowsPath(d).c_str());
        setenv("FABLE_DIR", w.c_str(), 1);
    }
    w32::wstr cmd;
    for (int i = 0; i < argc; ++i) {
        const bool path = i > 0 && argv[i][0] == '/' && std::string(argv[i - 1]) == "--game";
        const w32::wstr a = path ? w32::toWindowsPath(argv[i]) : w32::fromUtf8(argv[i]);
        if (i) cmd += L' ';
        const bool quote = a.empty() || a.find_first_of(L" \t\"") != w32::wstr::npos;
        if (quote) cmd += L'"';
        for (wchar_t c : a) {
            if (c == L'"') cmd += L'\\';
            cmd += c;
        }
        if (quote) cmd += L'"';
    }
    w32_setCommandLine(cmd.c_str());
    return wWinMain(GetModuleHandleW(nullptr), nullptr, GetCommandLineW(), SW_SHOWNORMAL);
}

// The game runs on a thread with the 256 MiB stack the Windows build links with (lifted code
// recurses on the host stack); the process's own main thread (on Android, SDL's Java thread,
// with about 1 MiB) is too small.
int main(int argc, char** argv) {
    struct Args { int argc; char** argv; int rc; } args{argc, argv, 0};
    pthread_attr_t a;
    pthread_attr_init(&a);
    pthread_attr_setstacksize(&a, size_t(256) << 20);
    pthread_t t;
    if (pthread_create(&t, &a, [](void* p) -> void* {
            auto* x = static_cast<Args*>(p);
            x->rc = gameMain(x->argc, x->argv);
            return nullptr;
        }, &args) != 0)
        return gameMain(argc, argv);
    pthread_attr_destroy(&a);
    pthread_join(t, nullptr);
    return args.rc;
}
