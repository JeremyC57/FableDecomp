// Handles, last error, UTF-16 / code-page conversion, alertable APCs.
#include "w32.hpp"

#include <cerrno>
#include <cstring>
#include <deque>
#include <map>
#include <unistd.h>
#include <sys/syscall.h>

namespace w32 {

// ---- last error ---------------------------------------------------------------------------
static thread_local DWORD t_lastError;
void setError(DWORD e) { t_lastError = e; }

DWORD errnoToWin(int e) {
    switch (e) {
        case 0: return ERROR_SUCCESS;
        case ENOENT: return ERROR_FILE_NOT_FOUND;
        case ENOTDIR: return ERROR_PATH_NOT_FOUND;
        case EACCES: case EPERM: case EROFS: return ERROR_ACCESS_DENIED;
        case EEXIST: return ERROR_ALREADY_EXISTS;
        case ENOTEMPTY: return ERROR_DIR_NOT_EMPTY;
        case ENOSPC: return ERROR_DISK_FULL;
        case ENOMEM: return ERROR_NOT_ENOUGH_MEMORY;
        case EBADF: return ERROR_INVALID_HANDLE;
        case EINVAL: return ERROR_INVALID_PARAMETER;
        case EBUSY: return ERROR_BUSY;
        case EMFILE: return ERROR_TOO_MANY_OPEN_FILES;
        default: return ERROR_GEN_FAILURE;
    }
}

// ---- UTF-16 / code pages ----------------------------------------------------------------
size_t wlen(const wchar_t* s) {
    size_t n = 0;
    if (s) while (s[n]) ++n;
    return n;
}

static void putUtf8(std::string& o, uint32_t cp) {
    if (cp < 0x80) o += static_cast<char>(cp);
    else if (cp < 0x800) { o += static_cast<char>(0xC0 | (cp >> 6)); o += static_cast<char>(0x80 | (cp & 0x3F)); }
    else if (cp < 0x10000) {
        o += static_cast<char>(0xE0 | (cp >> 12)); o += static_cast<char>(0x80 | ((cp >> 6) & 0x3F)); o += static_cast<char>(0x80 | (cp & 0x3F));
    } else {
        o += static_cast<char>(0xF0 | (cp >> 18)); o += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        o += static_cast<char>(0x80 | ((cp >> 6) & 0x3F)); o += static_cast<char>(0x80 | (cp & 0x3F));
    }
}

std::string toUtf8(const wchar_t* s, size_t n) {
    std::string o;
    if (!s) return o;
    if (n == size_t(-1)) n = wlen(s);
    for (size_t i = 0; i < n; ++i) {
        uint32_t c = static_cast<uint16_t>(s[i]);
        if (c >= 0xD800 && c < 0xDC00 && i + 1 < n && static_cast<uint16_t>(s[i + 1]) >= 0xDC00 && static_cast<uint16_t>(s[i + 1]) < 0xE000)
            c = 0x10000 + ((c - 0xD800) << 10) + (static_cast<uint16_t>(s[++i]) - 0xDC00);
        putUtf8(o, c);
    }
    return o;
}

wstr fromUtf8(const char* s, size_t n) {
    wstr o;
    if (!s) return o;
    if (n == size_t(-1)) n = std::strlen(s);
    for (size_t i = 0; i < n;) {
        const auto b = static_cast<unsigned char>(s[i]);
        uint32_t cp;
        int len;
        if (b < 0x80) cp = b, len = 1;
        else if ((b & 0xE0) == 0xC0) cp = b & 0x1F, len = 2;
        else if ((b & 0xF0) == 0xE0) cp = b & 0x0F, len = 3;
        else if ((b & 0xF8) == 0xF0) cp = b & 0x07, len = 4;
        else { o += static_cast<wchar_t>(0xFFFD); ++i; continue; }
        if (i + len > n) { o += static_cast<wchar_t>(0xFFFD); break; }
        for (int k = 1; k < len; ++k) cp = (cp << 6) | (static_cast<unsigned char>(s[i + k]) & 0x3F);
        i += len;
        if (cp >= 0x10000) {
            cp -= 0x10000;
            o += static_cast<wchar_t>(0xD800 + (cp >> 10));
            o += static_cast<wchar_t>(0xDC00 + (cp & 0x3FF));
        } else {
            o += static_cast<wchar_t>(cp);
        }
    }
    return o;
}

// Windows-1252 bytes 0x80..0x9F (the rest equals Latin-1).
static const uint16_t kCp1252[32] = {
    0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021, 0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0x008F,
    0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009D, 0x017E, 0x0178};
static uint16_t acpToWide(unsigned char b) { return b >= 0x80 && b < 0xA0 ? kCp1252[b - 0x80] : b; }
static int wideToAcp(uint16_t w) {
    if (w < 0x80 || (w >= 0xA0 && w < 0x100)) return w;
    for (int i = 0; i < 32; ++i)
        if (kCp1252[i] == w) return 0x80 + i;
    return -1;
}

std::string acpToUtf8(const char* s, size_t n) {
    std::string o;
    if (!s) return o;
    if (n == size_t(-1)) n = std::strlen(s);
    for (size_t i = 0; i < n; ++i) putUtf8(o, acpToWide(static_cast<unsigned char>(s[i])));
    return o;
}
std::string utf8ToAcp(const char* s) {
    const wstr w = fromUtf8(s);
    std::string o;
    for (wchar_t c : w) {
        const int b = wideToAcp(static_cast<uint16_t>(c));
        o += static_cast<char>(b < 0 ? '?' : b);
    }
    return o;
}

wchar_t upper(wchar_t c) {
    const auto u = static_cast<uint16_t>(c);
    if (u >= 'a' && u <= 'z') return static_cast<wchar_t>(u - 32);
    if ((u >= 0xE0 && u <= 0xFE && u != 0xF7)) return static_cast<wchar_t>(u - 32);
    if (u == 0xFF) return static_cast<wchar_t>(0x178);
    if (u >= 0x100 && u < 0x180 && (u & 1)) return static_cast<wchar_t>(u - 1);  // Latin Extended-A pairs
    if (u >= 0x3B1 && u <= 0x3C9 && u != 0x3C2) return static_cast<wchar_t>(u - 32);  // Greek
    if (u >= 0x430 && u <= 0x44F) return static_cast<wchar_t>(u - 32);  // Cyrillic
    if (u >= 0x450 && u <= 0x45F) return static_cast<wchar_t>(u - 80);
    return c;
}
wchar_t lower(wchar_t c) {
    const auto u = static_cast<uint16_t>(c);
    if (u >= 'A' && u <= 'Z') return static_cast<wchar_t>(u + 32);
    if ((u >= 0xC0 && u <= 0xDE && u != 0xD7)) return static_cast<wchar_t>(u + 32);
    if (u == 0x178) return static_cast<wchar_t>(0xFF);
    if (u >= 0x100 && u < 0x180 && !(u & 1)) return static_cast<wchar_t>(u + 1);
    if (u >= 0x391 && u <= 0x3A9) return static_cast<wchar_t>(u + 32);
    if (u >= 0x410 && u <= 0x42F) return static_cast<wchar_t>(u + 32);
    if (u >= 0x400 && u <= 0x40F) return static_cast<wchar_t>(u + 80);
    return c;
}

// ---- handles ------------------------------------------------------------------------------
namespace {
std::mutex g_handleLock;
std::vector<std::shared_ptr<Object>> g_handles;
std::vector<uint32_t> g_freeSlots;
constexpr uintptr_t kHandleBase = 0x1000;  // well above pseudo handles, small enough for the guest
}  // namespace

HANDLE newHandle(std::shared_ptr<Object> o) {
    std::lock_guard<std::mutex> l(g_handleLock);
    uint32_t i;
    if (!g_freeSlots.empty()) { i = g_freeSlots.back(); g_freeSlots.pop_back(); g_handles[i] = std::move(o); }
    else { i = static_cast<uint32_t>(g_handles.size()); g_handles.push_back(std::move(o)); }
    return reinterpret_cast<HANDLE>(kHandleBase + 4 * static_cast<uintptr_t>(i));
}

std::shared_ptr<Object> object(HANDLE h) {
    const auto v = reinterpret_cast<uintptr_t>(h);
    if (v < kHandleBase || (v & 3)) return nullptr;
    const size_t i = (v - kHandleBase) / 4;
    std::lock_guard<std::mutex> l(g_handleLock);
    return i < g_handles.size() ? g_handles[i] : nullptr;
}

bool closeHandle(HANDLE h) {
    const auto v = reinterpret_cast<uintptr_t>(h);
    if (v < kHandleBase || (v & 3)) return false;
    const size_t i = (v - kHandleBase) / 4;
    std::shared_ptr<Object> dead;  // destroyed outside the lock
    std::lock_guard<std::mutex> l(g_handleLock);
    if (i >= g_handles.size() || !g_handles[i]) return false;
    dead = std::move(g_handles[i]);
    g_freeSlots.push_back(static_cast<uint32_t>(i));
    return true;
}

static std::mutex g_waitLock;
static std::condition_variable_any g_waitCv;
std::mutex& waitLock() { return g_waitLock; }
void notifyAll() { g_waitCv.notify_all(); }
std::condition_variable_any& waitCv() { return g_waitCv; }

// ---- APCs ---------------------------------------------------------------------------------
namespace {
std::mutex g_apcLock;
std::map<DWORD, std::deque<std::function<void()>>> g_apcs;
}  // namespace

void queueApc(DWORD tid, std::function<void()> fn) {
    {
        std::lock_guard<std::mutex> l(g_apcLock);
        g_apcs[tid].push_back(std::move(fn));
    }
    std::lock_guard<std::mutex> l(g_waitLock);
    g_waitCv.notify_all();
}

bool hasApcs(DWORD tid) {
    std::lock_guard<std::mutex> l(g_apcLock);
    auto it = g_apcs.find(tid);
    return it != g_apcs.end() && !it->second.empty();
}

bool runApcs() {
    bool any = false;
    for (;;) {
        std::function<void()> fn;
        {
            std::lock_guard<std::mutex> l(g_apcLock);
            auto it = g_apcs.find(currentTid());
            if (it == g_apcs.end() || it->second.empty()) return any;
            fn = std::move(it->second.front());
            it->second.pop_front();
        }
        fn();
        any = true;
    }
}

}  // namespace w32

using namespace w32;

extern "C" {
DWORD WINAPI GetLastError(void) { return t_lastError; }
void WINAPI SetLastError(DWORD e) { t_lastError = e; }
}
