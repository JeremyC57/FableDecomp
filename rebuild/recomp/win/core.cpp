// Core host services: logging, low guest memory + heap, traps, imports,
// guest threads and host->guest calls.
#include "host.hpp"

#include <atomic>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace host {

// ============================================================================
// logging
// ============================================================================
int g_logLevel = 1;
static FILE* g_log;
static SRWLOCK g_logLock = SRWLOCK_INIT;

void logInit(const std::wstring& path) {
    g_log = _wfopen(path.c_str(), L"w");
    if (const char* l = std::getenv("FABLE_RECOMP_LOG")) g_logLevel = std::atoi(l);
}

static void vlog(const char* fmt, va_list ap) {
    char buf[2048];
    std::vsnprintf(buf, sizeof buf, fmt, ap);
    AcquireSRWLockExclusive(&g_logLock);
    std::fprintf(stderr, "[%5lu] %s\n", GetCurrentThreadId(), buf);
    if (g_log) {
        std::fprintf(g_log, "[%5lu] %s\n", GetCurrentThreadId(), buf);
        std::fflush(g_log);
    }
    ReleaseSRWLockExclusive(&g_logLock);
}

void log(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vlog(fmt, ap);
    va_end(ap);
}

void die(const char* fmt, ...) {
    char buf[2048];
    va_list ap;
    va_start(ap, fmt);
    std::vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    log("FATAL: %s", buf);
    if (!std::getenv("FABLE_RECOMP_HEADLESS")) MessageBoxA(nullptr, buf, "Fable (recompiled) - fatal error", MB_ICONERROR | MB_OK);
    ExitProcess(1);
}

std::string narrow(const wchar_t* w) {
    if (!w) return {};
    const int n = WideCharToMultiByte(CP_ACP, 0, w, -1, nullptr, 0, nullptr, nullptr);
    std::string s(n > 0 ? n - 1 : 0, '\0');
    if (n > 1) WideCharToMultiByte(CP_ACP, 0, w, -1, s.data(), n, nullptr, nullptr);
    return s;
}
std::wstring widen(const char* s) {
    if (!s) return {};
    const int n = MultiByteToWideChar(CP_ACP, 0, s, -1, nullptr, 0);
    std::wstring w(n > 0 ? n - 1 : 0, L'\0');
    if (n > 1) MultiByteToWideChar(CP_ACP, 0, s, -1, w.data(), n);
    return w;
}

// ============================================================================
// low memory
// ============================================================================
uint32_t ga(const void* p) {
    if (!p) return 0;
    if (!isGuest(p)) die("host pointer %p is not guest-visible", p);
    return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(p));
}

static SRWLOCK g_vmLock = SRWLOCK_INIT;

uint32_t findFreeLow(uint32_t size, uint32_t align) {
    uintptr_t a = 0x00100000;
    while (a + size <= kGuestLimit) {
        MEMORY_BASIC_INFORMATION mbi;
        if (!VirtualQuery(reinterpret_cast<void*>(a), &mbi, sizeof mbi)) break;
        const uintptr_t regEnd = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
        if (mbi.State == MEM_FREE) {
            const uintptr_t start = (a + align - 1) & ~uintptr_t(align - 1);
            if (start + size <= regEnd && start + size <= kGuestLimit) return static_cast<uint32_t>(start);
        }
        a = regEnd;
    }
    return 0;
}

uint32_t lowVirtualAlloc(uint32_t addr, uint32_t size, DWORD type, DWORD protect) {
    if (addr) {
        void* p = VirtualAlloc(gp(addr), size, type, protect);
        return p && isGuest(p) ? ga(p) : 0;
    }
    AcquireSRWLockExclusive(&g_vmLock);
    uint32_t result = 0;
    for (int tries = 0; tries < 16 && !result; ++tries) {
        const uint32_t at = findFreeLow((size + 0xFFFF) & ~0xFFFFu);
        if (!at) break;
        if (VirtualAlloc(gp(at), size, type | MEM_RESERVE, protect)) result = at;
    }
    ReleaseSRWLockExclusive(&g_vmLock);
    return result;
}

// ---------------------------------------------------------------------------
// Guest heap: boundary-tag allocator over 64 MiB low segments, 8-byte aligned
// payloads (what the retail CRT heap guarantees), big blocks straight from VM.
// Chunk header (8 bytes, before the payload): prevSize, size|flags.
// ---------------------------------------------------------------------------
namespace heap {
constexpr uint32_t INUSE = 1, PREV_INUSE = 2, BIG = 4, FLAGS = 7;
constexpr uint32_t kSeg = 64u << 20, kBig = 1u << 20, kMin = 16;
constexpr int kSmallBins = 64, kLargeBins = 40;

SRWLOCK lock = SRWLOCK_INIT;
uint32_t bins[kSmallBins + kLargeBins];  // head chunk addresses (header address)
uint64_t binMap[2];
uint32_t top = 0, topEnd = 0;  // wilderness chunk (header) and end of its segment fence
uint64_t bytesInUse = 0;

inline uint32_t& prevSize(uint32_t ch) { return *gp<uint32_t>(ch); }
inline uint32_t& head(uint32_t ch) { return *gp<uint32_t>(ch + 4); }
inline uint32_t size(uint32_t ch) { return head(ch) & ~FLAGS; }
inline uint32_t& fwd(uint32_t ch) { return *gp<uint32_t>(ch + 8); }
inline uint32_t& bck(uint32_t ch) { return *gp<uint32_t>(ch + 12); }

inline int binOf(uint32_t sz) {
    if (sz < kSmallBins * 8) return static_cast<int>(sz >> 3);
    int b = 31 - __builtin_clz(sz);  // >= 9
    return kSmallBins + (b - 9) * 2 + ((sz >> (b - 1)) & 1);
}
inline void mark(int b) { binMap[b >> 6] |= 1ull << (b & 63); }
inline void unmark(int b) { binMap[b >> 6] &= ~(1ull << (b & 63)); }

void link(uint32_t ch) {
    const int b = binOf(size(ch));
    fwd(ch) = bins[b];
    bck(ch) = 0;
    if (bins[b]) bck(bins[b]) = ch;
    bins[b] = ch;
    mark(b);
}
void unlink(uint32_t ch) {
    const int b = binOf(size(ch));
    if (bck(ch)) fwd(bck(ch)) = fwd(ch);
    else bins[b] = fwd(ch);
    if (fwd(ch)) bck(fwd(ch)) = bck(ch);
    if (!bins[b]) unmark(b);
}
// Writes a free chunk's boundary tags (size in head, footer in next chunk's prevSize).
void setFree(uint32_t ch, uint32_t sz) {
    head(ch) = sz | (head(ch) & PREV_INUSE);
    prevSize(ch + sz) = sz;
    head(ch + sz) &= ~PREV_INUSE;
}

bool newSegment() {
    const uint32_t seg = lowVirtualAlloc(0, kSeg, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!seg) return false;
    // Retire the old wilderness into the bins.
    if (top && size(top) >= kMin) {
        const uint32_t sz = size(top);
        setFree(top, sz);
        link(top);
    }
    top = seg;
    topEnd = seg + kSeg - 8;  // fence chunk header lives at topEnd
    head(top) = (kSeg - 8) | PREV_INUSE;
    head(topEnd) = 0 | INUSE;
    return true;
}

uint32_t take(uint32_t ch, uint32_t need) {
    // ch is free and unlinked, size >= need: split remainder off.
    const uint32_t sz = size(ch);
    if (sz - need >= kMin) {
        const uint32_t rest = ch + need;
        head(ch) = need | INUSE | (head(ch) & PREV_INUSE);
        head(rest) = (sz - need) | PREV_INUSE;
        setFree(rest, sz - need);
        link(rest);
    } else {
        head(ch) |= INUSE;
        head(ch + sz) |= PREV_INUSE;
    }
    return ch;
}

uint32_t alloc(uint32_t n) {
    if (n >= kBig) {
        const uint32_t total = (n + 16 + 0xFFFF) & ~0xFFFFu;
        const uint32_t base = lowVirtualAlloc(0, total, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (!base) return 0;
        *gp<uint32_t>(base) = total;
        head(base + 8) = n | INUSE | BIG;  // header at base+8, payload base+16
        return base + 16;
    }
    uint32_t need = (n + 8 + 7) & ~7u;
    if (need < kMin) need = kMin;
    AcquireSRWLockExclusive(&lock);
    uint32_t ch = 0;
    int b = binOf(need);
    if (b < kSmallBins && bins[b]) {
        ch = bins[b];
        unlink(ch);
    } else {
        // first fit in the bin itself (large bins hold a range), then any higher bin
        for (uint32_t x = bins[b]; x && b >= kSmallBins; x = fwd(x))
            if (size(x) >= need) { ch = x; unlink(ch); break; }
        if (!ch) {
            for (int i = b + 1; i < kSmallBins + kLargeBins; ++i) {
                if (!(binMap[i >> 6] >> (i & 63) & 1)) continue;
                ch = bins[i];
                unlink(ch);
                break;
            }
        }
    }
    if (ch) {
        take(ch, need);
    } else {
        if (!top || size(top) < need + kMin) {
            if (!newSegment()) { ReleaseSRWLockExclusive(&lock); return 0; }
        }
        ch = top;
        const uint32_t sz = size(top);
        top = ch + need;
        head(ch) = need | INUSE | (head(ch) & PREV_INUSE);
        head(top) = (sz - need) | PREV_INUSE;
    }
    bytesInUse += size(ch);
    ReleaseSRWLockExclusive(&lock);
    return ch + 8;
}

void release(uint32_t p) {
    if (!p) return;
    uint32_t ch = p - 8;
    if (head(ch) & BIG) {
        VirtualFree(gp(p - 16), 0, MEM_RELEASE);
        return;
    }
    AcquireSRWLockExclusive(&lock);
    if (!(head(ch) & INUSE)) { ReleaseSRWLockExclusive(&lock); log("heap: double free of 0x%08X", p); return; }
    uint32_t sz = size(ch);
    bytesInUse -= sz;
    head(ch) &= ~INUSE;
    if (!(head(ch) & PREV_INUSE)) {  // merge backwards
        const uint32_t ps = prevSize(ch);
        ch -= ps;
        unlink(ch);
        sz += ps;
    }
    const uint32_t next = ch + sz;
    if (next == top) {  // merge into wilderness
        head(ch) = (sz + size(top)) | (head(ch) & PREV_INUSE);
        top = ch;
        ReleaseSRWLockExclusive(&lock);
        return;
    }
    if (!(head(next) & INUSE)) {
        unlink(next);
        sz += size(next);
    }
    head(ch) = sz | (head(ch) & PREV_INUSE);
    setFree(ch, sz);
    link(ch);
    ReleaseSRWLockExclusive(&lock);
}

uint32_t usable(uint32_t p) {
    const uint32_t ch = p - 8;
    if (head(ch) & BIG) return head(ch) & ~FLAGS;
    return size(ch) - 8;
}
}  // namespace heap

uint32_t gmalloc(uint32_t n) { return heap::alloc(n ? n : 1); }
uint32_t gcalloc(uint32_t n, uint32_t s) {
    const uint64_t total = uint64_t(n) * s;
    if (total > 0x7FFFFFFF) return 0;
    const uint32_t p = gmalloc(static_cast<uint32_t>(total));
    if (p) std::memset(gp(p), 0, static_cast<size_t>(total));
    return p;
}
void gfree(uint32_t p) { heap::release(p); }
uint32_t gmsize(uint32_t p) { return p ? heap::usable(p) : 0; }
uint32_t grealloc(uint32_t p, uint32_t n) {
    if (!p) return gmalloc(n);
    if (!n) { gfree(p); return 0; }
    const uint32_t old = gmsize(p);
    if (n <= old && n + 64 >= old / 2) return p;
    const uint32_t q = gmalloc(n);
    if (!q) return 0;
    std::memcpy(gp(q), gp(p), old < n ? old : n);
    gfree(p);
    return q;
}
uint32_t gmemdup(const void* p, uint32_t n) {
    const uint32_t q = gmalloc(n);
    std::memcpy(gp(q), p, n);
    return q;
}
uint32_t gstrdup(const char* s) { return gmemdup(s, static_cast<uint32_t>(std::strlen(s) + 1)); }
uint32_t gwcsdup(const wchar_t* s) { return gmemdup(s, static_cast<uint32_t>((wcslen(s) + 1) * 2)); }

bool memInit() {
    // The PE image goes at its retail base; nothing else may be there.
    void* img = VirtualAlloc(gp(kImageBase), kImageEnd - kImageBase, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (img != gp(kImageBase)) {
        log("cannot reserve the guest image range 0x%08X-0x%08X (error %lu)", kImageBase, kImageEnd, GetLastError());
        return false;
    }
    return heap::newSegment();
}

// ============================================================================
// traps and imports
// ============================================================================
struct Trap {
    std::string name;
    Handler fn = nullptr;
    DataHandler dfn = nullptr;
    uintptr_t data = 0;
};
static std::vector<Trap>* g_traps;
static SRWLOCK g_trapLock = SRWLOCK_INIT;

static uint32_t addTrapImpl(Trap t) {
    AcquireSRWLockExclusive(&g_trapLock);
    if (!g_traps) { g_traps = new std::vector<Trap>; g_traps->reserve(65536); }
    const uint32_t a = kTrapBase + 16u * static_cast<uint32_t>(g_traps->size());
    if (a >= kTrapEnd) die("out of trap addresses");
    g_traps->push_back(std::move(t));
    ReleaseSRWLockExclusive(&g_trapLock);
    return a;
}
uint32_t addTrap(const char* name, Handler h) { return addTrapImpl(Trap{name, h, nullptr, 0}); }
uint32_t addTrap(const char* name, DataHandler h, uintptr_t data) { return addTrapImpl(Trap{name, nullptr, h, data}); }
const char* trapName(uint32_t a) {
    if (a < kTrapBase || !g_traps) return nullptr;
    const uint32_t i = (a - kTrapBase) / 16;
    return i < g_traps->size() ? (*g_traps)[i].name.c_str() : nullptr;
}

static std::string lowerDll(const char* dll) {
    std::string s(dll);
    if (auto p = s.find_last_of("\\/"); p != std::string::npos) s = s.substr(p + 1);
    for (auto& ch : s) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    if (s.size() < 4 || s.compare(s.size() - 4, 4, ".dll") != 0) s += ".dll";
    return s;
}
struct ImportEntry { Handler fn; uint32_t dataAddr; uint32_t trap; };
static std::map<std::string, std::map<std::string, ImportEntry>>& importTable() {
    static auto* t = new std::map<std::string, std::map<std::string, ImportEntry>>;
    return *t;
}
void registerImport(const char* dll, const char* name, Handler fn) { importTable()[lowerDll(dll)][name] = {fn, 0, 0}; }
void registerDataImport(const char* dll, const char* name, uint32_t addr) { importTable()[lowerDll(dll)][name] = {nullptr, addr, 0}; }
bool isKnownDll(const char* dll) { return importTable().count(lowerDll(dll)) != 0; }

uint32_t resolveImport(const char* dll, const char* name) {
    auto d = importTable().find(lowerDll(dll));
    if (d == importTable().end()) return 0;
    auto f = d->second.find(name);
    if (f == d->second.end()) return 0;
    ImportEntry& e = f->second;
    if (e.dataAddr) return e.dataAddr;
    if (!e.trap) e.trap = addTrap((d->first + "!" + name).c_str(), e.fn);
    return e.trap;
}

static void missingImport(Ctx* c, uintptr_t data) {
    die("unimplemented import %s (called from 0x%08X)", reinterpret_cast<const char*>(data), rd32(c->esp));
}
uint32_t resolveImportOrStub(const char* dll, const char* name) {
    if (uint32_t a = resolveImport(dll, name)) return a;
    const std::string full = lowerDll(dll) + "!" + name;
    char* keep = _strdup(full.c_str());
    return addTrap(keep, &missingImport, reinterpret_cast<uintptr_t>(keep));
}

static int onUnknownTarget(Ctx* c, uint32_t target) {
    if (target >= kTrapBase && target < kTrapEnd && g_traps) {
        const uint32_t i = (target - kTrapBase) / 16;
        AcquireSRWLockShared(&g_trapLock);
        const bool ok = i < g_traps->size();
        Trap* t = ok ? &(*g_traps)[i] : nullptr;
        ReleaseSRWLockShared(&g_trapLock);
        if (!t) return 0;
        HLOG(2, "-> %s (ret 0x%08X)", t->name.c_str(), rd32(c->esp));
        if (t->fn) t->fn(c);
        else t->dfn(c, t->data);
        return 1;
    }
    return 0;
}

static void onFatal(Ctx* c, uint32_t eip, const char* what) {
    die("guest fault: %s at 0x%08X (eax=%08X ecx=%08X edx=%08X ebx=%08X esp=%08X ebp=%08X esi=%08X edi=%08X, [esp]=%08X)", what, eip,
        c->eax, c->ecx, c->edx, c->ebx, c->esp, c->ebp, c->esi, c->edi, c->esp ? rd32(c->esp) : 0);
}

// ============================================================================
// threads
// ============================================================================
static thread_local GuestThread* t_thread;
static uint32_t g_peb;

uint32_t pebAddr() { return g_peb; }

GuestThread* newGuestThread(uint32_t stackSize) {
    if (stackSize < 0x40000) stackSize = 0x100000;
    stackSize = (stackSize + 0xFFFF) & ~0xFFFFu;
    auto* t = new GuestThread;
    const uint32_t mem = lowVirtualAlloc(0, stackSize + 0x10000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!mem) die("cannot allocate a guest stack");
    t->stackLo = mem;
    t->stackHi = mem + stackSize;
    t->teb = mem + stackSize;  // TEB (and TLS slots) right above the stack
    const uint32_t teb = t->teb;
    wr32(teb + 0x00, 0xFFFFFFFFu);  // SEH chain end
    wr32(teb + 0x04, t->stackHi);
    wr32(teb + 0x08, t->stackLo);
    wr32(teb + 0x18, teb);
    wr32(teb + 0x20, GetCurrentProcessId());
    wr32(teb + 0x2C, teb + 0xE10);  // TLS slot array
    wr32(teb + 0x30, g_peb);
    t->ctx.fs_base = teb;
    t->ctx.esp = t->stackHi - 16;
    t->ctx.fpu.cw = 0x027F;
    t->ctx.mxcsr = 0x1F80;
    return t;
}

void bindThread(GuestThread* t) {
    t->tid = GetCurrentThreadId();
    wr32(t->teb + 0x24, t->tid);
    t_thread = t;
}

GuestThread* currentThread() {
    if (!t_thread) bindThread(newGuestThread(0x100000));
    return t_thread;
}

void threadsInit() {
    g_peb = lowVirtualAlloc(0, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    wr8(g_peb + 0x02, 0);                // BeingDebugged
    wr32(g_peb + 0x08, kImageBase);      // ImageBaseAddress
    wr32(g_peb + 0x18, 0x00140000);      // ProcessHeap (matches GetProcessHeap)
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    wr32(g_peb + 0x64, si.dwNumberOfProcessors);
    wr32(g_peb + 0xA4, 5);  // OSMajorVersion: report XP-era 5.1
    wr32(g_peb + 0xA8, 1);
    recomp_on_unknown_target = onUnknownTarget;
    recomp_on_fatal = onFatal;
}

static uint32_t callImpl(uint32_t fn, bool hasThis, uint32_t thisPtr, std::initializer_list<uint32_t> args) {
    Ctx* c = cur();
    const Ctx saved = *c;
    uint32_t esp = c->esp;
    const uint32_t* a = args.end();
    while (a != args.begin()) { --a; esp -= 4; wr32(esp, *a); }
    esp -= 4;
    wr32(esp, kReturnSentinel);
    c->esp = esp;
    if (hasThis) c->ecx = thisPtr;
    recomp_dispatch(c, fn);
    const uint32_t result = c->eax;
    const X87 fpu = c->fpu;
    *c = saved;
    c->fpu = fpu;  // x87 results (e.g. float returns) stay visible to the caller
    c->eax = result;
    return result;
}
uint32_t guestCall(uint32_t fn, std::initializer_list<uint32_t> args) { return callImpl(fn, false, 0, args); }
uint32_t guestCallThis(uint32_t fn, uint32_t thisPtr, std::initializer_list<uint32_t> args) { return callImpl(fn, true, thisPtr, args); }

struct ThreadStart { GuestThread* t; uint32_t fn, param; };
static DWORD WINAPI threadMain(void* p) {
    auto* s = static_cast<ThreadStart*>(p);
    bindThread(s->t);
    const uint32_t fn = s->fn, param = s->param;
    delete s;
    const uint32_t r = guestCall(fn, {param});
    return r;
}

HANDLE startGuestThread(uint32_t fn, uint32_t param, uint32_t stackSize, DWORD flags, DWORD* tid) {
    auto* s = new ThreadStart{newGuestThread(stackSize), fn, param};
    // Recompiled code nests one host frame per guest call: give it room.
    return CreateThread(nullptr, 64u << 20, threadMain, s, (flags & CREATE_SUSPENDED) | STACK_SIZE_PARAM_IS_A_RESERVATION, tid);
}

// ============================================================================
// module handles
// ============================================================================
struct ModuleRec { uint32_t g; HMODULE h; std::string name; };
static std::vector<ModuleRec> g_modules;
static SRWLOCK g_modLock = SRWLOCK_INIT;

uint32_t moduleToGuest(HMODULE m, const char* name) {
    AcquireSRWLockExclusive(&g_modLock);
    for (auto& r : g_modules)
        if ((m && r.h == m) || (!m && _stricmp(r.name.c_str(), name) == 0)) { ReleaseSRWLockExclusive(&g_modLock); return r.g; }
    const uint32_t g = 0xFFB00000u + 0x1000u * static_cast<uint32_t>(g_modules.size() + 1);
    g_modules.push_back({g, m, name ? name : ""});
    ReleaseSRWLockExclusive(&g_modLock);
    return g;
}
void registerModule(uint32_t g, HMODULE h, const char* name) {
    AcquireSRWLockExclusive(&g_modLock);
    g_modules.push_back({g, h, name ? name : ""});
    ReleaseSRWLockExclusive(&g_modLock);
}
HMODULE moduleFromGuest(uint32_t g) {
    if (g == kImageBase || g == 0) return g_fableRes;
    AcquireSRWLockShared(&g_modLock);
    HMODULE h = nullptr;
    for (auto& r : g_modules) if (r.g == g) h = r.h;
    ReleaseSRWLockShared(&g_modLock);
    return h;
}
const char* moduleName(uint32_t g) {
    if (g == kImageBase || g == 0) return "fable.exe";
    AcquireSRWLockShared(&g_modLock);
    const char* n = nullptr;
    for (auto& r : g_modules) if (r.g == g) n = r.name.c_str();
    ReleaseSRWLockShared(&g_modLock);
    return n;
}
HINSTANCE instFromGuest(uint32_t g) {
    if (g == kImageBase || g == 0) return g_hinst;
    return moduleFromGuest(g);
}

}  // namespace host
