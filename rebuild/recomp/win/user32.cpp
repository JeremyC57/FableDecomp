// USER32 imports: window classes and procedures (guest WndProcs called through
// a host proc), messages (MSG and pointer-carrying lParams converted between
// the x86 and x64 layouts), dialogs, and simple forwards.
#include "format.hpp"
#include "host.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <vector>

namespace host {
namespace {
constexpr const char* U = "user32.dll";

// ---------------------------------------------------------------------------
// window procedure registry
// ---------------------------------------------------------------------------
std::mutex g_wpLock;
std::map<ATOM, uint32_t> g_classProc;      // class atom -> guest proc
std::map<HWND, uint32_t> g_windowProc;     // subclassed window -> guest proc
std::map<HWND, uint32_t> g_dialogProc;     // dialog -> guest DlgProc
std::map<WNDPROC, uint32_t> g_hostProcTraps;  // host procs exposed to the guest
thread_local uint32_t t_pendingDlgProc;
thread_local std::vector<uint32_t> t_pendingClassProc;

LRESULT CALLBACK hostWndProcA(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK hostWndProcW(HWND, UINT, WPARAM, LPARAM);

uint32_t guestProcFor(HWND h) {
    std::lock_guard<std::mutex> l(g_wpLock);
    if (auto it = g_windowProc.find(h); it != g_windowProc.end()) return it->second;
    if (auto it = g_classProc.find(static_cast<ATOM>(GetClassWord(h, GCW_ATOM))); it != g_classProc.end()) return it->second;
    return 0;
}

// Host WNDPROC exposed to guest code (e.g. returned by GetWindowLong(GWL_WNDPROC)).
void callHostProc(Ctx* c, uintptr_t data);
uint32_t trapForHostProc(WNDPROC p) {
    std::lock_guard<std::mutex> l(g_wpLock);
    if (auto it = g_hostProcTraps.find(p); it != g_hostProcTraps.end()) return it->second;
    const uint32_t t = addTrap("host WNDPROC", callHostProc, reinterpret_cast<uintptr_t>(p));
    g_hostProcTraps[p] = t;
    return t;
}

// ---------------------------------------------------------------------------
// message parameter conversion
// ---------------------------------------------------------------------------
struct MsgFrame {
    HWND hwnd;
    UINT msg;
    WPARAM wp;
    LPARAM hostLp;    // what Windows passed
    uint32_t guestLp; // what the guest sees
    void (*toHost)(MsgFrame&) = nullptr;   // copy guest edits back into the host struct
    void (*toGuest)(MsgFrame&) = nullptr;  // refresh guest copy from host struct
    uint32_t extra = 0;
};
thread_local std::vector<MsgFrame*> t_frames;

void cpyToGuest(MsgFrame& f, uint32_t n) { std::memcpy(gp(f.guestLp), reinterpret_cast<void*>(f.hostLp), n); }
void cpyToHost(MsgFrame& f, uint32_t n) { std::memcpy(reinterpret_cast<void*>(f.hostLp), gp(f.guestLp), n); }

// WINDOWPOS: x86 {hwnd, hwndInsertAfter, x, y, cx, cy, flags} = 28 bytes.
void wposToGuest(uint32_t g, const WINDOWPOS* w) {
    wr32(g, gh(w->hwnd)); wr32(g + 4, gh(w->hwndInsertAfter));
    wr32(g + 8, w->x); wr32(g + 12, w->y); wr32(g + 16, w->cx); wr32(g + 20, w->cy); wr32(g + 24, w->flags);
}
void wposToHost(uint32_t g, WINDOWPOS* w) {
    w->hwndInsertAfter = static_cast<HWND>(hh(rd32(g + 4)));
    w->x = rd32(g + 8); w->y = rd32(g + 12); w->cx = rd32(g + 16); w->cy = rd32(g + 20); w->flags = rd32(g + 24);
}

// Prepares guest-visible lParam for a message; returns false if no conversion was needed.
bool convertIn(MsgFrame& f) {
    const LPARAM lp = f.hostLp;
    switch (f.msg) {
    case WM_NCCREATE:
    case WM_CREATE: {
        auto* cs = reinterpret_cast<CREATESTRUCTW*>(lp);  // A and W layouts match apart from string types
        const uint32_t g = gcalloc(1, 48);
        wr32(g + 0, static_cast<uint32_t>(reinterpret_cast<uintptr_t>(cs->lpCreateParams)));
        wr32(g + 4, kImageBase);
        wr32(g + 8, gh(cs->hMenu));
        wr32(g + 12, gh(cs->hwndParent));
        wr32(g + 16, cs->cy); wr32(g + 20, cs->cx); wr32(g + 24, cs->y); wr32(g + 28, cs->x);
        wr32(g + 32, cs->style);
        wr32(g + 44, cs->dwExStyle);
        // Names: keep guest pointers when they already are, otherwise leave null.
        if (isGuest(cs->lpszName)) wr32(g + 36, static_cast<uint32_t>(reinterpret_cast<uintptr_t>(cs->lpszName)));
        if (IS_INTRESOURCE(cs->lpszClass) || isGuest(cs->lpszClass)) wr32(g + 40, static_cast<uint32_t>(reinterpret_cast<uintptr_t>(cs->lpszClass)));
        f.guestLp = g;
        f.extra = g;
        return true;
    }
    case WM_GETMINMAXINFO:
        f.guestLp = gmemdup(reinterpret_cast<void*>(lp), sizeof(MINMAXINFO));
        f.extra = f.guestLp;
        f.toHost = [](MsgFrame& x) { cpyToHost(x, sizeof(MINMAXINFO)); };
        f.toGuest = [](MsgFrame& x) { cpyToGuest(x, sizeof(MINMAXINFO)); };
        return true;
    case WM_SIZING:
    case WM_MOVING:
        f.guestLp = gmemdup(reinterpret_cast<void*>(lp), sizeof(RECT));
        f.extra = f.guestLp;
        f.toHost = [](MsgFrame& x) { cpyToHost(x, sizeof(RECT)); };
        f.toGuest = [](MsgFrame& x) { cpyToGuest(x, sizeof(RECT)); };
        return true;
    case WM_STYLECHANGING:
    case WM_STYLECHANGED:
        f.guestLp = gmemdup(reinterpret_cast<void*>(lp), sizeof(STYLESTRUCT));
        f.extra = f.guestLp;
        f.toHost = [](MsgFrame& x) { cpyToHost(x, sizeof(STYLESTRUCT)); };
        f.toGuest = [](MsgFrame& x) { cpyToGuest(x, sizeof(STYLESTRUCT)); };
        return true;
    case WM_WINDOWPOSCHANGING:
    case WM_WINDOWPOSCHANGED:
        f.guestLp = gcalloc(1, 28);
        f.extra = f.guestLp;
        wposToGuest(f.guestLp, reinterpret_cast<WINDOWPOS*>(lp));
        f.toHost = [](MsgFrame& x) { wposToHost(x.guestLp, reinterpret_cast<WINDOWPOS*>(x.hostLp)); };
        f.toGuest = [](MsgFrame& x) { wposToGuest(x.guestLp, reinterpret_cast<WINDOWPOS*>(x.hostLp)); };
        return true;
    case WM_NCCALCSIZE:
        if (f.wp) {
            // NCCALCSIZE_PARAMS x86: RECT[3] + PWINDOWPOS (52 bytes), WINDOWPOS copy follows.
            f.guestLp = gcalloc(1, 52 + 28);
            f.extra = f.guestLp;
            f.toGuest = [](MsgFrame& x) {
                auto* p = reinterpret_cast<NCCALCSIZE_PARAMS*>(x.hostLp);
                std::memcpy(gp(x.guestLp), p->rgrc, 48);
                wr32(x.guestLp + 48, x.guestLp + 52);
                if (p->lppos) wposToGuest(x.guestLp + 52, p->lppos);
            };
            f.toHost = [](MsgFrame& x) {
                auto* p = reinterpret_cast<NCCALCSIZE_PARAMS*>(x.hostLp);
                std::memcpy(p->rgrc, gp(x.guestLp), 48);
                if (p->lppos) wposToHost(x.guestLp + 52, p->lppos);
            };
            f.toGuest(f);
        } else {
            f.guestLp = gmemdup(reinterpret_cast<void*>(lp), sizeof(RECT));
            f.extra = f.guestLp;
            f.toHost = [](MsgFrame& x) { cpyToHost(x, sizeof(RECT)); };
            f.toGuest = [](MsgFrame& x) { cpyToGuest(x, sizeof(RECT)); };
        }
        return true;
    default:
        break;
    }
    if (lp != static_cast<LPARAM>(static_cast<int32_t>(lp))) {
        // A host pointer the guest cannot address: hand over the low half; DefWindowProc
        // and CallWindowProc restore the original from the frame.
        f.guestLp = static_cast<uint32_t>(lp);
        return false;
    }
    f.guestLp = static_cast<uint32_t>(lp);
    return false;
}

LRESULT dispatchToGuest(uint32_t proc, HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    MsgFrame f{h, msg, wp, lp, 0};
    convertIn(f);
    t_frames.push_back(&f);
    const uint32_t r = guestCall(proc, {gh(h), msg, static_cast<uint32_t>(wp), f.guestLp});
    t_frames.pop_back();
    if (f.toHost) f.toHost(f);
    if (f.extra) gfree(f.extra);
    return static_cast<LRESULT>(static_cast<int32_t>(r));
}

// Finds the host lParam for a guest call that forwards the message being dispatched.
LPARAM hostLParam(HWND h, UINT msg, uint32_t guestLp, MsgFrame** frame) {
    *frame = nullptr;
    for (auto it = t_frames.rbegin(); it != t_frames.rend(); ++it) {
        MsgFrame* f = *it;
        if (f->hwnd == h && f->msg == msg && f->guestLp == guestLp) {
            *frame = f;
            return f->hostLp;
        }
    }
    return static_cast<LPARAM>(static_cast<int32_t>(guestLp));
}

template <bool W> LRESULT callDefault(WNDPROC host, HWND h, UINT msg, uint32_t wp, uint32_t lp) {
    MsgFrame* f;
    const LPARAM hl = hostLParam(h, msg, lp, &f);
    if (f && f->toHost) f->toHost(*f);
    LRESULT r;
    if (host) r = W ? CallWindowProcW(host, h, msg, wp, hl) : CallWindowProcA(host, h, msg, wp, hl);
    else r = W ? DefWindowProcW(h, msg, wp, hl) : DefWindowProcA(h, msg, wp, hl);
    if (f && f->toGuest) f->toGuest(*f);
    return r;
}

void callHostProc(Ctx* c, uintptr_t data) {
    const LRESULT r = callDefault<false>(reinterpret_cast<WNDPROC>(data), static_cast<HWND>(hh(arg(c, 0))), arg(c, 1), arg(c, 2), arg(c, 3));
    retStd(c, static_cast<uint32_t>(r), 4);
}

LRESULT CALLBACK hostWndProcImpl(HWND h, UINT msg, WPARAM wp, LPARAM lp, bool wide) {
    uint32_t proc = guestProcFor(h);
    if (!proc && !t_pendingClassProc.empty()) proc = t_pendingClassProc.back();  // messages before the atom is queryable
    if (!proc) return wide ? DefWindowProcW(h, msg, wp, lp) : DefWindowProcA(h, msg, wp, lp);
    return dispatchToGuest(proc, h, msg, wp, lp);
}
LRESULT CALLBACK hostWndProcA(HWND h, UINT m, WPARAM w, LPARAM l) { return hostWndProcImpl(h, m, w, l, false); }
LRESULT CALLBACK hostWndProcW(HWND h, UINT m, WPARAM w, LPARAM l) { return hostWndProcImpl(h, m, w, l, true); }

INT_PTR CALLBACK hostDlgProc(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    uint32_t proc = 0;
    {
        std::lock_guard<std::mutex> l(g_wpLock);
        auto it = g_dialogProc.find(h);
        if (it != g_dialogProc.end()) proc = it->second;
        else if (t_pendingDlgProc) { proc = t_pendingDlgProc; g_dialogProc[h] = proc; }
    }
    if (!proc) return FALSE;
    if (msg == WM_NCDESTROY) {
        std::lock_guard<std::mutex> l(g_wpLock);
        g_dialogProc.erase(h);
    }
    return static_cast<INT_PTR>(static_cast<int32_t>(dispatchToGuest(proc, h, msg, wp, lp)));
}

// ---------------------------------------------------------------------------
// classes and windows
// ---------------------------------------------------------------------------
template <bool W> void registerClass(Ctx* c) {
    const uint32_t g = arg(c, 0);
    using WC = std::conditional_t<W, WNDCLASSEXW, WNDCLASSEXA>;
    using CH = std::conditional_t<W, wchar_t, char>;
    WC wc{};
    wc.cbSize = sizeof wc;
    wc.style = rd32(g + 4);
    const uint32_t guestProc = rd32(g + 8);
    wc.lpfnWndProc = W ? hostWndProcW : hostWndProcA;
    wc.cbClsExtra = rd32(g + 12);
    wc.cbWndExtra = rd32(g + 16);
    wc.hInstance = instFromGuest(rd32(g + 20));
    wc.hIcon = static_cast<HICON>(hh(rd32(g + 24)));
    wc.hCursor = static_cast<HCURSOR>(hh(rd32(g + 28)));
    wc.hbrBackground = static_cast<HBRUSH>(hh(rd32(g + 32)));
    wc.lpszMenuName = reinterpret_cast<const CH*>(static_cast<uintptr_t>(rd32(g + 36)));
    wc.lpszClassName = reinterpret_cast<const CH*>(static_cast<uintptr_t>(rd32(g + 40)));
    wc.hIconSm = static_cast<HICON>(hh(rd32(g + 44)));
    ATOM a;
    if constexpr (W) a = RegisterClassExW(&wc);
    else a = RegisterClassExA(&wc);
    if (a) {
        std::lock_guard<std::mutex> l(g_wpLock);
        g_classProc[a] = guestProc;
    }
    HLOG(1, "RegisterClassEx%c(proc 0x%08X) -> atom 0x%X", W ? 'W' : 'A', guestProc, a);
    retStd(c, a, 1);
}
IMPORT(U, RegisterClassExA) { registerClass<false>(c); }
IMPORT(U, RegisterClassExW) { registerClass<true>(c); }

uint32_t classProcByName(uint32_t cls, bool wide, HINSTANCE inst) {
    ATOM a = 0;
    if (cls < 0x10000) a = static_cast<ATOM>(cls);
    else if (wide) { WNDCLASSEXW wc{sizeof wc}; a = static_cast<ATOM>(GetClassInfoExW(inst, gp<wchar_t>(cls), &wc)); }
    else { WNDCLASSEXA wc{sizeof wc}; a = static_cast<ATOM>(GetClassInfoExA(inst, gp<char>(cls), &wc)); }
    std::lock_guard<std::mutex> l(g_wpLock);
    auto it = g_classProc.find(a);
    return it == g_classProc.end() ? 0 : it->second;
}

template <bool W> void createWindow(Ctx* c) {
    const uint32_t cls = arg(c, 1);
    HINSTANCE inst = instFromGuest(arg(c, 10));
    t_pendingClassProc.push_back(classProcByName(cls, W, inst));
    HWND h;
    if constexpr (W)
        h = CreateWindowExW(arg(c, 0), reinterpret_cast<LPCWSTR>(static_cast<uintptr_t>(cls)), argp<wchar_t>(c, 2), arg(c, 3), arg(c, 4),
                            arg(c, 5), arg(c, 6), arg(c, 7), static_cast<HWND>(hh(arg(c, 8))), static_cast<HMENU>(hh(arg(c, 9))), inst,
                            reinterpret_cast<void*>(static_cast<uintptr_t>(arg(c, 11))));
    else
        h = CreateWindowExA(arg(c, 0), reinterpret_cast<LPCSTR>(static_cast<uintptr_t>(cls)), argp(c, 2), arg(c, 3), arg(c, 4), arg(c, 5),
                            arg(c, 6), arg(c, 7), static_cast<HWND>(hh(arg(c, 8))), static_cast<HMENU>(hh(arg(c, 9))), inst,
                            reinterpret_cast<void*>(static_cast<uintptr_t>(arg(c, 11))));
    t_pendingClassProc.pop_back();
    HLOG(1, "CreateWindowEx%c(%dx%d style 0x%X) -> %p (error %lu)", W ? 'W' : 'A', arg(c, 6), arg(c, 7), arg(c, 3), h, h ? 0 : GetLastError());
    retStd(c, gh(h), 12);
}
IMPORT(U, CreateWindowExA) { createWindow<false>(c); }
IMPORT(U, CreateWindowExW) { createWindow<true>(c); }

IMPORT(U, UnregisterClassA) {
    retStd(c, UnregisterClassA(reinterpret_cast<LPCSTR>(static_cast<uintptr_t>(arg(c, 0))), instFromGuest(arg(c, 1))), 2);
}
IMPORT(U, DefWindowProcA) {
    retStd(c, static_cast<uint32_t>(callDefault<false>(nullptr, static_cast<HWND>(hh(arg(c, 0))), arg(c, 1), arg(c, 2), arg(c, 3))), 4);
}
IMPORT(U, DefWindowProcW) {
    retStd(c, static_cast<uint32_t>(callDefault<true>(nullptr, static_cast<HWND>(hh(arg(c, 0))), arg(c, 1), arg(c, 2), arg(c, 3))), 4);
}
IMPORT(U, CallWindowProcA) {
    const uint32_t proc = arg(c, 0);
    HWND h = static_cast<HWND>(hh(arg(c, 1)));
    if (proc >= kTrapBase) {
        // A host procedure handed out earlier as a trap.
        WNDPROC hp = nullptr;
        {
            std::lock_guard<std::mutex> l(g_wpLock);
            for (auto& [p, t] : g_hostProcTraps)
                if (t == proc) hp = p;
        }
        retStd(c, static_cast<uint32_t>(callDefault<false>(hp, h, arg(c, 2), arg(c, 3), arg(c, 4))), 5);
        return;
    }
    retStd(c, guestCall(proc, {arg(c, 1), arg(c, 2), arg(c, 3), arg(c, 4)}), 5);
}

bool isDialog(HWND h) {
    char cls[16] = {};
    GetClassNameA(h, cls, sizeof cls);
    return std::strcmp(cls, "#32770") == 0;
}

uint32_t getWndProc(HWND h) {
    if (uint32_t p = guestProcFor(h)) {
        std::lock_guard<std::mutex> l(g_wpLock);
        if (g_windowProc.count(h)) return p;
    }
    const WNDPROC hp = reinterpret_cast<WNDPROC>(GetWindowLongPtrA(h, GWLP_WNDPROC));
    if (hp == hostWndProcA || hp == hostWndProcW) return guestProcFor(h);
    return trapForHostProc(hp);
}

void setWindowLong(Ctx* c, bool wide) {
    HWND h = static_cast<HWND>(hh(arg(c, 0)));
    const int idx = static_cast<int32_t>(arg(c, 1));
    const uint32_t v = arg(c, 2);
    uint32_t prev = 0;
    if (idx == GWLP_WNDPROC) {
        prev = getWndProc(h);
        if (v >= kTrapBase) {
            // Restoring a host procedure obtained earlier.
            std::lock_guard<std::mutex> l(g_wpLock);
            g_windowProc.erase(h);
            for (auto& [hp, t] : g_hostProcTraps)
                if (t == v) SetWindowLongPtrA(h, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hp));
        } else {
            {
                std::lock_guard<std::mutex> l(g_wpLock);
                g_windowProc[h] = v;
            }
            SetWindowLongPtrW(h, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wide || IsWindowUnicode(h) ? hostWndProcW : hostWndProcA));
        }
    } else if (idx == GWLP_USERDATA || idx == GWLP_HINSTANCE || idx == GWLP_HWNDPARENT || idx == GWLP_ID) {
        prev = static_cast<uint32_t>(SetWindowLongPtrA(h, idx, static_cast<LONG_PTR>(static_cast<int32_t>(v))));
    } else if (idx >= 0 && isDialog(h) && idx <= 8) {
        // x86 DWL_MSGRESULT 0 / DWL_DLGPROC 4 / DWL_USER 8 -> x64 offsets
        if (idx == 4) {
            std::lock_guard<std::mutex> l(g_wpLock);
            prev = g_dialogProc[h];
            g_dialogProc[h] = v;
        } else {
            prev = static_cast<uint32_t>(SetWindowLongPtrA(h, idx == 0 ? DWLP_MSGRESULT : DWLP_USER, static_cast<LONG_PTR>(static_cast<int32_t>(v))));
        }
    } else {
        prev = static_cast<uint32_t>(SetWindowLongA(h, idx, static_cast<LONG>(v)));
    }
    retStd(c, prev, 3);
}
IMPORT(U, SetWindowLongA) { setWindowLong(c, false); }
IMPORT(U, GetWindowLongA) {
    HWND h = static_cast<HWND>(hh(arg(c, 0)));
    const int idx = static_cast<int32_t>(arg(c, 1));
    uint32_t r;
    if (idx == GWLP_WNDPROC) r = getWndProc(h);
    else if (idx == GWLP_HINSTANCE) {
        const HINSTANCE i = reinterpret_cast<HINSTANCE>(GetWindowLongPtrA(h, idx));
        r = i == g_hinst ? kImageBase : gh(i);
    } else if (idx == GWLP_USERDATA || idx == GWLP_HWNDPARENT || idx == GWLP_ID) r = static_cast<uint32_t>(GetWindowLongPtrA(h, idx));
    else if (idx >= 0 && idx <= 8 && isDialog(h)) {
        if (idx == 4) { std::lock_guard<std::mutex> l(g_wpLock); r = g_dialogProc[h]; }
        else r = static_cast<uint32_t>(GetWindowLongPtrA(h, idx == 0 ? DWLP_MSGRESULT : DWLP_USER));
    } else r = static_cast<uint32_t>(GetWindowLongA(h, idx));
    retStd(c, r, 2);
}

// ---------------------------------------------------------------------------
// message loop
// ---------------------------------------------------------------------------
// Guest MSG (28 bytes): hwnd, message, wParam, lParam, time, pt.x, pt.y
thread_local MSG t_lastMsg;
void msgToGuest(uint32_t g, const MSG& m) {
    wr32(g, gh(m.hwnd)); wr32(g + 4, m.message); wr32(g + 8, static_cast<uint32_t>(m.wParam)); wr32(g + 12, static_cast<uint32_t>(m.lParam));
    wr32(g + 16, m.time); wr32(g + 20, m.pt.x); wr32(g + 24, m.pt.y);
}
MSG msgFromGuest(uint32_t g) {
    MSG m{};
    m.hwnd = static_cast<HWND>(hh(rd32(g)));
    m.message = rd32(g + 4);
    m.wParam = rd32(g + 8);
    m.lParam = static_cast<LPARAM>(static_cast<int32_t>(rd32(g + 12)));
    m.time = rd32(g + 16);
    m.pt.x = static_cast<LONG>(rd32(g + 20));
    m.pt.y = static_cast<LONG>(rd32(g + 24));
    // Same message as the last one retrieved: keep its full-width lParam.
    if (t_lastMsg.hwnd == m.hwnd && t_lastMsg.message == m.message && static_cast<uint32_t>(t_lastMsg.lParam) == rd32(g + 12)) {
        m.lParam = t_lastMsg.lParam;
        m.wParam = t_lastMsg.wParam;
    }
    return m;
}
IMPORT(U, PeekMessageA) {
    MSG m;
    const BOOL r = PeekMessageA(&m, static_cast<HWND>(hh(arg(c, 1))), arg(c, 2), arg(c, 3), arg(c, 4));
    if (r) { t_lastMsg = m; msgToGuest(arg(c, 0), m); }
    retStd(c, r, 5);
}
IMPORT(U, PeekMessageW) {
    MSG m;
    const BOOL r = PeekMessageW(&m, static_cast<HWND>(hh(arg(c, 1))), arg(c, 2), arg(c, 3), arg(c, 4));
    if (r) { t_lastMsg = m; msgToGuest(arg(c, 0), m); }
    retStd(c, r, 5);
}
IMPORT(U, GetMessageA) {
    MSG m;
    const BOOL r = GetMessageA(&m, static_cast<HWND>(hh(arg(c, 1))), arg(c, 2), arg(c, 3));
    if (r != -1) { t_lastMsg = m; msgToGuest(arg(c, 0), m); }
    retStd(c, static_cast<uint32_t>(r), 4);
}
IMPORT(U, TranslateMessage) {
    const MSG m = msgFromGuest(arg(c, 0));
    retStd(c, TranslateMessage(&m), 1);
}
IMPORT(U, DispatchMessageA) {
    const MSG m = msgFromGuest(arg(c, 0));
    retStd(c, static_cast<uint32_t>(DispatchMessageA(&m)), 1);
}
IMPORT(U, DispatchMessageW) {
    const MSG m = msgFromGuest(arg(c, 0));
    retStd(c, static_cast<uint32_t>(DispatchMessageW(&m)), 1);
}
IMPORT(U, SendMessageA) {
    retStd(c, static_cast<uint32_t>(SendMessageA(static_cast<HWND>(hh(arg(c, 0))), arg(c, 1), arg(c, 2), static_cast<int32_t>(arg(c, 3)))), 4);
}
IMPORT(U, SendMessageW) {
    retStd(c, static_cast<uint32_t>(SendMessageW(static_cast<HWND>(hh(arg(c, 0))), arg(c, 1), arg(c, 2), static_cast<int32_t>(arg(c, 3)))), 4);
}
IMPORT(U, PostMessageA) {
    retStd(c, PostMessageA(static_cast<HWND>(hh(arg(c, 0))), arg(c, 1), arg(c, 2), static_cast<int32_t>(arg(c, 3))), 4);
}
IMPORT(U, PostThreadMessageW) { retStd(c, PostThreadMessageW(arg(c, 0), arg(c, 1), arg(c, 2), static_cast<int32_t>(arg(c, 3))), 4); }
IMPORT(U, MsgWaitForMultipleObjects) {
    const uint32_t n = arg(c, 0);
    HANDLE hs[MAXIMUM_WAIT_OBJECTS];
    for (uint32_t i = 0; i < n && i < MAXIMUM_WAIT_OBJECTS; ++i) hs[i] = hh(rd32(arg(c, 1) + 4 * i));
    retStd(c, MsgWaitForMultipleObjects(n, hs, arg(c, 2), arg(c, 3), arg(c, 4)), 5);
}

// ---------------------------------------------------------------------------
// dialogs
// ---------------------------------------------------------------------------
IMPORT(U, DialogBoxParamA) {
    t_pendingDlgProc = arg(c, 3);
    HMODULE m = moduleFromGuest(arg(c, 0));
    const INT_PTR r = DialogBoxParamA(m, reinterpret_cast<LPCSTR>(static_cast<uintptr_t>(arg(c, 1))), static_cast<HWND>(hh(arg(c, 2))),
                                      hostDlgProc, static_cast<int32_t>(arg(c, 4)));
    t_pendingDlgProc = 0;
    retStd(c, static_cast<uint32_t>(r), 5);
}
IMPORT(U, DialogBoxIndirectParamA) {
    t_pendingDlgProc = arg(c, 3);
    const INT_PTR r = DialogBoxIndirectParamA(g_hinst, argp<DLGTEMPLATE>(c, 1), static_cast<HWND>(hh(arg(c, 2))), hostDlgProc,
                                              static_cast<int32_t>(arg(c, 4)));
    t_pendingDlgProc = 0;
    retStd(c, static_cast<uint32_t>(r), 5);
}
IMPORT(U, EndDialog) { retStd(c, EndDialog(static_cast<HWND>(hh(arg(c, 0))), static_cast<int32_t>(arg(c, 1))), 2); }

// ---------------------------------------------------------------------------
// resources
// ---------------------------------------------------------------------------
HANDLE loadImage(uint32_t inst, uint32_t name, UINT type, bool wide) {
    HMODULE m = inst ? moduleFromGuest(inst) : nullptr;
    const UINT flags = LR_DEFAULTSIZE | LR_SHARED;
    if (wide) return LoadImageW(m, reinterpret_cast<LPCWSTR>(static_cast<uintptr_t>(name)), type, 0, 0, flags);
    return LoadImageA(m, reinterpret_cast<LPCSTR>(static_cast<uintptr_t>(name)), type, 0, 0, flags);
}
IMPORT(U, LoadIconA) { retStd(c, gh(loadImage(arg(c, 0), arg(c, 1), IMAGE_ICON, false)), 2); }
IMPORT(U, LoadIconW) { retStd(c, gh(loadImage(arg(c, 0), arg(c, 1), IMAGE_ICON, true)), 2); }
IMPORT(U, LoadCursorA) { retStd(c, gh(loadImage(arg(c, 0), arg(c, 1), IMAGE_CURSOR, false)), 2); }
IMPORT(U, LoadCursorW) { retStd(c, gh(loadImage(arg(c, 0), arg(c, 1), IMAGE_CURSOR, true)), 2); }
IMPORT(U, LoadStringA) { retStd(c, LoadStringA(moduleFromGuest(arg(c, 0)), arg(c, 1), argp(c, 2), arg(c, 3)), 4); }

// ---------------------------------------------------------------------------
// formatting
// ---------------------------------------------------------------------------
IMPORT(U, wsprintfA) {
    uint32_t ap = c->esp + 12;
    const std::string s = formatA(argp(c, 1), ap);
    const size_t n = s.size() < 1024 ? s.size() : 1023;
    std::memcpy(argp(c, 0), s.c_str(), n);
    wr8(arg(c, 0) + static_cast<uint32_t>(n), 0);
    retCdecl(c, static_cast<uint32_t>(n));
}
IMPORT(U, wsprintfW) {
    uint32_t ap = c->esp + 12;
    const std::wstring s = formatW(argp<wchar_t>(c, 1), ap);
    const size_t n = s.size() < 1024 ? s.size() : 1023;
    std::memcpy(argp(c, 0), s.c_str(), n * 2);
    wr16(arg(c, 0) + static_cast<uint32_t>(n * 2), 0);
    retCdecl(c, static_cast<uint32_t>(n));
}
IMPORT(U, wvsprintfW) {
    uint32_t ap = arg(c, 2);
    const std::wstring s = formatW(argp<wchar_t>(c, 1), ap);
    const size_t n = s.size() < 1024 ? s.size() : 1023;
    std::memcpy(argp(c, 0), s.c_str(), n * 2);
    wr16(arg(c, 0) + static_cast<uint32_t>(n * 2), 0);
    retStd(c, static_cast<uint32_t>(n), 3);
}

IMPORT(U, PtInRect) {
    POINT p{static_cast<LONG>(arg(c, 1)), static_cast<LONG>(arg(c, 2))};
    retStd(c, PtInRect(argp<RECT>(c, 0), p), 3);
}
IMPORT(U, MessageBoxA) {
    log("MessageBoxA: %s: %s", arg(c, 2) ? argp(c, 2) : "", arg(c, 1) ? argp(c, 1) : "");
    retStd(c, MessageBoxA(static_cast<HWND>(hh(arg(c, 0))), argp(c, 1), argp(c, 2), arg(c, 3)), 4);
}
IMPORT(U, MessageBoxW) {
    log("MessageBoxW: %s: %s", narrow(argp<wchar_t>(c, 2)).c_str(), narrow(argp<wchar_t>(c, 1)).c_str());
    retStd(c, MessageBoxW(static_cast<HWND>(hh(arg(c, 0))), argp<wchar_t>(c, 1), argp<wchar_t>(c, 2), arg(c, 3)), 4);
}

// ---------------------------------------------------------------------------
// forwards
// ---------------------------------------------------------------------------
FWD_STD(U, IsWindowUnicode);
FWD_STD(U, GetCaretBlinkTime);
FWD_STD(U, GetFocus);
FWD_STD(U, GetAsyncKeyState);
FWD_STD(U, GetKeyState);
FWD_STD(U, RemovePropA);
FWD_STD(U, GetPropA);
FWD_STD(U, SetPropA);
FWD_STD(U, SetCursor);
FWD_STD(U, ReleaseCapture);
FWD_STD(U, ClientToScreen);
FWD_STD(U, GetWindowRect);
FWD_STD(U, SetCapture);
FWD_STD(U, InvalidateRect);
FWD_STD(U, GetCapture);
FWD_STD(U, GetParent);
FWD_STD(U, EnableWindow);
FWD_STD(U, GetDlgItem);
FWD_STD(U, SetDlgItemTextA);
FWD_STD(U, SetWindowTextA);
FWD_STD(U, MoveWindow);
FWD_STD(U, GetClientRect);
FWD_STD(U, GetDesktopWindow);
FWD_STD(U, DestroyWindow);
FWD_STD(U, IsDlgButtonChecked);
FWD_STD(U, ShowCursor);
FWD_STD(U, ShowWindow);
FWD_STD(U, GetSysColor);
FWD_STD(U, GetForegroundWindow);
FWD_STD(U, WaitMessage);
FWD_STD(U, UpdateWindow);
FWD_STD(U, GetSystemMetrics);
FWD_STD(U, CharNextExA);
FWD_STD(U, SetCursorPos);
FWD_STD(U, OffsetRect);
FWD_STD(U, GetCursorPos);
FWD_STD(U, GetQueueStatus);
FWD_STD(U, RegisterWindowMessageW);
FWD_STD(U, PostQuitMessage);
FWD_STD(U, GetKeyboardLayout);

}  // namespace
}  // namespace host
