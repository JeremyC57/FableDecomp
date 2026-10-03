// USER32 on SDL2: window classes, windows (top-level ones are SDL windows), per-thread
// message queues fed from SDL events, keyboard/mouse state, cursor, focus, simple dialogs
// and message boxes. Plus the GDI and IMM32 calls the game makes (stubs).
#include <SDL.h>  // before the Windows headers: SDL keys its intrinsics on MinGW macros

#include "keymap.hpp"
#include "w32.hpp"
#include "w32sdl.hpp"

#include <chrono>
#include <cstring>
#include <deque>
#include <map>
#include <thread>

namespace w32 {

namespace {

struct WndClass {
    ATOM atom = 0;
    std::string name;  // lower case
    WNDPROC proc = nullptr;
    bool unicode = false;
    UINT style = 0;
    int wndExtra = 0;
    HINSTANCE inst = nullptr;
};

struct Wnd {
    HWND h = nullptr;
    ATOM atom = 0;
    WNDPROC proc = nullptr;
    bool unicode = false;
    DWORD style = 0, exStyle = 0;
    RECT rect{};  // screen coordinates
    HWND parent = nullptr;
    std::string title;
    std::map<std::string, HANDLE> props;
    LONG_PTR userData = 0, id = 0;
    HINSTANCE inst = nullptr;
    std::vector<uint8_t> extra;
    SDL_Window* sdl = nullptr;
    DWORD thread = 0;
    bool visible = false, enabled = true;
    // dialogs
    bool dialog = false;
    DLGPROC dlgProc = nullptr;
    LONG_PTR dlgUser = 0, msgResult = 0;
    bool ended = false;
    INT_PTR result = 0;
};
using WndPtr = std::shared_ptr<Wnd>;

struct Queue {
    std::deque<MSG> msgs;
    bool quit = false;
    int quitCode = 0;
};

std::recursive_mutex g_lock;  // windows, classes, queues
std::map<ATOM, WndClass> g_classes;
ATOM g_nextAtom = 0xC000;
std::map<HWND, WndPtr> g_windows;
uintptr_t g_nextHwnd = 0x20000;
std::map<DWORD, Queue> g_queues;
std::condition_variable_any g_queueCv;
std::map<std::string, UINT> g_messages;  // RegisterWindowMessage
UINT g_nextMessage = 0xC000;

DWORD g_sdlThread;  // thread that owns SDL video
HWND g_focus, g_active, g_capture;
int g_cursorCount = 0;
uint8_t g_keys[256];         // GetKeyState: high bit down, low bit toggled
std::vector<std::function<void(const SDL_Event&)>> g_eventHooks;

const HWND kDesktop = reinterpret_cast<HWND>(static_cast<uintptr_t>(0x10010));

std::string lc(const std::string& s) {
    std::string r = s;
    for (auto& c : r) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return r;
}
std::string strA(LPCSTR s) { return reinterpret_cast<uintptr_t>(s) < 0x10000 ? std::string() : acpToUtf8(s); }
std::string strW(LPCWSTR s) { return reinterpret_cast<uintptr_t>(s) < 0x10000 ? std::string() : toUtf8(s); }

WndPtr wnd(HWND h) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    auto it = g_windows.find(h);
    return it == g_windows.end() ? nullptr : it->second;
}
WndPtr wndBySdl(uint32_t id) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    for (auto& [h, w] : g_windows)
        if (w->sdl && SDL_GetWindowID(w->sdl) == id) return w;
    return nullptr;
}
WndPtr mainWindow() {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    for (auto& [h, w] : g_windows)
        if (w->sdl) return w;
    return nullptr;
}

void ensureSdl() {
    if (SDL_WasInit(SDL_INIT_VIDEO)) return;
    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
    if (SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) std::fprintf(stderr, "SDL_Init(video) failed: %s\n", SDL_GetError());
    g_sdlThread = currentTid();
}

void post(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    DWORD tid = g_sdlThread;
    if (auto w = g_windows.find(h); w != g_windows.end()) tid = w->second->thread;
    MSG m{};
    m.hwnd = h;
    m.message = msg;
    m.wParam = wp;
    m.lParam = lp;
    m.time = GetTickCount();
    POINT p;
    GetCursorPos(&p);
    m.pt = p;
    g_queues[tid].msgs.push_back(m);
    g_queueCv.notify_all();
}

LRESULT send(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    WndPtr w = wnd(h);
    if (!w || !w->proc) return 0;
    return w->proc(h, msg, wp, lp);
}

WPARAM mouseKeys() {
    const uint32_t b = SDL_GetMouseState(nullptr, nullptr);
    WPARAM k = 0;
    if (b & SDL_BUTTON_LMASK) k |= MK_LBUTTON;
    if (b & SDL_BUTTON_RMASK) k |= MK_RBUTTON;
    if (b & SDL_BUTTON_MMASK) k |= MK_MBUTTON;
    if (g_keys[VK_SHIFT] & 0x80) k |= MK_SHIFT;
    if (g_keys[VK_CONTROL] & 0x80) k |= MK_CONTROL;
    return k;
}

void setKey(uint8_t vk, bool down) {
    if (down && !(g_keys[vk] & 0x80)) g_keys[vk] ^= 1;  // toggle state flips on press
    g_keys[vk] = static_cast<uint8_t>((g_keys[vk] & 1) | (down ? 0x80 : 0));
}

void translate(const SDL_Event& e) {
    for (auto& hook : g_eventHooks) hook(e);
    switch (e.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            WndPtr w = wndBySdl(e.key.windowID);
            if (!w) w = mainWindow();
            if (!w) break;
            const KeyInfo k = keyInfo(e.key.keysym.scancode);
            if (!k.vk) break;
            const bool down = e.type == SDL_KEYDOWN;
            const bool wasDown = g_keys[genericVk(k.vk)] & 0x80;
            setKey(k.vk, down);
            setKey(genericVk(k.vk), down || (genericVk(k.vk) != k.vk && (g_keys[k.vk ^ 1] & 0x80)));
            const bool alt = g_keys[VK_MENU] & 0x80;
            LPARAM lp = 1 | (static_cast<LPARAM>(k.scan) << 16) | (k.extended ? 1 << 24 : 0) | (alt ? 1 << 29 : 0);
            if (down) lp |= wasDown ? (1u << 30) : 0;
            else lp |= (1u << 30) | (1u << 31);
            const bool sys = alt || k.vk == VK_F10 || genericVk(k.vk) == VK_MENU;
            const UINT msg = down ? (sys ? WM_SYSKEYDOWN : WM_KEYDOWN) : (sys ? WM_SYSKEYUP : WM_KEYUP);
            post(w->h, msg, genericVk(k.vk), static_cast<LPARAM>(static_cast<int32_t>(lp)));
            // Control characters that SDL does not deliver as text input
            if (down && (k.vk == VK_RETURN || k.vk == VK_BACK || k.vk == VK_ESCAPE || k.vk == VK_TAB))
                post(w->h, WM_CHAR, k.vk == VK_RETURN ? '\r' : k.vk == VK_BACK ? 8 : k.vk == VK_ESCAPE ? 27 : '\t', 1);
            break;
        }
        case SDL_TEXTINPUT: {
            WndPtr w = wndBySdl(e.text.windowID);
            if (!w) w = mainWindow();
            if (!w) break;
            const wstr t = fromUtf8(e.text.text);
            for (wchar_t ch : t) post(w->h, WM_CHAR, static_cast<uint16_t>(ch), 1);
            break;
        }
        case SDL_MOUSEMOTION: {
            WndPtr w = wndBySdl(e.motion.windowID);
            if (!w) break;
            post(w->h, WM_MOUSEMOVE, mouseKeys(), MAKELPARAM(e.motion.x, e.motion.y));
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            WndPtr w = wndBySdl(e.button.windowID);
            if (!w) break;
            const bool down = e.type == SDL_MOUSEBUTTONDOWN;
            UINT msg = 0;
            uint8_t vk = 0;
            switch (e.button.button) {
                case SDL_BUTTON_LEFT: msg = down ? WM_LBUTTONDOWN : WM_LBUTTONUP; vk = VK_LBUTTON; break;
                case SDL_BUTTON_RIGHT: msg = down ? WM_RBUTTONDOWN : WM_RBUTTONUP; vk = VK_RBUTTON; break;
                case SDL_BUTTON_MIDDLE: msg = down ? WM_MBUTTONDOWN : WM_MBUTTONUP; vk = VK_MBUTTON; break;
                default: break;
            }
            if (!msg) break;
            setKey(vk, down);
            post(w->h, msg, mouseKeys(), MAKELPARAM(e.button.x, e.button.y));
            break;
        }
        case SDL_MOUSEWHEEL: {
            WndPtr w = wndBySdl(e.wheel.windowID);
            if (!w) break;
            int x = 0, y = 0;
            SDL_GetMouseState(&x, &y);
            post(w->h, WM_MOUSEWHEEL, MAKEWPARAM(mouseKeys(), static_cast<int16_t>(e.wheel.y * WHEEL_DELTA)), MAKELPARAM(x, y));
            break;
        }
        case SDL_WINDOWEVENT: {
            WndPtr w = wndBySdl(e.window.windowID);
            if (!w) break;
            switch (e.window.event) {
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    g_focus = g_active = w->h;
                    post(w->h, WM_ACTIVATEAPP, TRUE, 0);
                    post(w->h, WM_ACTIVATE, WA_ACTIVE, 0);
                    post(w->h, WM_SETFOCUS, 0, 0);
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    std::memset(g_keys, 0, sizeof g_keys);
                    g_focus = g_active = nullptr;
                    post(w->h, WM_KILLFOCUS, 0, 0);
                    post(w->h, WM_ACTIVATE, WA_INACTIVE, 0);
                    post(w->h, WM_ACTIVATEAPP, FALSE, 0);
                    break;
                case SDL_WINDOWEVENT_SIZE_CHANGED: {
                    int x, y;
                    SDL_GetWindowPosition(w->sdl, &x, &y);
                    w->rect = {x, y, x + e.window.data1, y + e.window.data2};
                    post(w->h, WM_SIZE, SIZE_RESTORED, MAKELPARAM(e.window.data1, e.window.data2));
                    break;
                }
                case SDL_WINDOWEVENT_MOVED:
                    w->rect = {e.window.data1, e.window.data2, e.window.data1 + (w->rect.right - w->rect.left), e.window.data2 + (w->rect.bottom - w->rect.top)};
                    post(w->h, WM_MOVE, 0, MAKELPARAM(e.window.data1, e.window.data2));
                    break;
                case SDL_WINDOWEVENT_MINIMIZED: post(w->h, WM_SIZE, SIZE_MINIMIZED, 0); break;
                case SDL_WINDOWEVENT_RESTORED: post(w->h, WM_SIZE, SIZE_RESTORED, MAKELPARAM(w->rect.right - w->rect.left, w->rect.bottom - w->rect.top)); break;
                case SDL_WINDOWEVENT_CLOSE: post(w->h, WM_CLOSE, 0, 0); break;
                case SDL_WINDOWEVENT_EXPOSED: post(w->h, WM_PAINT, 0, 0); break;
                default: break;
            }
            break;
        }
        case SDL_QUIT:
            if (WndPtr w = mainWindow()) post(w->h, WM_CLOSE, 0, 0);
            break;
        default: break;
    }
}

void pump() {
    if (!g_sdlThread || currentTid() != g_sdlThread) return;
    SDL_Event e;
    while (SDL_PollEvent(&e)) translate(e);
}

bool matches(const MSG& m, HWND h, UINT lo, UINT hi) {
    if (h && h != reinterpret_cast<HWND>(-1) && m.hwnd != h) return false;
    if (h == reinterpret_cast<HWND>(-1) && m.hwnd) return false;
    if (lo == 0 && hi == 0) return true;
    return m.message >= lo && m.message <= hi;
}

BOOL peek(MSG* out, HWND h, UINT lo, UINT hi, UINT remove) {
    pump();
    std::lock_guard<std::recursive_mutex> l(g_lock);
    Queue& q = g_queues[currentTid()];
    for (auto it = q.msgs.begin(); it != q.msgs.end(); ++it)
        if (matches(*it, h, lo, hi)) {
            *out = *it;
            if (remove & PM_REMOVE) q.msgs.erase(it);
            return TRUE;
        }
    if (q.quit) {
        std::memset(out, 0, sizeof *out);
        out->message = WM_QUIT;
        out->wParam = static_cast<WPARAM>(q.quitCode);
        if (remove & PM_REMOVE) q.quit = false;
        return TRUE;
    }
    return FALSE;
}

void waitForMessage(DWORD ms) {
    const bool sdlThread = g_sdlThread && currentTid() == g_sdlThread;
    if (sdlThread) {
        SDL_Event e;
        if (SDL_WaitEventTimeout(&e, ms == INFINITE ? 10 : static_cast<int>(std::min<DWORD>(ms, 10)))) translate(e);
        return;
    }
    std::unique_lock<std::recursive_mutex> l(g_lock);
    g_queueCv.wait_for(l, std::chrono::milliseconds(ms == INFINITE ? 10 : std::min<DWORD>(ms, 10)));
}

HWND createWindow(DWORD exStyle, const std::string& cls, ATOM clsAtom, const std::string& title, DWORD style, int x, int y, int w, int h,
                  HWND parent, HMENU menu, HINSTANCE inst, LPVOID param, bool unicode, LPCVOID className) {
    WndClass wc;
    {
        std::lock_guard<std::recursive_mutex> l(g_lock);
        auto it = g_classes.end();
        if (clsAtom) it = g_classes.find(clsAtom);
        else
            for (auto i = g_classes.begin(); i != g_classes.end(); ++i)
                if (i->second.name == lc(cls)) it = i;
        if (it == g_classes.end()) { setError(ERROR_CANNOT_FIND_WND_CLASS); return nullptr; }
        wc = it->second;
    }
    auto win = std::make_shared<Wnd>();
    win->atom = wc.atom;
    win->proc = wc.proc;
    win->unicode = unicode;
    win->style = style;
    win->exStyle = exStyle;
    win->parent = parent;
    win->title = title;
    win->inst = inst;
    win->id = reinterpret_cast<LONG_PTR>(menu);
    win->extra.resize(static_cast<size_t>(wc.wndExtra));
    win->thread = currentTid();
    if (x == CW_USEDEFAULT) x = 0, y = 0;
    if (w == CW_USEDEFAULT) w = 800, h = 600;
    win->rect = {x, y, x + w, y + h};
    {
        std::lock_guard<std::recursive_mutex> l(g_lock);
        win->h = reinterpret_cast<HWND>(g_nextHwnd);
        g_nextHwnd += 4;
        g_windows[win->h] = win;
    }
    const bool topLevel = !(style & WS_CHILD) && parent != HWND_MESSAGE;
    if (topLevel) {
        ensureSdl();
        Uint32 flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI;
        if (!(style & WS_CAPTION)) flags |= SDL_WINDOW_BORDERLESS;
        if (style & WS_THICKFRAME) flags |= SDL_WINDOW_RESIZABLE;
        // The client area: Windows sizes include the frame; SDL sizes are the client.
        RECT r = win->rect;
        RECT frame{0, 0, 0, 0};
        AdjustWindowRectEx(&frame, style, FALSE, exStyle);
        const int cw = std::max<int>(1, (r.right - r.left) - (frame.right - frame.left));
        const int ch = std::max<int>(1, (r.bottom - r.top) - (frame.bottom - frame.top));
        win->sdl = SDL_CreateWindow(title.empty() ? "Fable" : title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, cw, ch, flags);
        if (!win->sdl) std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        else {
            int wx, wy;
            SDL_GetWindowPosition(win->sdl, &wx, &wy);
            win->rect = {wx, wy, wx + cw, wy + ch};
        }
    }
    // WM_NCCREATE / WM_CREATE with a CREATESTRUCT of the caller's character width
    CREATESTRUCTW cs{};
    cs.lpCreateParams = param;
    cs.hInstance = inst;
    cs.hMenu = menu;
    cs.hwndParent = parent;
    cs.cx = w;
    cs.cy = h;
    cs.x = x;
    cs.y = y;
    cs.style = static_cast<LONG>(style);
    cs.lpszClass = static_cast<LPCWSTR>(className);
    cs.dwExStyle = exStyle;
    if (!send(win->h, WM_NCCREATE, 0, reinterpret_cast<LPARAM>(&cs))) {
        DestroyWindow(win->h);
        return nullptr;
    }
    send(win->h, WM_NCCALCSIZE, FALSE, reinterpret_cast<LPARAM>(&win->rect));
    if (send(win->h, WM_CREATE, 0, reinterpret_cast<LPARAM>(&cs)) == -1) {
        DestroyWindow(win->h);
        return nullptr;
    }
    send(win->h, WM_SIZE, SIZE_RESTORED, MAKELPARAM(win->rect.right - win->rect.left, win->rect.bottom - win->rect.top));
    if (style & WS_VISIBLE) ShowWindow(win->h, SW_SHOW);
    return win->h;
}

ATOM registerClass(const std::string& name, WNDPROC proc, bool unicode, UINT style, int wndExtra, HINSTANCE inst) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    for (auto& [a, c] : g_classes)
        if (c.name == lc(name)) { setError(ERROR_CLASS_ALREADY_EXISTS); return 0; }
    WndClass c;
    c.atom = g_nextAtom++;
    c.name = lc(name);
    c.proc = proc;
    c.unicode = unicode;
    c.style = style;
    c.wndExtra = wndExtra;
    c.inst = inst;
    g_classes[c.atom] = c;
    return c.atom;
}

}  // namespace

void* sdlWindow(HWND h) {
    WndPtr w = wnd(h);
    while (w && !w->sdl && w->parent) w = wnd(w->parent);
    return w ? w->sdl : nullptr;
}
void setFullscreenDesktop(HWND h, bool on) {
    WndPtr w = wnd(h);
    if (w && w->sdl) SDL_SetWindowFullscreen(w->sdl, on ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}
HWND hwndForSdl(void* sdl) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    for (auto& [h, w] : g_windows)
        if (w->sdl == sdl) return h;
    return nullptr;
}
void addSdlEventHook(std::function<void(const SDL_Event&)> fn) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    g_eventHooks.push_back(std::move(fn));
}
void pumpSdlEvents() { pump(); }
HWND activeWindow() { return g_active; }

}  // namespace w32

using namespace w32;

template <class WC> static BOOL classInfo(const std::string& name, uintptr_t atom, WC* out) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    for (auto& [a, c] : g_classes)
        if ((atom && a == atom) || (!atom && c.name == lc(name))) {
            out->style = c.style;
            out->lpfnWndProc = c.proc;
            out->cbWndExtra = c.wndExtra;
            out->hInstance = c.inst;
            setError(ERROR_SUCCESS);
            return a;
        }
    setError(ERROR_CLASS_DOES_NOT_EXIST);
    return FALSE;
}

extern "C" {

// ---- classes ------------------------------------------------------------------------------
ATOM WINAPI RegisterClassExW(const WNDCLASSEXW* wc) {
    const std::string name = reinterpret_cast<uintptr_t>(wc->lpszClassName) < 0x10000 ? "#" + std::to_string(reinterpret_cast<uintptr_t>(wc->lpszClassName)) : strW(wc->lpszClassName);
    return registerClass(name, wc->lpfnWndProc, true, wc->style, wc->cbWndExtra, wc->hInstance);
}
ATOM WINAPI RegisterClassExA(const WNDCLASSEXA* wc) {
    const std::string name = reinterpret_cast<uintptr_t>(wc->lpszClassName) < 0x10000 ? "#" + std::to_string(reinterpret_cast<uintptr_t>(wc->lpszClassName)) : strA(wc->lpszClassName);
    return registerClass(name, wc->lpfnWndProc, false, wc->style, wc->cbWndExtra, wc->hInstance);
}
BOOL WINAPI UnregisterClassA(LPCSTR name, HINSTANCE) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    for (auto it = g_classes.begin(); it != g_classes.end(); ++it)
        if ((reinterpret_cast<uintptr_t>(name) < 0x10000 && it->first == reinterpret_cast<uintptr_t>(name)) || it->second.name == lc(strA(name))) {
            g_classes.erase(it);
            return TRUE;
        }
    return FALSE;
}
BOOL WINAPI GetClassInfoExW(HINSTANCE, LPCWSTR name, LPWNDCLASSEXW out) {
    const auto v = reinterpret_cast<uintptr_t>(name);
    return classInfo(v < 0x10000 ? std::string() : strW(name), v < 0x10000 ? v : 0, out);
}
BOOL WINAPI GetClassInfoExA(HINSTANCE, LPCSTR name, LPWNDCLASSEXA out) {
    const auto v = reinterpret_cast<uintptr_t>(name);
    return classInfo(v < 0x10000 ? std::string() : strA(name), v < 0x10000 ? v : 0, out);
}
WORD WINAPI GetClassWord(HWND h, int idx) {
    WndPtr w = wnd(h);
    return w && idx == GCW_ATOM ? w->atom : 0;
}
int WINAPI GetClassNameA(HWND h, LPSTR buf, int n) {
    WndPtr w = wnd(h);
    if (!w || n <= 0) return 0;
    std::string name = w->dialog ? "#32770" : "";
    if (!w->dialog) {
        std::lock_guard<std::recursive_mutex> l(g_lock);
        if (auto it = g_classes.find(w->atom); it != g_classes.end()) name = it->second.name;
    }
    std::strncpy(buf, name.c_str(), static_cast<size_t>(n) - 1);
    buf[n - 1] = 0;
    return static_cast<int>(std::strlen(buf));
}

// ---- windows ------------------------------------------------------------------------------
HWND WINAPI CreateWindowExW(DWORD ex, LPCWSTR cls, LPCWSTR title, DWORD style, int x, int y, int w, int h, HWND parent, HMENU menu, HINSTANCE inst, LPVOID param) {
    const auto atom = reinterpret_cast<uintptr_t>(cls);
    return createWindow(ex, atom < 0x10000 ? std::string() : strW(cls), static_cast<ATOM>(atom < 0x10000 ? atom : 0), title ? strW(title) : "", style, x, y, w, h,
                        parent, menu, inst, param, true, cls);
}
HWND WINAPI CreateWindowExA(DWORD ex, LPCSTR cls, LPCSTR title, DWORD style, int x, int y, int w, int h, HWND parent, HMENU menu, HINSTANCE inst, LPVOID param) {
    const auto atom = reinterpret_cast<uintptr_t>(cls);
    return createWindow(ex, atom < 0x10000 ? std::string() : strA(cls), static_cast<ATOM>(atom < 0x10000 ? atom : 0), title ? strA(title) : "", style, x, y, w, h,
                        parent, menu, inst, param, false, cls);
}
BOOL WINAPI DestroyWindow(HWND h) {
    WndPtr w = wnd(h);
    if (!w) return fail(ERROR_INVALID_WINDOW_HANDLE);
    send(h, WM_DESTROY, 0, 0);
    send(h, WM_NCDESTROY, 0, 0);
    if (w->sdl) SDL_DestroyWindow(w->sdl);
    std::lock_guard<std::recursive_mutex> l(g_lock);
    g_windows.erase(h);
    if (g_focus == h) g_focus = nullptr;
    if (g_active == h) g_active = nullptr;
    return TRUE;
}
BOOL WINAPI IsWindow(HWND h) { return wnd(h) != nullptr; }
BOOL WINAPI IsWindowUnicode(HWND h) {
    WndPtr w = wnd(h);
    return w && w->unicode;
}
BOOL WINAPI ShowWindow(HWND h, int cmd) {
    WndPtr w = wnd(h);
    if (!w) return FALSE;
    const bool was = w->visible;
    if (w->sdl) {
        switch (cmd) {
            case SW_HIDE: SDL_HideWindow(w->sdl); break;
            case SW_MINIMIZE: case SW_SHOWMINIMIZED: case SW_SHOWMINNOACTIVE: SDL_MinimizeWindow(w->sdl); break;
            case SW_MAXIMIZE: SDL_MaximizeWindow(w->sdl); break;
            default: SDL_ShowWindow(w->sdl); if (cmd != SW_SHOWNA && cmd != SW_SHOWNOACTIVATE) SDL_RaiseWindow(w->sdl); break;
        }
    }
    w->visible = cmd != SW_HIDE;
    if (w->visible != was) send(h, WM_SHOWWINDOW, w->visible, 0);
    return was;
}
BOOL WINAPI UpdateWindow(HWND h) {
    if (wnd(h)) send(h, WM_PAINT, 0, 0);
    return TRUE;
}
BOOL WINAPI InvalidateRect(HWND h, const RECT*, BOOL) {
    if (wnd(h)) post(h, WM_PAINT, 0, 0);
    return TRUE;
}
BOOL WINAPI EnableWindow(HWND h, BOOL e) {
    WndPtr w = wnd(h);
    if (!w) return FALSE;
    const bool was = !w->enabled;
    w->enabled = e;
    return was;
}
HWND WINAPI GetParent(HWND h) {
    WndPtr w = wnd(h);
    return w ? w->parent : nullptr;
}
HWND WINAPI GetDesktopWindow(void) { return kDesktop; }
BOOL WINAPI GetWindowRect(HWND h, LPRECT r) {
    if (h == kDesktop || !h) {
        SDL_Rect b{0, 0, 1920, 1080};
        if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_GetDisplayBounds(0, &b);
        *r = {b.x, b.y, b.x + b.w, b.y + b.h};
        return TRUE;
    }
    WndPtr w = wnd(h);
    if (!w) return fail(ERROR_INVALID_WINDOW_HANDLE);
    if (w->sdl) {
        int x, y, cw, ch;
        SDL_GetWindowPosition(w->sdl, &x, &y);
        SDL_GetWindowSize(w->sdl, &cw, &ch);
        RECT f{0, 0, 0, 0};
        AdjustWindowRectEx(&f, w->style, FALSE, w->exStyle);
        *r = {x + f.left, y + f.top, x + cw + f.right, y + ch + f.bottom};
    } else {
        *r = w->rect;
    }
    return TRUE;
}
BOOL WINAPI GetClientRect(HWND h, LPRECT r) {
    if (h == kDesktop) return GetWindowRect(h, r);
    WndPtr w = wnd(h);
    if (!w) return fail(ERROR_INVALID_WINDOW_HANDLE);
    if (w->sdl) {
        int cw, ch;
        SDL_GetWindowSize(w->sdl, &cw, &ch);
        *r = {0, 0, cw, ch};
    } else {
        *r = {0, 0, w->rect.right - w->rect.left, w->rect.bottom - w->rect.top};
    }
    return TRUE;
}
BOOL WINAPI ClientToScreen(HWND h, LPPOINT p) {
    WndPtr w = wnd(h);
    if (!w) return FALSE;
    if (w->sdl) {
        int x, y;
        SDL_GetWindowPosition(w->sdl, &x, &y);
        p->x += x, p->y += y;
    } else {
        p->x += w->rect.left, p->y += w->rect.top;
    }
    return TRUE;
}
BOOL WINAPI ScreenToClient(HWND h, LPPOINT p) {
    POINT o{0, 0};
    if (!ClientToScreen(h, &o)) return FALSE;
    p->x -= o.x, p->y -= o.y;
    return TRUE;
}
// Window-rectangle changes: top-level windows move/resize their SDL window (client size).
static void placeWindow(const WndPtr& w, int x, int y, int cx, int cy, UINT flags) {
    if (w->sdl) {
        RECT f{0, 0, 0, 0};
        AdjustWindowRectEx(&f, w->style, FALSE, w->exStyle);
        if (!(flags & SWP_NOSIZE)) SDL_SetWindowSize(w->sdl, std::max(1, cx - (f.right - f.left)), std::max(1, cy - (f.bottom - f.top)));
        if (!(flags & SWP_NOMOVE)) SDL_SetWindowPosition(w->sdl, x - f.left, y - f.top);
        int px, py, pw, ph;
        SDL_GetWindowPosition(w->sdl, &px, &py);
        SDL_GetWindowSize(w->sdl, &pw, &ph);
        w->rect = {px, py, px + pw, py + ph};
    } else {
        if (flags & SWP_NOMOVE) x = w->rect.left, y = w->rect.top;
        if (flags & SWP_NOSIZE) cx = w->rect.right - w->rect.left, cy = w->rect.bottom - w->rect.top;
        w->rect = {x, y, x + cx, y + cy};
    }
}
BOOL WINAPI MoveWindow(HWND h, int x, int y, int cx, int cy, BOOL) {
    WndPtr w = wnd(h);
    if (!w) return FALSE;
    placeWindow(w, x, y, cx, cy, 0);
    send(h, WM_SIZE, SIZE_RESTORED, MAKELPARAM(w->rect.right - w->rect.left, w->rect.bottom - w->rect.top));
    return TRUE;
}
BOOL WINAPI SetWindowPos(HWND h, HWND, int x, int y, int cx, int cy, UINT flags) {
    WndPtr w = wnd(h);
    if (!w) return FALSE;
    if (w->sdl && (flags & SWP_FRAMECHANGED)) SDL_SetWindowBordered(w->sdl, (w->style & WS_CAPTION) ? SDL_TRUE : SDL_FALSE);
    placeWindow(w, x, y, cx, cy, flags);
    if (flags & SWP_SHOWWINDOW) ShowWindow(h, SW_SHOW);
    if (flags & SWP_HIDEWINDOW) ShowWindow(h, SW_HIDE);
    return TRUE;
}
BOOL WINAPI AdjustWindowRectEx(LPRECT r, DWORD style, BOOL menu, DWORD) {
    // A plausible Windows frame for captioned windows; borderless windows are unchanged.
    if (style & WS_CAPTION) {
        r->left -= 8, r->right += 8, r->bottom += 8;
        r->top -= 31;
    } else if (style & (WS_BORDER | WS_THICKFRAME)) {
        r->left -= 1, r->right += 1, r->top -= 1, r->bottom += 1;
    }
    if (menu) r->top -= 20;
    return TRUE;
}
BOOL WINAPI AdjustWindowRect(LPRECT r, DWORD style, BOOL menu) { return AdjustWindowRectEx(r, style, menu, 0); }

static LONG_PTR getLong(HWND h, int idx) {
    WndPtr w = wnd(h);
    if (!w) { setError(ERROR_INVALID_WINDOW_HANDLE); return 0; }
    switch (idx) {
        case GWLP_WNDPROC: return reinterpret_cast<LONG_PTR>(w->proc);
        case GWLP_HINSTANCE: return reinterpret_cast<LONG_PTR>(w->inst);
        case GWLP_HWNDPARENT: return reinterpret_cast<LONG_PTR>(w->parent);
        case GWLP_ID: return w->id;
        case GWLP_USERDATA: return w->userData;
        case GWL_STYLE: return static_cast<LONG>(w->style);
        case GWL_EXSTYLE: return static_cast<LONG>(w->exStyle);
        default:
            if (w->dialog && idx == DWLP_MSGRESULT) return w->msgResult;
            if (w->dialog && idx == DWLP_DLGPROC) return reinterpret_cast<LONG_PTR>(w->dlgProc);
            if (w->dialog && idx == DWLP_USER) return w->dlgUser;
            if (idx >= 0 && static_cast<size_t>(idx) + sizeof(LONG) <= w->extra.size()) {
                LONG v;
                std::memcpy(&v, &w->extra[static_cast<size_t>(idx)], sizeof v);
                return v;
            }
            return 0;
    }
}
static LONG_PTR setLong(HWND h, int idx, LONG_PTR v) {
    WndPtr w = wnd(h);
    if (!w) { setError(ERROR_INVALID_WINDOW_HANDLE); return 0; }
    const LONG_PTR old = getLong(h, idx);
    switch (idx) {
        case GWLP_WNDPROC: w->proc = reinterpret_cast<WNDPROC>(v); break;
        case GWLP_HINSTANCE: w->inst = reinterpret_cast<HINSTANCE>(v); break;
        case GWLP_HWNDPARENT: w->parent = reinterpret_cast<HWND>(v); break;
        case GWLP_ID: w->id = v; break;
        case GWLP_USERDATA: w->userData = v; break;
        case GWL_STYLE:
            w->style = static_cast<DWORD>(v);
            if (w->sdl) {
                SDL_SetWindowBordered(w->sdl, (w->style & WS_CAPTION) ? SDL_TRUE : SDL_FALSE);
                SDL_SetWindowResizable(w->sdl, (w->style & WS_THICKFRAME) ? SDL_TRUE : SDL_FALSE);
            }
            break;
        case GWL_EXSTYLE: w->exStyle = static_cast<DWORD>(v); break;
        default:
            if (w->dialog && idx == DWLP_MSGRESULT) w->msgResult = v;
            else if (w->dialog && idx == DWLP_DLGPROC) w->dlgProc = reinterpret_cast<DLGPROC>(v);
            else if (w->dialog && idx == DWLP_USER) w->dlgUser = v;
            else if (idx >= 0 && static_cast<size_t>(idx) + sizeof(LONG) <= w->extra.size()) {
                const LONG x = static_cast<LONG>(v);
                std::memcpy(&w->extra[static_cast<size_t>(idx)], &x, sizeof x);
            }
            break;
    }
    return old;
}
LONG WINAPI GetWindowLongA(HWND h, int i) { return static_cast<LONG>(getLong(h, i)); }
LONG WINAPI GetWindowLongW(HWND h, int i) { return static_cast<LONG>(getLong(h, i)); }
LONG WINAPI SetWindowLongA(HWND h, int i, LONG v) { return static_cast<LONG>(setLong(h, i, v)); }
LONG WINAPI SetWindowLongW(HWND h, int i, LONG v) { return static_cast<LONG>(setLong(h, i, v)); }
LONG_PTR WINAPI GetWindowLongPtrA(HWND h, int i) { return getLong(h, i); }
LONG_PTR WINAPI GetWindowLongPtrW(HWND h, int i) { return getLong(h, i); }
LONG_PTR WINAPI SetWindowLongPtrA(HWND h, int i, LONG_PTR v) { return setLong(h, i, v); }
LONG_PTR WINAPI SetWindowLongPtrW(HWND h, int i, LONG_PTR v) { return setLong(h, i, v); }

BOOL WINAPI SetPropA(HWND h, LPCSTR name, HANDLE v) {
    WndPtr w = wnd(h);
    if (!w) return FALSE;
    w->props[reinterpret_cast<uintptr_t>(name) < 0x10000 ? "#" + std::to_string(reinterpret_cast<uintptr_t>(name)) : lc(name)] = v;
    return TRUE;
}
HANDLE WINAPI GetPropA(HWND h, LPCSTR name) {
    WndPtr w = wnd(h);
    if (!w) return nullptr;
    auto it = w->props.find(reinterpret_cast<uintptr_t>(name) < 0x10000 ? "#" + std::to_string(reinterpret_cast<uintptr_t>(name)) : lc(name));
    return it == w->props.end() ? nullptr : it->second;
}
HANDLE WINAPI RemovePropA(HWND h, LPCSTR name) {
    WndPtr w = wnd(h);
    if (!w) return nullptr;
    const std::string k = reinterpret_cast<uintptr_t>(name) < 0x10000 ? "#" + std::to_string(reinterpret_cast<uintptr_t>(name)) : lc(name);
    auto it = w->props.find(k);
    if (it == w->props.end()) return nullptr;
    HANDLE v = it->second;
    w->props.erase(it);
    return v;
}
BOOL WINAPI SetWindowTextA(HWND h, LPCSTR t) {
    WndPtr w = wnd(h);
    if (!w) return FALSE;
    w->title = strA(t);
    if (w->sdl) SDL_SetWindowTitle(w->sdl, w->title.c_str());
    return TRUE;
}
BOOL WINAPI SetWindowTextW(HWND h, LPCWSTR t) {
    WndPtr w = wnd(h);
    if (!w) return FALSE;
    w->title = strW(t);
    if (w->sdl) SDL_SetWindowTitle(w->sdl, w->title.c_str());
    return TRUE;
}
int WINAPI GetWindowTextA(HWND h, LPSTR buf, int n) {
    WndPtr w = wnd(h);
    if (!w || n <= 0) return 0;
    const std::string a = utf8ToAcp(w->title.c_str());
    std::strncpy(buf, a.c_str(), static_cast<size_t>(n) - 1);
    buf[n - 1] = 0;
    return static_cast<int>(std::strlen(buf));
}

// ---- focus, capture, cursor -----------------------------------------------------------------
HWND WINAPI GetFocus(void) { return g_focus; }
HWND WINAPI SetFocus(HWND h) { HWND old = g_focus; g_focus = h; return old; }
HWND WINAPI GetForegroundWindow(void) { return g_active; }
HWND WINAPI GetActiveWindow(void) { return g_active; }
BOOL WINAPI SetForegroundWindow(HWND h) {
    if (WndPtr w = wnd(h); w && w->sdl) SDL_RaiseWindow(w->sdl);
    return TRUE;
}
HWND WINAPI SetCapture(HWND h) {
    HWND old = g_capture;
    g_capture = h;
    if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_CaptureMouse(SDL_TRUE);
    return old;
}
BOOL WINAPI ReleaseCapture(void) {
    g_capture = nullptr;
    if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_CaptureMouse(SDL_FALSE);
    return TRUE;
}
HWND WINAPI GetCapture(void) { return g_capture; }
int WINAPI ShowCursor(BOOL show) {
    g_cursorCount += show ? 1 : -1;
    if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_ShowCursor(g_cursorCount >= 0 ? SDL_ENABLE : SDL_DISABLE);
    return g_cursorCount;
}
HCURSOR WINAPI SetCursor(HCURSOR c) { return c; }
BOOL WINAPI GetCursorPos(LPPOINT p) {
    int x = 0, y = 0;
    if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_GetGlobalMouseState(&x, &y);
    p->x = x, p->y = y;
    return TRUE;
}
BOOL WINAPI SetCursorPos(int x, int y) {
    if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_WarpMouseGlobal(x, y);
    return TRUE;
}
SHORT WINAPI GetKeyState(int vk) {
    const uint8_t s = g_keys[vk & 0xFF];
    return static_cast<SHORT>(((s & 0x80) ? 0x8000 : 0) | (s & 1));
}
SHORT WINAPI GetAsyncKeyState(int vk) {
    pump();
    return static_cast<SHORT>((g_keys[vk & 0xFF] & 0x80) ? 0x8000 : 0);
}
HKL WINAPI GetKeyboardLayout(DWORD) { return reinterpret_cast<HKL>(static_cast<uintptr_t>(0x04090409)); }
UINT WINAPI GetCaretBlinkTime(void) { return 530; }
DWORD WINAPI GetSysColor(int) { return 0x00C0C0C0; }
int WINAPI GetSystemMetrics(int i) {
    SDL_Rect b{0, 0, 1920, 1080};
    if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_GetDisplayBounds(0, &b);
    switch (i) {
        case SM_CXSCREEN: case SM_CXVIRTUALSCREEN: case SM_CXFULLSCREEN: return b.w;
        case SM_CYSCREEN: case SM_CYVIRTUALSCREEN: case SM_CYFULLSCREEN: return b.h;
        case SM_CMONITORS: return 1;
        case SM_CXCURSOR: case SM_CYCURSOR: return 32;
        case SM_CXICON: case SM_CYICON: return 32;
        case SM_CXSMICON: case SM_CYSMICON: return 16;
        case SM_CYCAPTION: return 23;
        case SM_CXFRAME: case SM_CYFRAME: return 4;
        case SM_CXDOUBLECLK: case SM_CYDOUBLECLK: return 4;
        case SM_SWAPBUTTON: case SM_REMOTESESSION: return 0;
        case SM_MOUSEPRESENT: return 1;
        default: return 0;
    }
}
HMONITOR WINAPI MonitorFromWindow(HWND, DWORD) { return reinterpret_cast<HMONITOR>(static_cast<uintptr_t>(0x10020)); }
HMONITOR WINAPI MonitorFromPoint(POINT, DWORD) { return reinterpret_cast<HMONITOR>(static_cast<uintptr_t>(0x10020)); }
BOOL WINAPI GetMonitorInfoW(HMONITOR, LPMONITORINFO mi) {
    SDL_Rect b{0, 0, 1920, 1080}, u = b;
    if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_GetDisplayBounds(0, &b), SDL_GetDisplayUsableBounds(0, &u);
    mi->rcMonitor = {b.x, b.y, b.x + b.w, b.y + b.h};
    mi->rcWork = {u.x, u.y, u.x + u.w, u.y + u.h};
    mi->dwFlags = MONITORINFOF_PRIMARY;
    return TRUE;
}
BOOL WINAPI GetMonitorInfoA(HMONITOR m, LPMONITORINFO mi) { return GetMonitorInfoW(m, mi); }

// ---- messages -----------------------------------------------------------------------------
BOOL WINAPI PeekMessageW(LPMSG m, HWND h, UINT lo, UINT hi, UINT rm) { return peek(m, h, lo, hi, rm); }
BOOL WINAPI PeekMessageA(LPMSG m, HWND h, UINT lo, UINT hi, UINT rm) { return peek(m, h, lo, hi, rm); }
BOOL WINAPI GetMessageA(LPMSG m, HWND h, UINT lo, UINT hi) {
    while (!peek(m, h, lo, hi, PM_REMOVE)) waitForMessage(INFINITE);
    return m->message != WM_QUIT;
}
BOOL WINAPI GetMessageW(LPMSG m, HWND h, UINT lo, UINT hi) { return GetMessageA(m, h, lo, hi); }
BOOL WINAPI TranslateMessage(const MSG*) { return FALSE; }  // WM_CHAR comes from SDL text input
LRESULT WINAPI DispatchMessageW(const MSG* m) {
    if (!m->hwnd) return 0;
    return send(m->hwnd, m->message, m->wParam, m->lParam);
}
LRESULT WINAPI DispatchMessageA(const MSG* m) { return DispatchMessageW(m); }
LRESULT WINAPI SendMessageW(HWND h, UINT msg, WPARAM wp, LPARAM lp) { return send(h, msg, wp, lp); }
LRESULT WINAPI SendMessageA(HWND h, UINT msg, WPARAM wp, LPARAM lp) { return send(h, msg, wp, lp); }
BOOL WINAPI PostMessageW(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    if (h && !wnd(h)) return fail(ERROR_INVALID_WINDOW_HANDLE);
    post(h, msg, wp, lp);
    return TRUE;
}
BOOL WINAPI PostMessageA(HWND h, UINT msg, WPARAM wp, LPARAM lp) { return PostMessageW(h, msg, wp, lp); }
BOOL WINAPI PostThreadMessageW(DWORD tid, UINT msg, WPARAM wp, LPARAM lp) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    MSG m{};
    m.message = msg;
    m.wParam = wp;
    m.lParam = lp;
    m.time = GetTickCount();
    g_queues[tid].msgs.push_back(m);
    g_queueCv.notify_all();
    return TRUE;
}
void WINAPI PostQuitMessage(int code) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    Queue& q = g_queues[currentTid()];
    q.quit = true;
    q.quitCode = code;
}
BOOL WINAPI WaitMessage(void) {
    MSG m;
    while (!peek(&m, nullptr, 0, 0, PM_NOREMOVE)) waitForMessage(INFINITE);
    return TRUE;
}
DWORD WINAPI GetQueueStatus(UINT) {
    pump();
    std::lock_guard<std::recursive_mutex> l(g_lock);
    return g_queues[currentTid()].msgs.empty() ? 0 : static_cast<DWORD>(QS_ALLINPUT | (QS_ALLINPUT << 16));
}
DWORD WINAPI MsgWaitForMultipleObjects(DWORD n, const HANDLE* hs, BOOL all, DWORD ms, DWORD mask) {
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms == INFINITE ? 0 : ms);
    for (;;) {
        if (n) {
            const DWORD r = WaitForMultipleObjects(n, hs, all, 0);
            if (r != WAIT_TIMEOUT) return r;
        }
        if (mask) {
            MSG m;
            if (peek(&m, nullptr, 0, 0, PM_NOREMOVE)) return WAIT_OBJECT_0 + n;
        }
        if (ms != INFINITE && std::chrono::steady_clock::now() >= deadline) return WAIT_TIMEOUT;
        waitForMessage(ms == 0 ? 0 : 5);
    }
}
UINT WINAPI RegisterWindowMessageW(LPCWSTR name) {
    std::lock_guard<std::recursive_mutex> l(g_lock);
    const std::string k = lc(strW(name));
    auto it = g_messages.find(k);
    if (it != g_messages.end()) return it->second;
    return g_messages[k] = g_nextMessage++;
}
LRESULT WINAPI CallWindowProcA(WNDPROC p, HWND h, UINT m, WPARAM w, LPARAM l) { return p ? p(h, m, w, l) : 0; }
LRESULT WINAPI CallWindowProcW(WNDPROC p, HWND h, UINT m, WPARAM w, LPARAM l) { return p ? p(h, m, w, l) : 0; }

LRESULT WINAPI DefWindowProcW(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_NCCREATE: return TRUE;
        case WM_CLOSE: DestroyWindow(h); return 0;
        case WM_SYSCOMMAND:
            if ((wp & 0xFFF0) == SC_CLOSE) post(h, WM_CLOSE, 0, 0);
            return 0;
        case WM_ERASEBKGND: return 1;
        case WM_SETCURSOR: return FALSE;
        case WM_MOUSEACTIVATE: return MA_ACTIVATE;
        case WM_NCACTIVATE: return TRUE;
        case WM_GETMINMAXINFO: return 0;
        default: return 0;
    }
}
LRESULT WINAPI DefWindowProcA(HWND h, UINT msg, WPARAM wp, LPARAM lp) { return DefWindowProcW(h, msg, wp, lp); }

// ---- dialogs: run the dialog procedure headless and accept it ------------------------------
static INT_PTR runDialog(DLGPROC proc, HWND parent, LPARAM init) {
    auto w = std::make_shared<Wnd>();
    w->dialog = true;
    w->dlgProc = proc;
    w->parent = parent;
    w->thread = currentTid();
    {
        std::lock_guard<std::recursive_mutex> l(g_lock);
        w->h = reinterpret_cast<HWND>(g_nextHwnd);
        g_nextHwnd += 4;
        g_windows[w->h] = w;
    }
    // Messages to the dialog go to its DLGPROC.
    w->proc = nullptr;
    proc(w->h, WM_INITDIALOG, 0, init);
    if (!w->ended) proc(w->h, WM_COMMAND, MAKEWPARAM(IDOK, BN_CLICKED), 0);
    if (!w->ended) w->result = IDOK;
    proc(w->h, WM_DESTROY, 0, 0);
    proc(w->h, WM_NCDESTROY, 0, 0);
    std::lock_guard<std::recursive_mutex> l(g_lock);
    g_windows.erase(w->h);
    std::fprintf(stderr, "dialog auto-accepted (result %ld)\n", static_cast<long>(w->result));
    return w->result;
}
INT_PTR WINAPI DialogBoxParamA(HINSTANCE, LPCSTR, HWND parent, DLGPROC proc, LPARAM init) { return runDialog(proc, parent, init); }
INT_PTR WINAPI DialogBoxIndirectParamA(HINSTANCE, LPCDLGTEMPLATEA, HWND parent, DLGPROC proc, LPARAM init) { return runDialog(proc, parent, init); }
BOOL WINAPI EndDialog(HWND h, INT_PTR r) {
    WndPtr w = wnd(h);
    if (!w) return FALSE;
    w->ended = true;
    w->result = r;
    return TRUE;
}
HWND WINAPI GetDlgItem(HWND h, int id) { return reinterpret_cast<HWND>(static_cast<uintptr_t>(0x30000 + (id & 0xFFFF))); }
BOOL WINAPI SetDlgItemTextA(HWND, int, LPCSTR) { return TRUE; }
UINT WINAPI IsDlgButtonChecked(HWND, int) { return BST_UNCHECKED; }

int WINAPI MessageBoxW(HWND, LPCWSTR text, LPCWSTR caption, UINT type) {
    const std::string t = strW(text), c = strW(caption);
    std::fprintf(stderr, "MessageBox: %s: %s\n", c.c_str(), t.c_str());
    if (!getenv("FABLE_RECOMP_HEADLESS")) SDL_ShowSimpleMessageBox((type & MB_ICONERROR) ? SDL_MESSAGEBOX_ERROR : SDL_MESSAGEBOX_INFORMATION, c.c_str(), t.c_str(), nullptr);
    switch (type & 0xF) {
        case MB_YESNO: case MB_YESNOCANCEL: return IDYES;
        default: return IDOK;
    }
}
int WINAPI MessageBoxA(HWND h, LPCSTR text, LPCSTR caption, UINT type) {
    const wstr t = fromUtf8(strA(text).c_str()), c = fromUtf8(strA(caption).c_str());
    return MessageBoxW(h, t.c_str(), c.c_str(), type);
}

// ---- resources ------------------------------------------------------------------------------
int w32_loadString(HMODULE h, UINT id, wstr& out);
int WINAPI LoadStringA(HINSTANCE h, UINT id, LPSTR buf, int n) {
    wstr s;
    if (!w32_loadString(h, id, s) || n <= 0) { if (n > 0) buf[0] = 0; return 0; }
    const std::string a = utf8ToAcp(toUtf8(s.c_str(), s.size()).c_str());
    std::strncpy(buf, a.c_str(), static_cast<size_t>(n) - 1);
    buf[n - 1] = 0;
    return static_cast<int>(std::strlen(buf));
}
int WINAPI LoadStringW(HINSTANCE h, UINT id, LPWSTR buf, int n) {
    wstr s;
    if (!w32_loadString(h, id, s) || n <= 0) { if (n > 0) buf[0] = 0; return 0; }
    const size_t len = std::min<size_t>(s.size(), static_cast<size_t>(n) - 1);
    for (size_t i = 0; i < len; ++i) buf[i] = s[i];
    buf[len] = 0;
    return static_cast<int>(len);
}
HANDLE WINAPI LoadImageA(HINSTANCE, LPCSTR, UINT, int, int, UINT) { return reinterpret_cast<HANDLE>(static_cast<uintptr_t>(0x10030)); }
HANDLE WINAPI LoadImageW(HINSTANCE, LPCWSTR, UINT, int, int, UINT) { return reinterpret_cast<HANDLE>(static_cast<uintptr_t>(0x10030)); }

// ---- misc ---------------------------------------------------------------------------------
LPSTR WINAPI CharNextExA(WORD, LPCSTR p, DWORD) { return const_cast<LPSTR>(*p ? p + 1 : p); }
BOOL WINAPI PtInRect(const RECT* r, POINT p) { return p.x >= r->left && p.x < r->right && p.y >= r->top && p.y < r->bottom; }
BOOL WINAPI OffsetRect(LPRECT r, int dx, int dy) {
    r->left += dx, r->right += dx, r->top += dy, r->bottom += dy;
    return TRUE;
}
BOOL WINAPI SetRect(LPRECT r, int a, int b, int c, int d) { *r = {a, b, c, d}; return TRUE; }

// ---- GDI (fonts are the game's own; these back the IME and dialogs only) -------------------
HFONT WINAPI CreateFontIndirectA(const LOGFONTA*) { return reinterpret_cast<HFONT>(static_cast<uintptr_t>(0x10040)); }
BOOL WINAPI DeleteObject(HGDIOBJ) { return TRUE; }
HGDIOBJ WINAPI GetStockObject(int) { return reinterpret_cast<HGDIOBJ>(static_cast<uintptr_t>(0x10044)); }
COLORREF WINAPI SetTextColor(HDC, COLORREF c) { return c; }
int WINAPI GetObjectA(HANDLE, int n, LPVOID out) {
    if (out && n > 0) std::memset(out, 0, static_cast<size_t>(n));
    return n;
}
DWORD WINAPI GetObjectType(HGDIOBJ) { return OBJ_FONT; }

// ---- IMM32: no input method editor --------------------------------------------------------
HIMC WINAPI ImmGetContext(HWND) { return nullptr; }
BOOL WINAPI ImmReleaseContext(HWND, HIMC) { return TRUE; }
HIMC WINAPI ImmAssociateContext(HWND, HIMC) { return nullptr; }
DWORD WINAPI ImmGetCandidateListA(HIMC, DWORD, LPCANDIDATELIST, DWORD) { return 0; }
DWORD WINAPI ImmGetCandidateListW(HIMC, DWORD, LPCANDIDATELIST, DWORD) { return 0; }
LONG WINAPI ImmGetCompositionStringA(HIMC, DWORD, LPVOID, DWORD) { return 0; }
LONG WINAPI ImmGetCompositionStringW(HIMC, DWORD, LPVOID, DWORD) { return 0; }
BOOL WINAPI ImmGetConversionStatus(HIMC, LPDWORD a, LPDWORD b) { if (a) *a = 0; if (b) *b = 0; return TRUE; }
BOOL WINAPI ImmSetConversionStatus(HIMC, DWORD, DWORD) { return TRUE; }
HWND WINAPI ImmGetDefaultIMEWnd(HWND) { return nullptr; }
UINT WINAPI ImmGetIMEFileNameA(HKL, LPSTR buf, UINT n) { if (buf && n) buf[0] = 0; return 0; }
BOOL WINAPI ImmGetOpenStatus(HIMC) { return FALSE; }
BOOL WINAPI ImmSetOpenStatus(HIMC, BOOL) { return TRUE; }
UINT WINAPI ImmGetVirtualKey(HWND) { return VK_PROCESSKEY; }
BOOL WINAPI ImmIsIME(HKL) { return FALSE; }
BOOL WINAPI ImmNotifyIME(HIMC, DWORD, DWORD, DWORD) { return FALSE; }
BOOL WINAPI ImmSimulateHotKey(HWND, DWORD) { return FALSE; }

}  // extern "C"
