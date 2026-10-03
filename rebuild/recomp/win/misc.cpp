// GDI32, ADVAPI32 (registry), WINMM, VERSION, SHELL32, OLE32/OLEAUT32, IMM32, WSOCK32.
#include "com.hpp"

#include <imm.h>
#include <mmsystem.h>
#include <shlobj.h>

#include <mutex>

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
IMPORT(A, RegOpenKeyExA) {
    HKEY out = nullptr;
    const LONG r = RegOpenKeyExA(hk(arg(c, 0)), argp(c, 1), arg(c, 2), arg(c, 3) | KEY_WOW64_32KEY, &out);
    if (r == ERROR_SUCCESS) wr32(arg(c, 4), gh(out));
    HLOG(1, "RegOpenKeyExA(%08X, %s) -> %ld", arg(c, 0), arg(c, 1) ? argp(c, 1) : "", r);
    retStd(c, static_cast<uint32_t>(r), 5);
}
IMPORT(A, RegCreateKeyExA) {
    HKEY out = nullptr;
    DWORD disp = 0;
    const LONG r = RegCreateKeyExA(hk(arg(c, 0)), argp(c, 1), 0, arg(c, 3) ? argp(c, 3) : nullptr, arg(c, 4), arg(c, 5) | KEY_WOW64_32KEY,
                                   nullptr, &out, &disp);
    if (r == ERROR_SUCCESS) wr32(arg(c, 7), gh(out));
    if (arg(c, 8)) wr32(arg(c, 8), disp);
    HLOG(1, "RegCreateKeyExA(%08X, %s) -> %ld", arg(c, 0), argp(c, 1), r);
    retStd(c, static_cast<uint32_t>(r), 9);
}
IMPORT(A, RegCreateKeyExW) {
    HKEY out = nullptr;
    DWORD disp = 0;
    const LONG r = RegCreateKeyExW(hk(arg(c, 0)), argp<wchar_t>(c, 1), 0, arg(c, 3) ? argp<wchar_t>(c, 3) : nullptr, arg(c, 4),
                                   arg(c, 5) | KEY_WOW64_32KEY, nullptr, &out, &disp);
    if (r == ERROR_SUCCESS) wr32(arg(c, 7), gh(out));
    if (arg(c, 8)) wr32(arg(c, 8), disp);
    HLOG(1, "RegCreateKeyExW(%08X, %s) -> %ld", arg(c, 0), narrow(argp<wchar_t>(c, 1)).c_str(), r);
    retStd(c, static_cast<uint32_t>(r), 9);
}
FWD_STD(A, RegSetValueExW);
FWD_STD(A, RegQueryValueExA);
FWD_STD(A, RegSetValueExA);
FWD_STD(A, RegCloseKey);
FWD_STD(A, RegQueryValueExW);
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
