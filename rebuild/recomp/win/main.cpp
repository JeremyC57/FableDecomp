// Entry point: find the user's Fable install, map its Fable.exe at 0x00400000
// (data only: the code that runs is the recompiled C), bind imports to host
// handlers and start the retail CRT entry point.
#ifdef FABLE_POSIX
#include <dlfcn.h>
#endif
#include "host.hpp"

#include <bcrypt.h>
#include <shobjidl.h>

#include <cstdio>
#include <map>
#include <string>
#include <vector>

namespace host {
wstring g_gameDir, g_exePath;
HMODULE g_fableRes;
HINSTANCE g_hinst;
void crtInit(const std::string& cmdline);
}  // namespace host

using namespace host;

namespace {

constexpr const char* kFableSha256 = "41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10";

bool fileExists(const wstring& p) {
    const DWORD a = GetFileAttributesW(p.c_str());
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
}

wstring withSlash(wstring d) {
    if (!d.empty() && d.back() != L'\\' && d.back() != L'/') d += L'\\';
    return d;
}

bool isGameDir(const wstring& d) { return !d.empty() && fileExists(withSlash(d) + L"Fable.exe"); }

// The folder chosen last time (HKCU\Software\FableRecomp, GameDir).
constexpr const wchar_t* kSettingsKey = L"Software\\FableRecomp";
wstring savedGameDir() {
    wchar_t buf[MAX_PATH * 2] = {};
    DWORD size = sizeof buf - sizeof(wchar_t);
    if (RegGetValueW(HKEY_CURRENT_USER, kSettingsKey, L"GameDir", RRF_RT_REG_SZ, nullptr, buf, &size) != ERROR_SUCCESS) return {};
    return buf;
}
void saveGameDir(const wstring& d) {
    RegSetKeyValueW(HKEY_CURRENT_USER, kSettingsKey, L"GameDir", REG_SZ, d.c_str(), static_cast<DWORD>((d.size() + 1) * sizeof(wchar_t)));
}

// The default Steam library: <Steam>\steamapps\common\Fable The Lost Chapters.
wstring steamGameDir() {
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
wstring pickGameDir() {
    const HRESULT init = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    wstring result;
    for (;;) {
        IFileOpenDialog* dlg = nullptr;
        if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dlg)))) break;
        DWORD opts = 0;
        dlg->GetOptions(&opts);
        dlg->SetOptions(opts | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST);
        dlg->SetTitle(L"Select your Fable: The Lost Chapters folder (the one containing Fable.exe)");
        wstring picked;
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
wstring findGameDir() {
    int n = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &n);
    wstring dir;
    for (int i = 1; i + 1 < n; ++i)
        if (_wcsicmp(argv[i], L"--game") == 0) dir = argv[i + 1];
    LocalFree(argv);
    if (isGameDir(dir)) return withSlash(dir);
    wchar_t buf[MAX_PATH * 2];
    if (GetEnvironmentVariableW(L"FABLE_DIR", buf, MAX_PATH * 2) && isGameDir(buf)) return withSlash(buf);
    GetModuleFileNameW(nullptr, buf, MAX_PATH * 2);
    wstring self = buf;
    self = self.substr(0, self.find_last_of(L"\\/") + 1);
    if (isGameDir(self)) return self;
    if (const wstring saved = savedGameDir(); isGameDir(saved)) return withSlash(saved);
    if (const wstring steam = steamGameDir(); isGameDir(steam)) return withSlash(steam);
    return pickGameDir();
}

std::vector<uint8_t> readFile(const wstring& p) {
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

#ifdef FABLE_POSIX
}  // namespace
// What the crash report names a host pc by. The lifted functions are in the tables (the main one
// and the recompiled DLLs'), but code without a table entry sits among and after them: each
// --wrap function's original body F_X_orig (Lifter.cpp emits it just before the F_X wrapper, and
// only F_X is in the table), the runtime (runtime.c) and the host's own code. Those are marks too
// (as in profiler.cpp), so a pc in them is not given to the lifted function before it.
extern "C" {
typedef struct RecompEntry { uint32_t addr; GuestFn fn; } RecompEntry;
typedef struct GuestDllTable { const char* file; const RecompEntry* table; const uint32_t* size; } GuestDllTable;
extern const GuestDllTable guest_dll_tables[];  // guest_dlls.c (CMake)
extern const uint32_t guest_dll_count;
uint32_t recomp_num_tables(void);
const RecompEntry* recomp_table_entries(uint32_t i, uint32_t* n);
int recomp_init_memory(void);
void recomp_register_table(const RecompEntry* table, uint32_t size);
// The lift command's --wrap list (HANDOFF.md): the original body and the host function of each.
// Weak: one this build was not lifted with is null.
#define FABLE_WRAPS(X)                                                                                                       \
    X(0055CB10, host_ui_event) X(0058F649, host_open_inventory) X(0041CF47, host_ui_scale) X(0069AC90, host_camera_set)        \
    X(00533D30, host_component_size) X(00B24690, host_video_options) X(0047E196, host_save_game) X(0054EF00, host_text_draw)   \
    X(0052FFD0, host_update_position) X(00445B80, host_ztarget_toggle) X(00591E5E, host_inventory_screen)                      \
    X(00581FA4, host_csi_graphic) X(00581E33, host_quick_access_item) X(005840A6, host_use_quick_item)                         \
    X(00583A92, host_use_context_item) X(0041DB1D, host_create_component) X(00530780, host_find_child)                         \
    X(00661F70, host_is_child) X(0041E6D3, host_ui_manager_event) X(00446000, host_quick_button) X(0054E6F0, host_viewport_draw) \
    X(00BE1E70, host_viewport2d_render) X(00A4CEC0, host_calc_local_transformations)                                           \
    X(00A4D770, host_calc_local_transformations_relative) X(00A4D330, host_calc_local_bone_transformation) X(00C78A10, host_hrtf_fir) X(00C79440, host_hrtf_fir_ramp)           \
    X(00C9C2F0, host_mdct_butterfly)
#define FABLE_WRAP_DECL(a, h) __attribute__((weak)) void F_##a##_orig(Ctx* c); __attribute__((weak)) void h(Ctx* c);
FABLE_WRAPS(FABLE_WRAP_DECL)
#undef FABLE_WRAP_DECL
}
namespace host::com { void comAddRef(Ctx* c); void comRelease(Ctx* c); void comQueryInterface(Ctx* c); }
namespace {
LONG CALLBACK crashHandler(EXCEPTION_POINTERS* e);

struct CodeMark {
    enum Kind : uint8_t { kNone, kLifted, kOrig, kRuntime, kHost } kind = kNone;
    uintptr_t at = 0;
    uint32_t guest = 0;
    const char* name = nullptr;
};

// The marks nearest to pc: the last one at or below it and the first one above it.
void nearestMarks(uintptr_t pc, CodeMark& lo, CodeMark& hi) {
    lo = {}, hi = {};
    auto mark = [&](uintptr_t at, CodeMark::Kind kind, uint32_t guest, const char* name) {
        if (!at) return;
        if (at <= pc) {
            if (!lo.kind || at > lo.at) lo = {kind, at, guest, name};
        } else if (!hi.kind || at < hi.at) {
            hi = {kind, at, guest, name};
        }
    };
    auto table = [&](const RecompEntry* r, uint32_t n) {
        for (uint32_t i = 0; r && i < n; ++i) mark(reinterpret_cast<uintptr_t>(r[i].fn), CodeMark::kLifted, r[i].addr, nullptr);
    };
    uint32_t n = 0;
    const RecompEntry* exe = recomp_table_entries(0, &n);  // Fable.exe's
    table(exe, n);
    for (uint32_t i = 0; i < guest_dll_count; ++i)  // every recompiled DLL, loaded (registered) or not
        if (guest_dll_tables[i].table && guest_dll_tables[i].size) table(guest_dll_tables[i].table, *guest_dll_tables[i].size);
#define FABLE_WRAP_MARK(a, h)                                                               \
    mark(reinterpret_cast<uintptr_t>(&F_##a##_orig), CodeMark::kOrig, 0x##a##u, nullptr); \
    mark(reinterpret_cast<uintptr_t>(&h), CodeMark::kHost, 0, #h);
    FABLE_WRAPS(FABLE_WRAP_MARK)
#undef FABLE_WRAP_MARK
#define FABLE_MARK(kind, f) mark(reinterpret_cast<uintptr_t>(&f), CodeMark::kind, 0, #f);
    FABLE_MARK(kRuntime, recomp_init_memory) FABLE_MARK(kRuntime, recomp_register_table) FABLE_MARK(kRuntime, recomp_num_tables)
    FABLE_MARK(kRuntime, recomp_table_entries) FABLE_MARK(kRuntime, recomp_lookup) FABLE_MARK(kRuntime, recomp_set_host_range)
    FABLE_MARK(kRuntime, recomp_dispatch) FABLE_MARK(kRuntime, recomp_dispatch_ic) FABLE_MARK(kRuntime, recomp_fatal)
    FABLE_MARK(kRuntime, recomp_cpuid) FABLE_MARK(kRuntime, recomp_rdtsc) FABLE_MARK(kRuntime, recomp_landing_push)
    FABLE_MARK(kRuntime, recomp_landing_pop) FABLE_MARK(kRuntime, recomp_landing_chain_get) FABLE_MARK(kRuntime, recomp_landing_chain_set)
    FABLE_MARK(kRuntime, recomp_resume_at)
    FABLE_MARK(kHost, host::com::comAddRef) FABLE_MARK(kHost, host::com::comRelease) FABLE_MARK(kHost, host::com::comQueryInterface)
    FABLE_MARK(kHost, crashHandler)
#undef FABLE_MARK
}

const char* markName(const CodeMark& m, char* buf, size_t size) {
    switch (m.kind) {
        case CodeMark::kLifted: std::snprintf(buf, size, "lifted F_%08X", m.guest); break;
        case CodeMark::kOrig: std::snprintf(buf, size, "F_%08X_orig (the original body of the wrapped F_%08X)", m.guest, m.guest); break;
        case CodeMark::kRuntime: std::snprintf(buf, size, "the runtime's %s", m.name); break;
        case CodeMark::kHost: std::snprintf(buf, size, "host function %s", m.name); break;
        default: std::snprintf(buf, size, "?"); break;
    }
    return buf;
}

// Where a host pc lies, from the marks around it. A pc between two lifted function starts is
// most likely in the lower one (marked approximate); a pc past the last lifted start before a
// runtime or host mark, below every mark, or in another module is not given to a lifted function.
void logCodeAt(const char* what, uintptr_t pc) {
    void* const p = reinterpret_cast<void*>(pc);
    Dl_info self{}, at{};
    const bool found = dladdr(p, &at) && at.dli_fname;
    if (!dladdr(reinterpret_cast<void*>(&crashHandler), &self) || !found || at.dli_fbase != self.dli_fbase) {
        if (found)
            log("  %s %p: %s+0x%llx%s%s, not the program's own code", what, p, at.dli_fname,
                static_cast<unsigned long long>(pc - reinterpret_cast<uintptr_t>(at.dli_fbase)), at.dli_sname ? " in " : "", at.dli_sname ? at.dli_sname : "");
        else
            log("  %s %p: in no loaded module", what, p);
        return;
    }
    CodeMark lo, hi;
    nearestMarks(pc, lo, hi);
    char low[96], next[128];
    if (hi.kind) {
        char n[96];
        std::snprintf(next, sizeof next, "%s -0x%llx", markName(hi, n, sizeof n), static_cast<unsigned long long>(hi.at - pc));
    } else {
        std::snprintf(next, sizeof next, "none");
    }
    const auto below = static_cast<unsigned long long>(pc - lo.at);
    const bool liftedLo = lo.kind == CodeMark::kLifted || lo.kind == CodeMark::kOrig;
    const bool liftedHi = hi.kind == CodeMark::kLifted || hi.kind == CodeMark::kOrig;
    if (!lo.kind)
        log("  %s %p: below every code mark (the first: %s): not lifted code", what, p, next);
    else if (liftedLo && liftedHi)
        log("  %s %p: most likely in %s +0x%llx (approximate: the function starts nearest below and above are that one and %s; "
            "out-of-line helpers from recomp.h also sit between lifted functions)",
            what, p, markName(lo, low, sizeof low), below, next);
    else if (liftedLo)
        log("  %s %p: past %s +0x%llx, the last lifted function start before the next mark (%s): not attributed (the end of that "
            "function, or the runtime or host code linked after the lifted code)",
            what, p, markName(lo, low, sizeof low), below, next);
    else
        log("  %s %p: not lifted code: after %s +0x%llx (approximate; the next mark: %s)", what, p, markName(lo, low, sizeof low), below, next);
}
#endif

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
#ifdef FABLE_POSIX
    if (e->ExceptionRecord->NumberParameters >= 4)  // kernel.cpp onSignal: a sent signal (si_code <= 0)
        log("  the signal was sent (by process %llu) after a fault: another handler took the fault first and re-raised it, so the "
            "host address is that raise; %s",
            static_cast<unsigned long long>(e->ExceptionRecord->ExceptionInformation[2]),
            e->ExceptionRecord->ExceptionInformation[3] ? "the address above is the thread's last memory fault as the kernel recorded it"
                                                        : "the fault address is not known (the address above is 0)");
    {   // the host symbol and module offset (GetModuleHandle is the guest image here)
        Dl_info info{};
        if (dladdr(reinterpret_cast<void*>(addr), &info) && info.dli_fname)
            log("  host: %s+0x%llx%s%s", info.dli_fname, static_cast<unsigned long long>(addr - reinterpret_cast<uintptr_t>(info.dli_fbase)), info.dli_sname ? " in " : "", info.dli_sname ? info.dli_sname : "");
    }
    logCodeAt("host pc", addr);  // the lifted guest function it falls in, if any
#if defined(__aarch64__)
    {
        const CONTEXT* x = e->ContextRecord;
        logCodeAt("host lr (the return address of the last call made, or of the caller)", static_cast<uintptr_t>(x->X[30]));
        for (int i = 0; i < 31; i += 4) {  // the lifted code keeps guest registers and pointers in these
            char line[160];
            int n = 0;
            for (int j = i; j < i + 4 && j < 31; ++j)
                n += std::snprintf(line + n, sizeof line - n, " x%-2d %016llx", j, static_cast<unsigned long long>(x->X[j]));
            log("  host%s", line);
        }
        log("  host sp %016llx", static_cast<unsigned long long>(x->Sp));
    }
#endif
#endif
    const GuestThread* t = currentThread();
    const uint32_t esp = t->ctx.esp;
    log("  guest registers at the last call or return: eax %08X ecx %08X edx %08X ebx %08X esi %08X edi %08X", t->ctx.eax, t->ctx.ecx,
        t->ctx.edx, t->ctx.ebx, t->ctx.esi, t->ctx.edi);
    log("  guest esp 0x%08X ebp 0x%08X; return addresses on the guest stack:", esp, t->ctx.ebp);
    int shown = 0;
    for (uint32_t a = esp; a + 4 <= t->stackHi && a < esp + 0x2000 && shown < 24; a += 4) {
        const uint32_t v = rd32(a);
        if (v >= 0x401000 && v < 0x1200000) { log("    [esp+0x%04X] 0x%08X", a - esp, v); ++shown; }
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

// FABLE_RECOMP_SAMPLE=<ms>: log the main guest thread's return-address chain every <ms>
// (a poor man's profiler for "where is it stuck"; the guest esp is the one last spilled
// at a call boundary, so the chain is approximate).
GuestThread* g_mainGuest;
DWORD WINAPI samplerMain(void* p) {
    const DWORD ms = static_cast<DWORD>(reinterpret_cast<uintptr_t>(p));
    for (;;) {
        Sleep(ms);
        const GuestThread* t = g_mainGuest;
        if (!t) continue;
        const uint32_t esp = t->ctx.esp;
        char line[512];
        int n = std::snprintf(line, sizeof line, "sample esp %08X:", esp);
        for (uint32_t a = esp, k = 0; a + 4 <= t->stackHi && a < esp + 0x4000 && k < 24 && n < 480; a += 4)
            if (const uint32_t v = rd32(a); v >= 0x401000 && v < 0x1200000) { n += std::snprintf(line + n, sizeof line - n, " %X", v); ++k; }
        log("%s", line);
    }
}

// Functions lifted with --trace: log ecx/arguments and a few words at ecx.
// Functions lifted with --trace ADDR: log each call with ecx and the first stack arguments
// (rate-limited per function).
void onTrace(Ctx* c, uint32_t fn) {
    static std::map<uint32_t, uint32_t> counts;
    const uint32_t n = ++counts[fn];
    if (n > 20 && (n & (n - 1))) return;  // the first 20 calls, then powers of two
    log("trace %08X #%u from %08X ecx=%08X edx=%08X args %08X %08X %08X", fn, n, rd32(c->esp), c->ecx, c->edx, rd32(c->esp + 4),
        rd32(c->esp + 8), rd32(c->esp + 12));
}

DWORD WINAPI guestMain(void*) {
    bindThread(newGuestThread(0x100000));
    g_mainGuest = currentThread();
    wchar_t buf[16];
    if (GetEnvironmentVariableW(L"FABLE_RECOMP_SAMPLE", buf, 16))
        CloseHandle(CreateThread(nullptr, 0, samplerMain, reinterpret_cast<void*>(static_cast<uintptr_t>(_wtoi(buf))), 0, nullptr));
    log("starting guest at entry point 0x%08X", kEntryPoint);
    guestCall(kEntryPoint, {});
    log("guest entry point returned");
    return 0;
}

}  // namespace

// Fable sets GFX_RESET when it starts and clears it when it exits cleanly; a set flag at
// start means the last run died and the game falls back to safe graphics (low resolution
// list, detail options locked). The recompiled game is rarely exited cleanly (an Android
// app gets swiped away), which left it in safe mode for good, so the flag is cleared here.
static void clearGfxReset() {
    HKEY k = nullptr;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Microsoft Games\\Fable TLC", 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &k) !=
        ERROR_SUCCESS)
        return;
    DWORD v = 0, size = sizeof v, type = 0;
    if (RegQueryValueExW(k, L"GFX_RESET", nullptr, &type, reinterpret_cast<BYTE*>(&v), &size) == ERROR_SUCCESS && type == REG_DWORD && v) {
        v = 0;
        RegSetValueExW(k, L"GFX_RESET", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&v), sizeof v);
        log("GFX_RESET was set (the last run did not exit cleanly); cleared so the graphics settings stay as chosen");
    }
    RegCloseKey(k);
}

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
    clearGfxReset();

    threadsInit();
    displayInit();
    uiScaleInit();
    AddVectoredExceptionHandler(1, crashHandler);
    recomp_on_trace = onTrace;
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
