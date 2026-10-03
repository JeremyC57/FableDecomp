// GDI32, ADVAPI32 (registry), WINMM, VERSION, SHELL32, OLE32/OLEAUT32, IMM32, WSOCK32.
#include "com.hpp"

#include <imm.h>
#include <mmsystem.h>
#include <shlobj.h>

#include <mutex>
#include <string>
#include <unordered_map>

namespace host {
namespace {

// ============================================================================
// GDI32
// ============================================================================
constexpr const char* G = "gdi32.dll";
FWD_STD(G, CreateFontIndirectA);
FWD_STD(G, DeleteObject);
FWD_STD(G, GetStockObject);
FWD_STD(G, SetTextColor);
IMPORT(G, GetObjectA) {
    HGDIOBJ h = hh(arg(c, 0));
    if (GetObjectType(h) == OBJ_BITMAP && arg(c, 2)) {
        BITMAP bm{};
        const int r = GetObjectA(h, sizeof bm, &bm);
        const uint32_t o = arg(c, 2);
        // x86 BITMAP: 6 x 4-byte fields + bmBits (24 bytes)
        wr32(o, bm.bmType); wr32(o + 4, bm.bmWidth); wr32(o + 8, bm.bmHeight); wr32(o + 12, bm.bmWidthBytes);
        wr16(o + 16, bm.bmPlanes); wr16(o + 18, bm.bmBitsPixel); wr32(o + 20, 0);
        retStd(c, r ? 24 : 0, 3);
        return;
    }
    retStd(c, GetObjectA(h, arg(c, 1), argp(c, 2)), 3);
}

// ============================================================================
// ADVAPI32: registry, in the 32-bit view the retail game used
// ============================================================================
constexpr const char* A = "advapi32.dll";
HKEY hk(uint32_t v) { return static_cast<HKEY>(hh(v)); }

// Registry virtualization. The game keeps its settings (resolution, ...) under
// HKLM\Software\Microsoft\Microsoft Games\Fable. Windows silently redirects such writes
// for the 32-bit retail Fable.exe to a per-user store; this 64-bit host does not get
// that, so it does the same itself: writes that HKLM refuses go to
//     HKCU\Software\Classes\VirtualStore\MACHINE\SOFTWARE\WOW6432Node\<rest>
// (the path Windows used for the retail game, so its saved settings carry over), and
// keys opened through the store fall back to the real HKLM key for values it lacks.
std::mutex g_regLock;
std::unordered_map<HKEY, HKEY> g_regFallback;  // virtual-store key -> real HKLM key (or null)

wstring virtualPath(const wstring& sub) {
    static const wstring kSoftware = L"software\\";
    if (sub.size() <= kSoftware.size() || _wcsnicmp(sub.c_str(), kSoftware.c_str(), kSoftware.size()) != 0) return {};
    return L"Software\\Classes\\VirtualStore\\MACHINE\\SOFTWARE\\WOW6432Node\\" + sub.substr(kSoftware.size());
}
bool isHklm(uint32_t root) { return root == 0x80000002u; }

// Opens (create=false) or creates the key, virtualizing HKLM\Software. Returns the
// handle the guest gets.
LONG regOpen(uint32_t root, const wstring& sub, REGSAM sam, bool create, HKEY* out, DWORD* disp) {
    sam |= KEY_WOW64_32KEY;
    const wstring vpath = isHklm(root) ? virtualPath(sub) : wstring();
    if (vpath.empty()) {
        return create ? RegCreateKeyExW(hk(root), sub.c_str(), 0, nullptr, 0, sam, nullptr, out, disp)
                      : RegOpenKeyExW(hk(root), sub.c_str(), 0, sam, out);
    }
    HKEY real = nullptr, virt = nullptr;
    LONG r = create ? RegCreateKeyExW(HKEY_LOCAL_MACHINE, sub.c_str(), 0, nullptr, 0, sam, nullptr, &real, disp)
                    : RegOpenKeyExW(HKEY_LOCAL_MACHINE, sub.c_str(), 0, sam, &real);
    if (r == ERROR_ACCESS_DENIED || r == ERROR_FILE_NOT_FOUND) {
        // read-only view of the real key for fallback values
        if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, sub.c_str(), 0, KEY_READ | KEY_WOW64_32KEY, &real) != ERROR_SUCCESS) real = nullptr;
    } else if (r != ERROR_SUCCESS) {
        return r;
    }
    // Prefer the virtual store when it exists (Windows' merged view), or when HKLM refused a write.
    const bool wantWrite = (sam & (KEY_SET_VALUE | KEY_CREATE_SUB_KEY)) != 0 || create;
    LONG v = RegOpenKeyExW(HKEY_CURRENT_USER, vpath.c_str(), 0, KEY_ALL_ACCESS, &virt);
    if (v != ERROR_SUCCESS && (r == ERROR_ACCESS_DENIED || (create && r != ERROR_SUCCESS)) && wantWrite)
        v = RegCreateKeyExW(HKEY_CURRENT_USER, vpath.c_str(), 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &virt, disp);
    if (v == ERROR_SUCCESS) {
        std::lock_guard<std::mutex> l(g_regLock);
        g_regFallback[virt] = real;
        *out = virt;
        return ERROR_SUCCESS;
    }
    if (real) { *out = real; return ERROR_SUCCESS; }  // HKLM itself (read-only if a write was refused)
    return r;
}
HKEY regFallback(HKEY h) {
    std::lock_guard<std::mutex> l(g_regLock);
    auto it = g_regFallback.find(h);
    return it == g_regFallback.end() ? nullptr : it->second;
}

IMPORT(A, RegOpenKeyExA) {
    HKEY out = nullptr;
    const LONG r = regOpen(arg(c, 0), widen(arg(c, 1) ? argp(c, 1) : ""), arg(c, 3), false, &out, nullptr);
    if (r == ERROR_SUCCESS) wr32(arg(c, 4), gh(out));
    HLOG(1, "RegOpenKeyExA(%08X, %s) -> %ld", arg(c, 0), arg(c, 1) ? argp(c, 1) : "", r);
    retStd(c, static_cast<uint32_t>(r), 5);
}
IMPORT(A, RegCreateKeyExA) {
    HKEY out = nullptr;
    DWORD disp = 0;
    const LONG r = regOpen(arg(c, 0), widen(argp(c, 1)), arg(c, 5), true, &out, &disp);
    if (r == ERROR_SUCCESS) wr32(arg(c, 7), gh(out));
    if (arg(c, 8)) wr32(arg(c, 8), disp);
    HLOG(1, "RegCreateKeyExA(%08X, %s) -> %ld", arg(c, 0), argp(c, 1), r);
    retStd(c, static_cast<uint32_t>(r), 9);
}
IMPORT(A, RegCreateKeyExW) {
    HKEY out = nullptr;
    DWORD disp = 0;
    const LONG r = regOpen(arg(c, 0), argp<wchar_t>(c, 1), arg(c, 5), true, &out, &disp);
    if (r == ERROR_SUCCESS) wr32(arg(c, 7), gh(out));
    if (arg(c, 8)) wr32(arg(c, 8), disp);
    HLOG(1, "RegCreateKeyExW(%08X, %s) -> %ld", arg(c, 0), narrow(argp<wchar_t>(c, 1)).c_str(), r);
    retStd(c, static_cast<uint32_t>(r), 9);
}
// RegQueryValueEx(key, name, reserved, type, data, cbData): falls back to HKLM for virtualized keys.
template <class Ch> void regQuery(Ctx* c) {
    const HKEY h = hk(arg(c, 0));
    auto q = [&](HKEY k) {
        if constexpr (sizeof(Ch) == 1)
            return RegQueryValueExA(k, argp(c, 1), nullptr, argp<DWORD>(c, 3), argp<BYTE>(c, 4), argp<DWORD>(c, 5));
        else
            return RegQueryValueExW(k, argp<wchar_t>(c, 1), nullptr, argp<DWORD>(c, 3), argp<BYTE>(c, 4), argp<DWORD>(c, 5));
    };
    LONG r = q(h);
    if (r == ERROR_FILE_NOT_FOUND)
        if (HKEY f = regFallback(h)) r = q(f);
    retStd(c, static_cast<uint32_t>(r), 6);
}
IMPORT(A, RegQueryValueExA) { regQuery<char>(c); }
IMPORT(A, RegQueryValueExW) { regQuery<wchar_t>(c); }
IMPORT(A, RegCloseKey) {
    const HKEY h = hk(arg(c, 0));
    HKEY f = nullptr;
    {
        std::lock_guard<std::mutex> l(g_regLock);
        auto it = g_regFallback.find(h);
        if (it != g_regFallback.end()) { f = it->second; g_regFallback.erase(it); }
    }
    if (f) RegCloseKey(f);
    retStd(c, static_cast<uint32_t>(RegCloseKey(h)), 1);
}
FWD_STD(A, RegSetValueExW);
FWD_STD(A, RegSetValueExA);
FWD_STD(A, RegDeleteValueW);

// ============================================================================
// WINMM
// ============================================================================
constexpr const char* W = "winmm.dll";
FWD_STD(W, timeGetTime);
FWD_STD(W, timeBeginPeriod);
FWD_STD(W, timeEndPeriod);
FWD_STD(W, timeKillEvent);
struct TimerCb { uint32_t fn, user; };
void CALLBACK timerProc(UINT id, UINT msg, DWORD_PTR user, DWORD_PTR, DWORD_PTR) {
    auto* t = reinterpret_cast<TimerCb*>(user);
    guestCall(t->fn, {id, msg, t->user, 0, 0});
}
IMPORT(W, timeSetEvent) {
    const uint32_t flags = arg(c, 4);
    MMRESULT r;
    if (flags & (TIME_CALLBACK_EVENT_SET | TIME_CALLBACK_EVENT_PULSE)) {
        r = timeSetEvent(arg(c, 0), arg(c, 1), reinterpret_cast<LPTIMECALLBACK>(hh(arg(c, 2))), arg(c, 3), flags);
    } else {
        auto* t = new TimerCb{arg(c, 2), arg(c, 3)};  // lives as long as the timer (one per game session)
        r = timeSetEvent(arg(c, 0), arg(c, 1), timerProc, reinterpret_cast<DWORD_PTR>(t), flags);
    }
    HLOG(1, "timeSetEvent(%u ms, cb 0x%08X, flags 0x%X) -> %u", arg(c, 0), arg(c, 2), flags, r);
    retStd(c, r, 5);
}
// No waveOut/mixer devices are reported: audio goes through DirectSound/OpenAL.
IMPORT(W, waveOutGetNumDevs) { retStd(c, 0, 0); }
IMPORT(W, mixerGetNumDevs) { retStd(c, 0, 0); }
IMPORT(W, mixerOpen) { retStd(c, MMSYSERR_NODRIVER, 5); }
IMPORT(W, waveOutOpen) { retStd(c, MMSYSERR_NODRIVER, 6); }
IMPORT(W, waveOutGetDevCapsA) { retStd(c, MMSYSERR_BADDEVICEID, 3); }
IMPORT(W, mixerGetDevCapsA) { retStd(c, MMSYSERR_BADDEVICEID, 3); }

// ============================================================================
// VERSION / SHELL32
// ============================================================================
constexpr const char* V = "version.dll";
FWD_STD(V, GetFileVersionInfoSizeA);
FWD_STD(V, GetFileVersionInfoA);
IMPORT(V, VerQueryValueA) {
    void* p = nullptr;
    UINT len = 0;
    const BOOL ok = VerQueryValueA(argp(c, 0), argp(c, 1), &p, &len);
    if (ok) wr32(arg(c, 2), static_cast<uint32_t>(reinterpret_cast<uintptr_t>(p)));  // points into the guest's block
    if (arg(c, 3)) wr32(arg(c, 3), len);
    retStd(c, ok, 4);
}
constexpr const char* S = "shell32.dll";
FWD_STD(S, SHGetFolderPathW);
IMPORT(S, ShellExecuteA) {
    log("ShellExecuteA(%s, %s)", arg(c, 1) ? argp(c, 1) : "", arg(c, 2) ? argp(c, 2) : "");
    retStd(c, gh(ShellExecuteA(static_cast<HWND>(hh(arg(c, 0))), argp(c, 1), argp(c, 2), argp(c, 3), argp(c, 4), arg(c, 5))), 6);
}

// ============================================================================
// OLE32 / OLEAUT32
// ============================================================================
constexpr const char* O = "ole32.dll";
FWD_STD(O, CoInitialize);
FWD_STD(O, CoInitializeEx);
FWD_STD(O, CoUninitialize);
FWD_STD(O, CoFreeUnusedLibraries);
IMPORT(O, CoTaskMemAlloc) { retStd(c, gmalloc(arg(c, 0)), 1); }
IMPORT(O, CoTaskMemFree) { gfree(arg(c, 0)); retStd(c, 0, 1); }
// Classes the host implements itself (e.g. the DirectShow filter graph) register here;
// everything else is created by the real 64-bit COM and handed out through a proxy when
// the requested interface has one.
IMPORT(O, CoCreateInstance) {
    const GUID& clsid = *argp<GUID>(c, 0);
    const GUID& iid = *argp<GUID>(c, 3);
    const uint32_t out = arg(c, 4);
    if (out) wr32(out, 0);
    if (com::HostClassFactory f = com::hostClass(clsid)) {
        const HRESULT hr = f(iid, out);
        HLOG(1, "CoCreateInstance({%08lX-...}, {%08lX-...}) -> host class 0x%08lX", clsid.Data1, iid.Data1, static_cast<unsigned long>(hr));
        retStd(c, static_cast<uint32_t>(hr), 5);
        return;
    }
    com::Class* cls = com::classForIid(iid);
    if (!cls || arg(c, 1)) {
        log("CoCreateInstance({%08lX-%04X-%04X-...}, {%08lX-...}) -> class not available", clsid.Data1, clsid.Data2, clsid.Data3, iid.Data1);
        retStd(c, static_cast<uint32_t>(REGDB_E_CLASSNOTREG), 5);
        return;
    }
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);  // no-op (S_FALSE / RPC_E_CHANGED_MODE) if the guest already did
    void* p = nullptr;
    const HRESULT hr = CoCreateInstance(clsid, nullptr, arg(c, 2), iid, &p);
    if (SUCCEEDED(hr) && out) wr32(out, com::wrapRaw(static_cast<IUnknown*>(p), *cls));
    log("CoCreateInstance({%08lX-%04X-%04X-...}, %s) -> 0x%08lX", clsid.Data1, clsid.Data2, clsid.Data3, cls->name, static_cast<unsigned long>(hr));
    retStd(c, static_cast<uint32_t>(hr), 5);
}
constexpr const char* OA = "oleaut32.dll";
IMPORTN(OA, "#2", SysAllocString) {
    const wchar_t* s = argp<wchar_t>(c, 0);
    if (!arg(c, 0)) { retStd(c, 0, 1); return; }
    const uint32_t n = static_cast<uint32_t>(wcslen(s));
    const uint32_t b = gmalloc(4 + n * 2 + 2);
    wr32(b, n * 2);
    std::memcpy(gp(b + 4), s, n * 2 + 2);
    retStd(c, b + 4, 1);
}
IMPORTN(OA, "#7", SysStringLen) { retStd(c, arg(c, 0) ? rd32(arg(c, 0) - 4) / 2 : 0, 1); }
IMPORTN(OA, "#8", VariantInit) { std::memset(argp(c, 0), 0, 16); retStd(c, 0, 1); }
IMPORTN(OA, "#9", VariantClear) {
    const uint32_t v = arg(c, 0);
    if (rd16(v) == 8 /* VT_BSTR */ && rd32(v + 8)) gfree(rd32(v + 8) - 4);
    std::memset(gp(v), 0, 16);
    retStd(c, 0, 1);
}
IMPORTN(OA, "#6", SysFreeString) {
    if (arg(c, 0)) gfree(arg(c, 0) - 4);
    retStd(c, 0, 1);
}


// ============================================================================
// IMM32 (input method editors)
// ============================================================================
constexpr const char* I = "imm32.dll";
FWD_STD(I, ImmGetCandidateListA);
FWD_STD(I, ImmGetIMEFileNameA);
FWD_STD(I, ImmGetVirtualKey);
FWD_STD(I, ImmReleaseContext);
FWD_STD(I, ImmNotifyIME);
FWD_STD(I, ImmGetContext);
FWD_STD(I, ImmGetDefaultIMEWnd);
FWD_STD(I, ImmGetOpenStatus);
FWD_STD(I, ImmGetConversionStatus);
FWD_STD(I, ImmIsIME);
FWD_STD(I, ImmSetConversionStatus);
FWD_STD(I, ImmAssociateContext);
FWD_STD(I, ImmGetCandidateListW);
FWD_STD(I, ImmGetCompositionStringW);
FWD_STD(I, ImmSetOpenStatus);
FWD_STD(I, ImmSimulateHotKey);
FWD_STD(I, ImmGetCompositionStringA);

// ============================================================================
// WSOCK32 (by ordinal): networking is reported as unavailable.
// ============================================================================
constexpr const char* WS = "wsock32.dll";
uint32_t g_wsaError = 0;
struct WsockOrdinals {
    WsockOrdinals() {
        registerImport(WS, "#115", [](Ctx* c) {  // WSAStartup(version, WSADATA*)
            const uint32_t d = arg(c, 1);
            std::memset(gp(d), 0, 400);
            wr16(d, 0x0101);
            wr16(d + 2, 0x0202);
            std::strcpy(gp<char>(d + 4), "Fable recompiled (no network)");
            retStd(c, 0, 2);
        });
        registerImport(WS, "#116", [](Ctx* c) { retStd(c, 0, 0); });  // WSACleanup
        registerImport(WS, "#111", [](Ctx* c) { retStd(c, g_wsaError, 0); });  // WSAGetLastError
        registerImport(WS, "#23", [](Ctx* c) { g_wsaError = 10050; retStd(c, 0xFFFFFFFFu, 3); });  // socket -> WSAENETDOWN
        registerImport(WS, "#9", [](Ctx* c) { const uint32_t v = arg(c, 0) & 0xFFFF; retStd(c, ((v & 0xFF) << 8) | (v >> 8), 1); });  // htons
        registerImport(WS, "#15", [](Ctx* c) { const uint32_t v = arg(c, 0) & 0xFFFF; retStd(c, ((v & 0xFF) << 8) | (v >> 8), 1); });  // ntohs
        registerImport(WS, "#10", [](Ctx* c) { retStd(c, 0xFFFFFFFFu, 1); });  // inet_addr -> INADDR_NONE
        registerImport(WS, "#11", [](Ctx* c) { static uint32_t s = gstrdup("0.0.0.0"); retStd(c, s, 1); });  // inet_ntoa
        registerImport(WS, "#52", [](Ctx* c) { g_wsaError = 11001; retStd(c, 0, 1); });  // gethostbyname
        registerImport(WS, "#151", [](Ctx* c) { retStd(c, 0, 2); });  // __WSAFDIsSet
        static const struct { const char* ord; int nargs; } fails[] = {
            {"#1", 3}, {"#2", 3}, {"#3", 1}, {"#4", 3}, {"#12", 3}, {"#13", 2}, {"#16", 4},
            {"#17", 6}, {"#18", 5}, {"#19", 4}, {"#20", 6}, {"#21", 5}};
        for (auto& f : fails) {
            Handler h = nullptr;
            switch (f.nargs) {
            case 1: h = [](Ctx* c) { g_wsaError = 10050; retStd(c, 0xFFFFFFFFu, 1); }; break;
            case 2: h = [](Ctx* c) { g_wsaError = 10050; retStd(c, 0xFFFFFFFFu, 2); }; break;
            case 3: h = [](Ctx* c) { g_wsaError = 10050; retStd(c, 0xFFFFFFFFu, 3); }; break;
            case 4: h = [](Ctx* c) { g_wsaError = 10050; retStd(c, 0xFFFFFFFFu, 4); }; break;
            case 5: h = [](Ctx* c) { g_wsaError = 10050; retStd(c, 0xFFFFFFFFu, 5); }; break;
            default: h = [](Ctx* c) { g_wsaError = 10050; retStd(c, 0xFFFFFFFFu, 6); }; break;
            }
            registerImport(WS, f.ord, h);
        }
    }
} g_wsock;

}  // namespace
}  // namespace host
