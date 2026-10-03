// Entry point: find the user's Fable install, map its Fable.exe at 0x00400000
// (data only: the code that runs is the recompiled C), bind imports to host
// handlers and start the retail CRT entry point.
#include "host.hpp"

#include <bcrypt.h>

#include <cstdio>
#include <vector>

namespace host {
std::wstring g_gameDir, g_exePath;
HMODULE g_fableRes;
HINSTANCE g_hinst;
void crtInit(const std::string& cmdline);
}  // namespace host

using namespace host;

namespace {

constexpr const char* kFableSha256 = "41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10";

bool fileExists(const std::wstring& p) {
    const DWORD a = GetFileAttributesW(p.c_str());
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
}

std::wstring withSlash(std::wstring d) {
    if (!d.empty() && d.back() != L'\\' && d.back() != L'/') d += L'\\';
    return d;
}

std::wstring findGameDir() {
    int n = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &n);
    std::wstring dir;
    for (int i = 1; i + 1 < n; ++i)
        if (_wcsicmp(argv[i], L"--game") == 0) dir = argv[i + 1];
    LocalFree(argv);
    if (!dir.empty() && fileExists(withSlash(dir) + L"Fable.exe")) return withSlash(dir);
    wchar_t buf[MAX_PATH * 2];
    if (GetEnvironmentVariableW(L"FABLE_DIR", buf, MAX_PATH * 2) && fileExists(withSlash(buf) + L"Fable.exe")) return withSlash(buf);
    GetModuleFileNameW(nullptr, buf, MAX_PATH * 2);
    std::wstring self = buf;
    self = self.substr(0, self.find_last_of(L"\\/") + 1);
    if (fileExists(self + L"Fable.exe")) return self;
    return {};
}

std::vector<uint8_t> readFile(const std::wstring& p) {
    std::vector<uint8_t> v;
    HANDLE h = CreateFileW(p.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (h == INVALID_HANDLE_VALUE) return v;
    LARGE_INTEGER sz;
    GetFileSizeEx(h, &sz);
    v.resize(static_cast<size_t>(sz.QuadPart));
    DWORD got = 0;
    ReadFile(h, v.data(), static_cast<DWORD>(v.size()), &got, nullptr);
    CloseHandle(h);
    v.resize(got);
    return v;
}

std::string sha256(const std::vector<uint8_t>& data) {
    BCRYPT_ALG_HANDLE alg = nullptr;
    BCRYPT_HASH_HANDLE hash = nullptr;
    uint8_t digest[32] = {};
    if (BCryptOpenAlgorithmProvider(&alg, BCRYPT_SHA256_ALGORITHM, nullptr, 0) != 0) return {};
    BCryptCreateHash(alg, &hash, nullptr, 0, nullptr, 0, 0);
    BCryptHashData(hash, const_cast<PUCHAR>(data.data()), static_cast<ULONG>(data.size()), 0);
    BCryptFinishHash(hash, digest, 32, 0);
    BCryptDestroyHash(hash);
    BCryptCloseAlgorithmProvider(alg, 0);
    std::string s;
    char b[3];
    for (uint8_t x : digest) { std::snprintf(b, sizeof b, "%02x", x); s += b; }
    return s;
}

DWORD WINAPI guestMain(void*) {
    bindThread(newGuestThread(0x100000));
    log("starting guest at entry point 0x%08X", kEntryPoint);
    guestCall(kEntryPoint, {});
    log("guest entry point returned");
    return 0;
}

}  // namespace

int WINAPI wWinMain(HINSTANCE inst, HINSTANCE, LPWSTR, int) {
    g_hinst = inst;
    // Guest memory first, before anything else can land in the low 2 GiB.
    reserveGuestDllRanges();  // recompiled DLLs must get their exact base addresses
    const bool memOk = memInit();
    g_gameDir = findGameDir();
    logInit(g_gameDir.empty() ? L"FableRecomp.log" : g_gameDir + L"FableRecomp.log");
    log("Fable: The Lost Chapters - recompiled x64 host");
    if (!memOk) die("could not reserve guest memory at 0x00400000; another program may be injecting into this process");
    if (g_gameDir.empty())
        die("Could not find your Fable: The Lost Chapters install.\n\nPut FableRecomp.exe in the game folder (next to Fable.exe), "
            "or start it with --game \"<folder>\".");
    g_exePath = g_gameDir + L"Fable.exe";
    log("game folder: %s", narrow(g_gameDir.c_str()).c_str());

    const std::vector<uint8_t> file = readFile(g_exePath);
    if (file.empty()) die("cannot read %s", narrow(g_exePath.c_str()).c_str());
    const std::string hash = sha256(file);
    if (hash != kFableSha256)
        die("This build was recompiled from the Steam Fable.exe (SHA-256 %s), but the Fable.exe found has SHA-256 %s.", kFableSha256,
            hash.c_str());

    threadsInit();
    std::string cmd = "\"" + narrow(g_exePath.c_str()) + "\"";
    crtInit(cmd);
    {
        const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS32*>(file.data() + reinterpret_cast<const IMAGE_DOS_HEADER*>(file.data())->e_lfanew);
        if (nt->Signature != IMAGE_NT_SIGNATURE || nt->OptionalHeader.ImageBase != kImageBase) die("Fable.exe is not the expected PE32 image");
        mapPeImage(file, kImageBase, "Fable.exe");
    }
    g_fableRes = LoadLibraryExW(g_exePath.c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE);
    SetCurrentDirectoryW(g_gameDir.c_str());

    HANDLE t = CreateThread(nullptr, 256u << 20, guestMain, nullptr, STACK_SIZE_PARAM_IS_A_RESERVATION, nullptr);
    WaitForSingleObject(t, INFINITE);
    DWORD code = 0;
    GetExitCodeThread(t, &code);
    return static_cast<int>(code);
}
