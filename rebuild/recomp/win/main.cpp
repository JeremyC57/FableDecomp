// Entry point: find the user's Fable install, map its Fable.exe at 0x00400000
// (data only: the code that runs is the recompiled C), bind imports to host
// handlers and start the retail CRT entry point.
#include "host.hpp"

#include <bcrypt.h>
#include <shobjidl.h>

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

bool isGameDir(const std::wstring& d) { return !d.empty() && fileExists(withSlash(d) + L"Fable.exe"); }

// The folder chosen last time (HKCU\Software\FableRecomp, GameDir).
constexpr const wchar_t* kSettingsKey = L"Software\\FableRecomp";
std::wstring savedGameDir() {
    wchar_t buf[MAX_PATH * 2] = {};
    DWORD size = sizeof buf - sizeof(wchar_t);
    if (RegGetValueW(HKEY_CURRENT_USER, kSettingsKey, L"GameDir", RRF_RT_REG_SZ, nullptr, buf, &size) != ERROR_SUCCESS) return {};
    return buf;
}
void saveGameDir(const std::wstring& d) {
    RegSetKeyValueW(HKEY_CURRENT_USER, kSettingsKey, L"GameDir", REG_SZ, d.c_str(), static_cast<DWORD>((d.size() + 1) * sizeof(wchar_t)));
}

// The default Steam library: <Steam>\steamapps\common\Fable The Lost Chapters.
std::wstring steamGameDir() {
    wchar_t buf[MAX_PATH * 2] = {};
    DWORD size = sizeof buf - sizeof(wchar_t);
    if (RegGetValueW(HKEY_CURRENT_USER, L"Software\\Valve\\Steam", L"SteamPath", RRF_RT_REG_SZ, nullptr, buf, &size) != ERROR_SUCCESS) {
        size = sizeof buf - sizeof(wchar_t);
        if (RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Valve\\Steam", L"InstallPath", RRF_RT_REG_SZ, nullptr, buf, &size) !=
            ERROR_SUCCESS)
            return {};
    }
    return withSlash(buf) + L"steamapps\\common\\Fable The Lost Chapters";
}

// Asks the user for the folder that holds their original Fable.exe.
std::wstring pickGameDir() {
    const HRESULT init = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    std::wstring result;
    for (;;) {
        IFileOpenDialog* dlg = nullptr;
        if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dlg)))) break;
        DWORD opts = 0;
        dlg->GetOptions(&opts);
        dlg->SetOptions(opts | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST);
        dlg->SetTitle(L"Select your Fable: The Lost Chapters folder (the one containing Fable.exe)");
        std::wstring picked;
        if (SUCCEEDED(dlg->Show(nullptr))) {
            IShellItem* item = nullptr;
            if (SUCCEEDED(dlg->GetResult(&item))) {
                PWSTR path = nullptr;
                if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path))) {
                    picked = path;
                    CoTaskMemFree(path);
                }
                item->Release();
            }
        }
        dlg->Release();
        if (picked.empty()) break;  // cancelled
        if (isGameDir(picked)) { result = withSlash(picked); break; }
        if (MessageBoxW(nullptr, (L"There is no Fable.exe in\n\n" + picked + L"\n\nPick the game folder, for example\n"
                                  L"...\\steamapps\\common\\Fable The Lost Chapters").c_str(),
                        L"Fable: The Lost Chapters", MB_RETRYCANCEL | MB_ICONWARNING) != IDRETRY)
            break;
    }
    if (SUCCEEDED(init)) CoUninitialize();
    return result;
}

// --game, FABLE_DIR, the exe's own folder, the folder chosen last time, the default
// Steam library, and finally a folder picker.
std::wstring findGameDir() {
    int n = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &n);
    std::wstring dir;
    for (int i = 1; i + 1 < n; ++i)
        if (_wcsicmp(argv[i], L"--game") == 0) dir = argv[i + 1];
    LocalFree(argv);
    if (isGameDir(dir)) return withSlash(dir);
    wchar_t buf[MAX_PATH * 2];
    if (GetEnvironmentVariableW(L"FABLE_DIR", buf, MAX_PATH * 2) && isGameDir(buf)) return withSlash(buf);
    GetModuleFileNameW(nullptr, buf, MAX_PATH * 2);
    std::wstring self = buf;
    self = self.substr(0, self.find_last_of(L"\\/") + 1);
    if (isGameDir(self)) return self;
    if (const std::wstring saved = savedGameDir(); isGameDir(saved)) return withSlash(saved);
    if (const std::wstring steam = steamGameDir(); isGameDir(steam)) return withSlash(steam);
    return pickGameDir();
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

// Crash report for faults in the recompiled code: the host address, and the likely guest
// call chain (return addresses into Fable.exe found on the guest stack).
LONG CALLBACK crashHandler(EXCEPTION_POINTERS* e) {
    const DWORD code = e->ExceptionRecord->ExceptionCode;
    if (code != EXCEPTION_ACCESS_VIOLATION && code != EXCEPTION_ILLEGAL_INSTRUCTION && code != EXCEPTION_INT_DIVIDE_BY_ZERO &&
        code != EXCEPTION_STACK_OVERFLOW)
        return EXCEPTION_CONTINUE_SEARCH;
    static LONG once = 0;
    if (InterlockedExchange(&once, 1)) return EXCEPTION_CONTINUE_SEARCH;
    const auto addr = reinterpret_cast<uintptr_t>(e->ExceptionRecord->ExceptionAddress);
    const auto self = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    log("CRASH: exception 0x%08lX at host %p (FableRecomp.exe+0x%llX), %s address 0x%llX", code, e->ExceptionRecord->ExceptionAddress,
        static_cast<unsigned long long>(addr - self), e->ExceptionRecord->ExceptionInformation[0] ? "writing" : "reading",
        static_cast<unsigned long long>(e->ExceptionRecord->ExceptionInformation[1]));
    const GuestThread* t = currentThread();
    const uint32_t esp = t->ctx.esp;
    log("  guest esp 0x%08X ebp 0x%08X; return addresses on the guest stack:", esp, t->ctx.ebp);
    int shown = 0;
    for (uint32_t a = esp; a + 4 <= t->stackHi && a < esp + 0x2000 && shown < 24; a += 4) {
        const uint32_t v = rd32(a);
        if (v >= 0x401000 && v < 0x1200000) { log("    [esp+0x%04X] 0x%08X", a - esp, v); ++shown; }
    }
    return EXCEPTION_CONTINUE_SEARCH;
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
        die("No Fable: The Lost Chapters folder was selected.\n\nRun FableRecomp.exe again and pick the folder that contains "
            "Fable.exe, or start it with --game \"<folder>\".");
    g_exePath = g_gameDir + L"Fable.exe";
    log("game folder: %s", narrow(g_gameDir.c_str()).c_str());

    const std::vector<uint8_t> file = readFile(g_exePath);
    if (file.empty()) die("cannot read %s", narrow(g_exePath.c_str()).c_str());
    const std::string hash = sha256(file);
    if (hash != kFableSha256)
        die("This build was recompiled from the Steam Fable.exe (SHA-256 %s), but the Fable.exe found has SHA-256 %s.", kFableSha256,
            hash.c_str());
    saveGameDir(g_gameDir);

    threadsInit();
    AddVectoredExceptionHandler(1, crashHandler);
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
