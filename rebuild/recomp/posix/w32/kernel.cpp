// Virtual memory, system information, time, process and environment, modules and PE
// resources, code pages and locale, error messages, vectored exception handlers.
#include "w32.hpp"

#include <cerrno>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <functional>
#include <map>
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <thread>
#include <ucontext.h>
#include <unistd.h>

extern char** environ;

namespace w32 {

// ============================================================================
// virtual memory: the guest window [kLow, kHigh) is reserved up front and managed here,
// page by page (4 KiB, the Windows page size), so VirtualQuery is exact.
// ============================================================================
namespace vm {
#ifdef __ANDROID__
// ART keeps its Java heap and boot image in the low 4 GiB of every app process, so the window
// has holes; it runs up to 3 GiB to make up for them (allocations fill from the bottom, so the
// game only sees addresses above 2 GiB once the lower part is full).
constexpr uintptr_t kLow = 0x00010000, kHigh = 0xC0000000, kPage = 0x1000;
#else
constexpr uintptr_t kLow = 0x00010000, kHigh = 0x80000000, kPage = 0x1000;
#endif
constexpr size_t kPages = (kHigh - kLow) / kPage;
enum : uint8_t { FREE = 0, RESERVED = 1, COMMITTED = 2 };
std::mutex lock;
uint8_t* state;            // per page
uint32_t* allocBase;       // per page: allocation base (0 = free)
uint8_t* prot;             // per page: Windows protection (low byte)
size_t hostPage;
bool ready;

size_t idx(uintptr_t a) { return (a - kLow) / kPage; }
bool inWindow(uintptr_t a, size_t n) { return a >= kLow && a + n <= kHigh && a + n >= a; }

void init() {
    if (ready) return;
    hostPage = static_cast<size_t>(sysconf(_SC_PAGESIZE));
    state = new uint8_t[kPages]();
    allocBase = new uint32_t[kPages]();
    prot = new uint8_t[kPages]();
    // Reserve the window; parts already taken by someone else are marked unusable.
    void* p = mmap(reinterpret_cast<void*>(kLow), kHigh - kLow, PROT_NONE,
                   MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE | MAP_FIXED_NOREPLACE, -1, 0);
    if (p != reinterpret_cast<void*>(kLow)) {
        if (p != MAP_FAILED) munmap(p, kHigh - kLow);
        // Piecewise: 64 KiB chunks, skipping occupied ones.
        for (uintptr_t a = kLow; a < kHigh; a += 0x10000) {
            void* q = mmap(reinterpret_cast<void*>(a), 0x10000, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE | MAP_FIXED_NOREPLACE, -1, 0);
            if (q == reinterpret_cast<void*>(a)) continue;
            if (q != MAP_FAILED) munmap(q, 0x10000);
            for (size_t i = idx(a); i < idx(a + 0x10000); ++i) state[i] = RESERVED, allocBase[i] = 0xFFFFFFFFu;  // foreign
        }
    }
    ready = true;
}

int hostProt(DWORD p) {
    switch (p & 0xFF) {
        case PAGE_NOACCESS: return PROT_NONE;
        case PAGE_READONLY: case PAGE_EXECUTE_READ: case PAGE_EXECUTE: return PROT_READ;
        default: return PROT_READ | PROT_WRITE;
    }
}

// mprotect over host pages covering [a, a+n): a host page stays accessible while any of its
// 4 KiB pages is committed.
void applyProt(uintptr_t a, size_t n) {
    const uintptr_t lo = a & ~(hostPage - 1), hi = (a + n + hostPage - 1) & ~(hostPage - 1);
    for (uintptr_t hp = lo; hp < hi; hp += hostPage) {
        int pr = PROT_NONE;
        for (uintptr_t q = hp; q < hp + hostPage; q += kPage)
            if (q >= kLow && q < kHigh && state[idx(q)] == COMMITTED) pr |= hostProt(prot[idx(q)]);
        mprotect(reinterpret_cast<void*>(hp), hostPage, pr);
    }
}

uintptr_t findFree(size_t size) {
    for (uintptr_t a = 0x00100000; a + size <= kHigh; a += 0x10000) {
        size_t i = idx(a), n = size / kPage;
        size_t k = 0;
        while (k < n && state[i + k] == FREE) ++k;
        if (k == n) return a;
        a += (k / 16) * 0x10000;  // skip the free prefix we already checked
    }
    return 0;
}
}  // namespace vm

// ============================================================================
// PE resources (resource-only modules: LoadLibraryEx with LOAD_LIBRARY_AS_DATAFILE)
// ============================================================================
struct PeModule {
    std::string path;
    std::vector<uint8_t> file;
    uint32_t rsrcRva = 0, rsrcOff = 0, rsrcSize = 0;
    struct Sec { uint32_t va, vsize, off, rsize; };
    std::vector<Sec> secs;
    const uint8_t* rva(uint32_t r) const {
        for (const Sec& s : secs)
            if (r >= s.va && r < s.va + std::max(s.vsize, s.rsize)) return file.data() + s.off + (r - s.va);
        return nullptr;
    }
};
struct ResEntry { const uint8_t* data; uint32_t size; };

static bool loadPe(PeModule& m) {
    std::ifstream f(m.path, std::ios::binary);
    if (!f) return false;
    m.file.assign(std::istreambuf_iterator<char>(f), {});
    if (m.file.size() < 0x40 || m.file[0] != 'M' || m.file[1] != 'Z') return false;
    const uint32_t pe = *reinterpret_cast<const uint32_t*>(&m.file[0x3C]);
    if (pe + 0x18 > m.file.size()) return false;
    const uint16_t nsec = *reinterpret_cast<const uint16_t*>(&m.file[pe + 6]);
    const uint16_t optSize = *reinterpret_cast<const uint16_t*>(&m.file[pe + 20]);
    const uint16_t magic = *reinterpret_cast<const uint16_t*>(&m.file[pe + 24]);
    const uint32_t dirOff = pe + 24 + (magic == 0x20B ? 112 : 96);
    m.rsrcRva = *reinterpret_cast<const uint32_t*>(&m.file[dirOff + 2 * 8]);
    m.rsrcSize = *reinterpret_cast<const uint32_t*>(&m.file[dirOff + 2 * 8 + 4]);
    for (uint16_t i = 0; i < nsec; ++i) {
        const uint8_t* s = &m.file[pe + 24 + optSize + 40 * i];
        m.secs.push_back({*reinterpret_cast<const uint32_t*>(s + 12), *reinterpret_cast<const uint32_t*>(s + 8),
                          *reinterpret_cast<const uint32_t*>(s + 20), *reinterpret_cast<const uint32_t*>(s + 16)});
    }
    return true;
}

// Walks one level of the resource directory; id is an integer or a (case-insensitive) name.
static const uint8_t* resChild(const PeModule& m, const uint8_t* dir, uintptr_t id, bool first, uint32_t* outId = nullptr) {
    const uint8_t* base = m.rva(m.rsrcRva);
    if (!base || !dir) return nullptr;
    const uint16_t named = *reinterpret_cast<const uint16_t*>(dir + 12), ids = *reinterpret_cast<const uint16_t*>(dir + 14);
    const uint8_t* e = dir + 16;
    for (uint32_t i = 0; i < static_cast<uint32_t>(named + ids); ++i, e += 8) {
        const uint32_t name = *reinterpret_cast<const uint32_t*>(e), off = *reinterpret_cast<const uint32_t*>(e + 4);
        bool match = first;
        if (!first) {
            if (id < 0x10000) match = !(name & 0x80000000u) && name == id;
            else if (name & 0x80000000u) {
                const uint8_t* s = base + (name & 0x7FFFFFFFu);
                const uint16_t len = *reinterpret_cast<const uint16_t*>(s);
                const wchar_t* str = reinterpret_cast<const wchar_t*>(s + 2);
                const auto* want = reinterpret_cast<const wchar_t*>(id);
                match = wlen(want) == len;
                for (uint16_t k = 0; match && k < len; ++k) match = upper(str[k]) == upper(want[k]);
            }
        }
        if (match) {
            if (outId) *outId = name;
            return base + (off & 0x7FFFFFFFu);  // subdirectory or (at the last level) data entry
        }
    }
    return nullptr;
}

static bool findRes(const PeModule& m, uintptr_t type, uintptr_t name, WORD lang, ResEntry& out) {
    const uint8_t* root = m.rva(m.rsrcRva);
    if (!root) return false;
    const uint8_t* t = resChild(m, root, type, false);
    if (!t) return false;
    const uint8_t* n = resChild(m, t, name, false);
    if (!n) return false;
    const uint8_t* l = resChild(m, n, lang, false);
    if (!l) l = resChild(m, n, 0, true);
    if (!l) return false;
    const uint32_t dataRva = *reinterpret_cast<const uint32_t*>(l), size = *reinterpret_cast<const uint32_t*>(l + 4);
    out.data = m.rva(dataRva);
    out.size = size;
    return out.data != nullptr;
}

// ============================================================================
// exceptions: POSIX signals -> vectored handlers
// ============================================================================
namespace {
std::mutex g_vehLock;
std::vector<PVECTORED_EXCEPTION_HANDLER> g_veh;

void onSignal(int sig, siginfo_t* si, void* uc) {
    EXCEPTION_RECORD rec{};
    CONTEXT ctx{};
    rec.ExceptionCode = sig == SIGSEGV || sig == SIGBUS ? EXCEPTION_ACCESS_VIOLATION
                       : sig == SIGILL                 ? EXCEPTION_ILLEGAL_INSTRUCTION
                       : sig == SIGFPE                 ? EXCEPTION_INT_DIVIDE_BY_ZERO
                                                       : EXCEPTION_BREAKPOINT;
    rec.NumberParameters = 2;
    rec.ExceptionInformation[1] = reinterpret_cast<ULONG_PTR>(si->si_addr);
    auto* u = static_cast<ucontext_t*>(uc);
#if defined(__x86_64__)
    rec.ExceptionAddress = reinterpret_cast<void*>(u->uc_mcontext.gregs[REG_RIP]);
    ctx.Rip = u->uc_mcontext.gregs[REG_RIP];
    ctx.Rsp = u->uc_mcontext.gregs[REG_RSP];
    rec.ExceptionInformation[0] = (u->uc_mcontext.gregs[REG_ERR] & 2) ? 1 : 0;
#elif defined(__aarch64__)
    rec.ExceptionAddress = reinterpret_cast<void*>(u->uc_mcontext.pc);
    ctx.Pc = u->uc_mcontext.pc;
    ctx.Sp = u->uc_mcontext.sp;
#endif
    EXCEPTION_POINTERS ep{&rec, &ctx};
    std::vector<PVECTORED_EXCEPTION_HANDLER> hs;
    {
        std::lock_guard<std::mutex> l(g_vehLock);
        hs = g_veh;
    }
    for (auto h : hs)
        if (h(&ep) == EXCEPTION_CONTINUE_EXECUTION) return;
    signal(sig, SIG_DFL);
    raise(sig);
}
}  // namespace

// ============================================================================
// pseudo modules: host-implemented "DLLs" handed out by LoadLibrary/GetProcAddress
// ============================================================================
struct ModuleRec {
    std::string name;  // lower case, with .dll
    std::map<std::string, void*> exports;
    std::shared_ptr<PeModule> pe;  // resource-only modules
};
namespace {
std::mutex g_modLock;
// Filled from static initializers in other files, so constructed on first use.
std::vector<std::unique_ptr<ModuleRec>>& modules() {
    static auto* m = new std::vector<std::unique_ptr<ModuleRec>>;
    return *m;
}
#define g_mods modules()

std::string lowerName(const std::string& s) {
    std::string n = s;
    if (auto p = n.find_last_of("\\/"); p != std::string::npos) n = n.substr(p + 1);
    for (auto& c : n) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    if (n.find('.') == std::string::npos) n += ".dll";
    return n;
}
}  // namespace

void registerHostModule(const char* dll, const char* fn, void* addr) {
    std::lock_guard<std::mutex> l(g_modLock);
    const std::string n = lowerName(dll);
    for (auto& m : g_mods)
        if (m->name == n) { m->exports[fn] = addr; return; }
    auto m = std::make_unique<ModuleRec>();
    m->name = n;
    m->exports[fn] = addr;
    g_mods.push_back(std::move(m));
}

}  // namespace w32

using namespace w32;

extern "C" {

// ---- virtual memory -----------------------------------------------------------------------
LPVOID WINAPI VirtualAlloc(LPVOID where, SIZE_T size, DWORD type, DWORD protect) {
    using namespace vm;
    if (!size) { setError(ERROR_INVALID_PARAMETER); return nullptr; }
    std::lock_guard<std::mutex> l(lock);
    init();
    uintptr_t a = reinterpret_cast<uintptr_t>(where);
    if (!(type & (MEM_RESERVE | MEM_COMMIT))) { setError(ERROR_INVALID_PARAMETER); return nullptr; }
    if (!a) {
        const size_t total = (size + 0xFFFF) & ~size_t(0xFFFF);
        a = findFree(total);
        if (!a) { setError(ERROR_NOT_ENOUGH_MEMORY); return nullptr; }
        for (size_t i = idx(a); i < idx(a + total); ++i) state[i] = RESERVED, allocBase[i] = static_cast<uint32_t>(a), prot[i] = static_cast<uint8_t>(protect);
        type |= MEM_RESERVE;
        size = total;
    } else if (type & MEM_RESERVE) {
        a &= ~uintptr_t(0xFFFF);
        const size_t total = ((reinterpret_cast<uintptr_t>(where) + size + 0xFFF) & ~uintptr_t(0xFFF)) - a;
        if (!inWindow(a, total)) { setError(ERROR_INVALID_ADDRESS); return nullptr; }
        for (size_t i = idx(a); i < idx(a + total); ++i)
            if (state[i] != FREE) { setError(ERROR_INVALID_ADDRESS); return nullptr; }
        for (size_t i = idx(a); i < idx(a + total); ++i) state[i] = RESERVED, allocBase[i] = static_cast<uint32_t>(a), prot[i] = static_cast<uint8_t>(protect);
        size = total;
    }
    if (type & MEM_COMMIT) {
        const uintptr_t lo = a & ~(kPage - 1), hi = (reinterpret_cast<uintptr_t>(where ? where : reinterpret_cast<void*>(a)) + size + kPage - 1) & ~(kPage - 1);
        if (!inWindow(lo, hi - lo)) { setError(ERROR_INVALID_ADDRESS); return nullptr; }
        for (size_t i = idx(lo); i < idx(hi); ++i)
            if (state[i] == FREE || allocBase[i] == 0xFFFFFFFFu) { setError(ERROR_INVALID_ADDRESS); return nullptr; }
        for (size_t i = idx(lo); i < idx(hi); ++i) state[i] = COMMITTED, prot[i] = static_cast<uint8_t>(protect);
        applyProt(lo, hi - lo);
        return reinterpret_cast<void*>(where ? lo : a);
    }
    return reinterpret_cast<void*>(a);
}

BOOL WINAPI VirtualFree(LPVOID where, SIZE_T size, DWORD type) {
    using namespace vm;
    std::lock_guard<std::mutex> l(lock);
    init();
    const uintptr_t a = reinterpret_cast<uintptr_t>(where);
    if (!inWindow(a, 1) || state[idx(a)] == FREE) return fail(ERROR_INVALID_ADDRESS);
    if (type & MEM_RELEASE) {
        if (allocBase[idx(a)] != a) return fail(ERROR_INVALID_ADDRESS);
        size_t i = idx(a);
        const size_t first = i;
        while (i < kPages && allocBase[i] == a) state[i] = FREE, allocBase[i] = 0, ++i;
        const uintptr_t end = kLow + i * kPage;
        // discard contents and re-protect
        const uintptr_t lo = (kLow + first * kPage) & ~(hostPage - 1), hi = (end + hostPage - 1) & ~(hostPage - 1);
        madvise(reinterpret_cast<void*>(lo), hi - lo, MADV_DONTNEED);
        applyProt(kLow + first * kPage, end - (kLow + first * kPage));
        return TRUE;
    }
    if (type & MEM_DECOMMIT) {
        const uintptr_t lo = a & ~(kPage - 1), hi = (a + size + kPage - 1) & ~(kPage - 1);
        for (size_t i = idx(lo); i < idx(hi) && i < kPages; ++i)
            if (state[i] == COMMITTED) state[i] = RESERVED;
        applyProt(lo, hi - lo);
        return TRUE;
    }
    return fail(ERROR_INVALID_PARAMETER);
}

SIZE_T WINAPI VirtualQuery(LPCVOID where, PMEMORY_BASIC_INFORMATION out, SIZE_T len) {
    using namespace vm;
    if (len < sizeof *out) return 0;
    std::lock_guard<std::mutex> l(lock);
    init();
    const uintptr_t a = reinterpret_cast<uintptr_t>(where) & ~(kPage - 1);
    std::memset(out, 0, sizeof *out);
    if (!inWindow(a, 1)) {
        // Outside the guest window: report it as busy host memory.
        out->BaseAddress = reinterpret_cast<void*>(a);
        out->AllocationBase = out->BaseAddress;
        out->RegionSize = kPage;
        out->State = MEM_COMMIT;
        out->Protect = out->AllocationProtect = PAGE_READWRITE;
        out->Type = MEM_PRIVATE;
        if (a < kLow) { out->State = MEM_FREE; out->Protect = PAGE_NOACCESS; out->RegionSize = kLow - a; }
        return sizeof *out;
    }
    size_t i = idx(a);
    const uint8_t st = state[i];
    const uint32_t base = allocBase[i];
    size_t j = i;
    while (j < kPages && state[j] == st && allocBase[j] == base && (st == FREE || prot[j] == prot[i])) ++j;
    out->BaseAddress = reinterpret_cast<void*>(a);
    out->RegionSize = (j - i) * kPage;
    if (st == FREE) {
        out->State = MEM_FREE;
        out->Protect = PAGE_NOACCESS;
    } else {
        out->AllocationBase = reinterpret_cast<void*>(static_cast<uintptr_t>(base == 0xFFFFFFFFu ? a : base));
        out->AllocationProtect = PAGE_READWRITE;
        out->State = st == COMMITTED ? MEM_COMMIT : MEM_RESERVE;
        out->Protect = st == COMMITTED ? prot[i] : 0;
        out->Type = MEM_PRIVATE;
    }
    return sizeof *out;
}

BOOL WINAPI VirtualProtect(LPVOID where, SIZE_T size, DWORD np, PDWORD old) {
    using namespace vm;
    std::lock_guard<std::mutex> l(lock);
    init();
    const uintptr_t lo = reinterpret_cast<uintptr_t>(where) & ~(kPage - 1), hi = (reinterpret_cast<uintptr_t>(where) + size + kPage - 1) & ~(kPage - 1);
    if (!inWindow(lo, hi - lo)) return fail(ERROR_INVALID_ADDRESS);
    if (old) *old = prot[idx(lo)];
    for (size_t i = idx(lo); i < idx(hi); ++i) prot[i] = static_cast<uint8_t>(np);
    applyProt(lo, hi - lo);
    return TRUE;
}

BOOL WINAPI GlobalMemoryStatusEx(LPMEMORYSTATUSEX m) {
    struct sysinfo si;
    sysinfo(&si);
    m->dwMemoryLoad = si.totalram ? static_cast<DWORD>(100 - (100ull * si.freeram / si.totalram)) : 0;
    m->ullTotalPhys = static_cast<uint64_t>(si.totalram) * si.mem_unit;
    m->ullAvailPhys = static_cast<uint64_t>(si.freeram + si.bufferram) * si.mem_unit;
    m->ullTotalPageFile = m->ullTotalPhys + static_cast<uint64_t>(si.totalswap) * si.mem_unit;
    m->ullAvailPageFile = m->ullAvailPhys + static_cast<uint64_t>(si.freeswap) * si.mem_unit;
    m->ullTotalVirtual = 0x7FFE0000ull;
    m->ullAvailVirtual = 0x70000000ull;
    m->ullAvailExtendedVirtual = 0;
    return TRUE;
}
void WINAPI GlobalMemoryStatus(LPMEMORYSTATUS m) {
    MEMORYSTATUSEX x{sizeof x};
    GlobalMemoryStatusEx(&x);
    m->dwMemoryLoad = x.dwMemoryLoad;
    m->dwTotalPhys = static_cast<SIZE_T>(x.ullTotalPhys);
    m->dwAvailPhys = static_cast<SIZE_T>(x.ullAvailPhys);
    m->dwTotalPageFile = static_cast<SIZE_T>(x.ullTotalPageFile);
    m->dwAvailPageFile = static_cast<SIZE_T>(x.ullAvailPageFile);
    m->dwTotalVirtual = static_cast<SIZE_T>(x.ullTotalVirtual);
    m->dwAvailVirtual = static_cast<SIZE_T>(x.ullAvailVirtual);
}
void WINAPI GetSystemInfo(LPSYSTEM_INFO si) {
    std::memset(si, 0, sizeof *si);
    si->wProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;
    si->dwPageSize = 0x1000;
    si->lpMinimumApplicationAddress = reinterpret_cast<void*>(0x10000);
    si->lpMaximumApplicationAddress = reinterpret_cast<void*>(0x7FFFFFFEFFFFull);
    const long n = sysconf(_SC_NPROCESSORS_ONLN);
    si->dwNumberOfProcessors = n > 0 ? static_cast<DWORD>(n) : 1;
    si->dwActiveProcessorMask = si->dwNumberOfProcessors >= 64 ? ~DWORD_PTR(0) : (DWORD_PTR(1) << si->dwNumberOfProcessors) - 1;
    si->dwProcessorType = PROCESSOR_AMD_X8664;
    si->dwAllocationGranularity = 0x10000;
    si->wProcessorLevel = 6;
}

// ---- time -----------------------------------------------------------------------------------
DWORD WINAPI GetTickCount(void) { return static_cast<DWORD>(monotonicNs() / 1000000ull); }
ULONGLONG WINAPI GetTickCount64(void) { return monotonicNs() / 1000000ull; }
BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER* c) { c->QuadPart = static_cast<LONGLONG>(monotonicNs() / 100); return TRUE; }
BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER* f) { f->QuadPart = 10000000; return TRUE; }
void WINAPI GetSystemTimeAsFileTime(LPFILETIME ft) {
    const uint64_t v = nowFileTime();
    ft->dwLowDateTime = static_cast<DWORD>(v);
    ft->dwHighDateTime = static_cast<DWORD>(v >> 32);
}
static void unixToSystemTime(time_t t, long ms, LPSYSTEMTIME st, bool local) {
    struct tm tm;
    if (local) localtime_r(&t, &tm);
    else gmtime_r(&t, &tm);
    st->wYear = static_cast<WORD>(tm.tm_year + 1900);
    st->wMonth = static_cast<WORD>(tm.tm_mon + 1);
    st->wDayOfWeek = static_cast<WORD>(tm.tm_wday);
    st->wDay = static_cast<WORD>(tm.tm_mday);
    st->wHour = static_cast<WORD>(tm.tm_hour);
    st->wMinute = static_cast<WORD>(tm.tm_min);
    st->wSecond = static_cast<WORD>(tm.tm_sec);
    st->wMilliseconds = static_cast<WORD>(ms);
}
void WINAPI GetSystemTime(LPSYSTEMTIME st) {
    const uint64_t v = nowFileTime();
    unixToSystemTime(static_cast<time_t>(v / 10000000ull) - 11644473600ll, static_cast<long>((v / 10000) % 1000), st, false);
}
void WINAPI GetLocalTime(LPSYSTEMTIME st) {
    const uint64_t v = nowFileTime();
    unixToSystemTime(static_cast<time_t>(v / 10000000ull) - 11644473600ll, static_cast<long>((v / 10000) % 1000), st, true);
}
BOOL WINAPI FileTimeToSystemTime(const FILETIME* ft, LPSYSTEMTIME st) {
    const uint64_t v = ft->dwLowDateTime | (static_cast<uint64_t>(ft->dwHighDateTime) << 32);
    unixToSystemTime(static_cast<time_t>(v / 10000000ull) - 11644473600ll, static_cast<long>((v / 10000) % 1000), st, false);
    return TRUE;
}
BOOL WINAPI SystemTimeToFileTime(const SYSTEMTIME* st, LPFILETIME ft) {
    struct tm tm{};
    tm.tm_year = st->wYear - 1900;
    tm.tm_mon = st->wMonth - 1;
    tm.tm_mday = st->wDay;
    tm.tm_hour = st->wHour;
    tm.tm_min = st->wMinute;
    tm.tm_sec = st->wSecond;
    const time_t t = timegm(&tm);
    const uint64_t v = (static_cast<uint64_t>(t) + 11644473600ull) * 10000000ull + st->wMilliseconds * 10000ull;
    ft->dwLowDateTime = static_cast<DWORD>(v);
    ft->dwHighDateTime = static_cast<DWORD>(v >> 32);
    return TRUE;
}
BOOL WINAPI FileTimeToLocalFileTime(const FILETIME* in, LPFILETIME out) {
    const uint64_t v = in->dwLowDateTime | (static_cast<uint64_t>(in->dwHighDateTime) << 32);
    const time_t t = static_cast<time_t>(v / 10000000ull) - 11644473600ll;
    struct tm tm;
    localtime_r(&t, &tm);
    const uint64_t r = v + static_cast<int64_t>(tm.tm_gmtoff) * 10000000ll;
    out->dwLowDateTime = static_cast<DWORD>(r);
    out->dwHighDateTime = static_cast<DWORD>(r >> 32);
    return TRUE;
}
BOOL WINAPI LocalFileTimeToFileTime(const FILETIME* in, LPFILETIME out) {
    const time_t now = time(nullptr);
    struct tm tm;
    localtime_r(&now, &tm);
    const uint64_t v = (in->dwLowDateTime | (static_cast<uint64_t>(in->dwHighDateTime) << 32)) - static_cast<int64_t>(tm.tm_gmtoff) * 10000000ll;
    out->dwLowDateTime = static_cast<DWORD>(v);
    out->dwHighDateTime = static_cast<DWORD>(v >> 32);
    return TRUE;
}
static int fmtTime(const SYSTEMTIME* st, bool date, LPWSTR out, int n) {
    SYSTEMTIME now;
    if (!st) { GetLocalTime(&now); st = &now; }
    char buf[64];
    if (date) std::snprintf(buf, sizeof buf, "%u/%u/%04u", st->wMonth, st->wDay, st->wYear);
    else std::snprintf(buf, sizeof buf, "%u:%02u:%02u %s", st->wHour % 12 ? st->wHour % 12 : 12, st->wMinute, st->wSecond, st->wHour < 12 ? "AM" : "PM");
    const int len = static_cast<int>(std::strlen(buf));
    if (!n) return len + 1;
    if (n < len + 1) { setError(ERROR_INSUFFICIENT_BUFFER); return 0; }
    for (int i = 0; i <= len; ++i) out[i] = static_cast<unsigned char>(buf[i]);
    return len + 1;
}
int WINAPI GetDateFormatW(LCID, DWORD, const SYSTEMTIME* st, LPCWSTR, LPWSTR out, int n) { return fmtTime(st, true, out, n); }
int WINAPI GetTimeFormatW(LCID, DWORD, const SYSTEMTIME* st, LPCWSTR, LPWSTR out, int n) { return fmtTime(st, false, out, n); }

// ---- process / environment ------------------------------------------------------------------
static wstr g_cmdLine = L"Fable.exe";
void WINAPI ExitProcess(UINT code) {
    std::fflush(nullptr);
    _exit(static_cast<int>(code));
}
BOOL WINAPI TerminateProcess(HANDLE, UINT code) { ExitProcess(code); return TRUE; }
LPWSTR WINAPI GetCommandLineW(void) { return g_cmdLine.data(); }
LPSTR WINAPI GetCommandLineA(void) {
    static std::string a = utf8ToAcp(toUtf8(g_cmdLine.c_str()).c_str());
    return a.data();
}
void w32_setCommandLine(const wchar_t* cmd) { g_cmdLine = cmd; }

LPCH WINAPI GetEnvironmentStrings(void) {
    std::string block;
    for (char** e = environ; *e; ++e) { block += *e; block += '\0'; }
    block += '\0';
    char* p = static_cast<char*>(std::malloc(block.size()));
    std::memcpy(p, block.data(), block.size());
    return p;
}
LPWCH WINAPI GetEnvironmentStringsW(void) {
    wstr block;
    for (char** e = environ; *e; ++e) { block += fromUtf8(*e); block += L'\0'; }
    block += L'\0';
    auto* p = static_cast<wchar_t*>(std::malloc(block.size() * sizeof(wchar_t)));
    std::memcpy(p, block.data(), block.size() * sizeof(wchar_t));
    return p;
}
BOOL WINAPI FreeEnvironmentStringsA(LPCH p) { std::free(p); return TRUE; }
BOOL WINAPI FreeEnvironmentStringsW(LPWCH p) { std::free(p); return TRUE; }
DWORD WINAPI GetEnvironmentVariableW(LPCWSTR name, LPWSTR buf, DWORD n) {
    const char* v = getenv(toUtf8(name).c_str());
    if (!v) { setError(ERROR_ENVVAR_NOT_FOUND); return 0; }
    const wstr w = fromUtf8(v);
    if (w.size() + 1 > n) return static_cast<DWORD>(w.size() + 1);
    for (size_t i = 0; i <= w.size(); ++i) buf[i] = i < w.size() ? w[i] : 0;
    return static_cast<DWORD>(w.size());
}
DWORD WINAPI GetEnvironmentVariableA(LPCSTR name, LPSTR buf, DWORD n) {
    const char* v = getenv(name);
    if (!v) { setError(ERROR_ENVVAR_NOT_FOUND); return 0; }
    const size_t len = std::strlen(v);
    if (len + 1 > n) return static_cast<DWORD>(len + 1);
    std::memcpy(buf, v, len + 1);
    return static_cast<DWORD>(len);
}
void WINAPI OutputDebugStringA(LPCSTR s) { std::fprintf(stderr, "%s", s); }
void WINAPI OutputDebugStringW(LPCWSTR s) { std::fprintf(stderr, "%s", toUtf8(s).c_str()); }
void WINAPI DebugBreak(void) { raise(SIGTRAP); }

DWORD WINAPI GetVersion(void) { return 0x0A280105; }  // 5.1 build 2600 (XP SP-era)
BOOL WINAPI GetVersionExA(LPOSVERSIONINFOA v) {
    v->dwMajorVersion = 5; v->dwMinorVersion = 1; v->dwBuildNumber = 2600; v->dwPlatformId = VER_PLATFORM_WIN32_NT;
    std::strcpy(v->szCSDVersion, "Service Pack 3");
    return TRUE;
}
BOOL WINAPI GetVersionExW(LPOSVERSIONINFOW v) {
    v->dwMajorVersion = 5; v->dwMinorVersion = 1; v->dwBuildNumber = 2600; v->dwPlatformId = VER_PLATFORM_WIN32_NT;
    const wchar_t sp[] = L"Service Pack 3";
    std::memcpy(v->szCSDVersion, sp, sizeof sp);
    return TRUE;
}
UINT WINAPI GetSystemDirectoryA(LPSTR buf, UINT n) {
    const char d[] = "C:\\windows\\system32";
    if (n < sizeof d) return sizeof d;
    std::memcpy(buf, d, sizeof d);
    return sizeof d - 1;
}
UINT WINAPI GetWindowsDirectoryA(LPSTR buf, UINT n) {
    const char d[] = "C:\\windows";
    if (n < sizeof d) return sizeof d;
    std::memcpy(buf, d, sizeof d);
    return sizeof d - 1;
}

// ---- modules ------------------------------------------------------------------------------
static ModuleRec g_self{"fablerecomp.exe", {}, nullptr};
HMODULE WINAPI GetModuleHandleW(LPCWSTR name) {
    if (!name) return reinterpret_cast<HMODULE>(&g_self);
    const std::string n = lowerName(toUtf8(name));
    std::lock_guard<std::mutex> l(g_modLock);
    for (auto& m : g_mods)
        if (m->name == n) return reinterpret_cast<HMODULE>(m.get());
    setError(ERROR_MOD_NOT_FOUND);
    return nullptr;
}
HMODULE WINAPI GetModuleHandleA(LPCSTR name) {
    if (!name) return GetModuleHandleW(nullptr);
    const wstr w = fromUtf8(name);
    return GetModuleHandleW(w.c_str());
}
HMODULE WINAPI LoadLibraryExW(LPCWSTR name, HANDLE, DWORD flags) {
    if (HMODULE h = GetModuleHandleW(name)) return h;
    if (flags & (LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE | LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE)) {
        auto pe = std::make_shared<PeModule>();
        pe->path = toPosixPath(name);
        if (!loadPe(*pe)) { setError(ERROR_MOD_NOT_FOUND); return nullptr; }
        auto m = std::make_unique<ModuleRec>();
        m->name = lowerName(toUtf8(name)) + "#data";
        m->pe = pe;
        std::lock_guard<std::mutex> l(g_modLock);
        g_mods.push_back(std::move(m));
        return reinterpret_cast<HMODULE>(g_mods.back().get());
    }
    setError(ERROR_MOD_NOT_FOUND);
    return nullptr;
}
HMODULE WINAPI LoadLibraryW(LPCWSTR name) { return LoadLibraryExW(name, nullptr, 0); }
HMODULE WINAPI LoadLibraryA(LPCSTR name) {
    const wstr w = fromUtf8(name);
    return LoadLibraryExW(w.c_str(), nullptr, 0);
}
BOOL WINAPI FreeLibrary(HMODULE) { return TRUE; }
FARPROC WINAPI GetProcAddress(HMODULE h, LPCSTR name) {
    auto* m = reinterpret_cast<ModuleRec*>(h);
    if (!m || reinterpret_cast<uintptr_t>(name) < 0x10000) { setError(ERROR_PROC_NOT_FOUND); return nullptr; }
    std::lock_guard<std::mutex> l(g_modLock);
    auto it = m->exports.find(name);
    if (it == m->exports.end()) { setError(ERROR_PROC_NOT_FOUND); return nullptr; }
    return reinterpret_cast<FARPROC>(it->second);
}
DWORD WINAPI GetModuleFileNameW(HMODULE h, LPWSTR buf, DWORD n) {
    wstr path;
    auto* m = reinterpret_cast<ModuleRec*>(h);
    if (!m || m == &g_self) {
        char p[4096];
        const ssize_t len = readlink("/proc/self/exe", p, sizeof p - 1);
        path = toWindowsPath(len > 0 ? std::string(p, static_cast<size_t>(len)) : "/FableRecomp");
    } else if (m->pe) {
        path = toWindowsPath(m->pe->path);
    } else {
        path = L"C:\\windows\\system32\\" + fromUtf8(m->name.c_str());
    }
    if (!n) return 0;
    const size_t len = std::min<size_t>(path.size(), n - 1);
    for (size_t i = 0; i < len; ++i) buf[i] = path[i];
    buf[len] = 0;
    if (len < path.size()) setError(ERROR_INSUFFICIENT_BUFFER);
    return static_cast<DWORD>(len);
}

// ---- resources ------------------------------------------------------------------------------
struct ResHandle { ResEntry e; };
HRSRC WINAPI FindResourceExA(HMODULE h, LPCSTR type, LPCSTR name, WORD lang) {
    auto* m = reinterpret_cast<ModuleRec*>(h);
    if (!m || !m->pe) { setError(ERROR_RESOURCE_DATA_NOT_FOUND); return nullptr; }
    wstr tw, nw;
    uintptr_t t = reinterpret_cast<uintptr_t>(type), n = reinterpret_cast<uintptr_t>(name);
    if (t >= 0x10000) { tw = fromUtf8(type); t = reinterpret_cast<uintptr_t>(tw.c_str()); }
    if (n >= 0x10000) { nw = fromUtf8(name); n = reinterpret_cast<uintptr_t>(nw.c_str()); }
    ResEntry e{};
    if (!findRes(*m->pe, t, n, lang, e)) { setError(ERROR_RESOURCE_NAME_NOT_FOUND); return nullptr; }
    return reinterpret_cast<HRSRC>(new ResHandle{e});
}
HRSRC WINAPI FindResourceA(HMODULE h, LPCSTR name, LPCSTR type) { return FindResourceExA(h, type, name, 0); }
HRSRC WINAPI FindResourceW(HMODULE h, LPCWSTR name, LPCWSTR type) {
    auto* m = reinterpret_cast<ModuleRec*>(h);
    if (!m || !m->pe) return nullptr;
    ResEntry e{};
    if (!findRes(*m->pe, reinterpret_cast<uintptr_t>(type), reinterpret_cast<uintptr_t>(name), 0, e)) return nullptr;
    return reinterpret_cast<HRSRC>(new ResHandle{e});
}
HGLOBAL WINAPI LoadResource(HMODULE, HRSRC r) { return r ? const_cast<uint8_t*>(reinterpret_cast<ResHandle*>(r)->e.data) : nullptr; }
LPVOID WINAPI LockResource(HGLOBAL g) { return g; }
DWORD WINAPI SizeofResource(HMODULE, HRSRC r) { return r ? reinterpret_cast<ResHandle*>(r)->e.size : 0; }
BOOL WINAPI EnumResourceNamesA(HMODULE h, LPCSTR type, ENUMRESNAMEPROCA fn, LONG_PTR lp) {
    auto* m = reinterpret_cast<ModuleRec*>(h);
    if (!m || !m->pe) return FALSE;
    const uint8_t* root = m->pe->rva(m->pe->rsrcRva);
    wstr tw;
    uintptr_t t = reinterpret_cast<uintptr_t>(type);
    if (t >= 0x10000) { tw = fromUtf8(type); t = reinterpret_cast<uintptr_t>(tw.c_str()); }
    const uint8_t* td = resChild(*m->pe, root, t, false);
    if (!td) return FALSE;
    const uint16_t named = *reinterpret_cast<const uint16_t*>(td + 12), ids = *reinterpret_cast<const uint16_t*>(td + 14);
    const uint8_t* e = td + 16;
    for (uint32_t i = 0; i < static_cast<uint32_t>(named + ids); ++i, e += 8) {
        const uint32_t nm = *reinterpret_cast<const uint32_t*>(e);
        std::string keep;
        LPSTR arg;
        if (nm & 0x80000000u) {
            const uint8_t* s = root + (nm & 0x7FFFFFFFu);
            keep = utf8ToAcp(toUtf8(reinterpret_cast<const wchar_t*>(s + 2), *reinterpret_cast<const uint16_t*>(s)).c_str());
            arg = keep.data();
        } else {
            arg = reinterpret_cast<LPSTR>(static_cast<uintptr_t>(nm));
        }
        if (!fn(h, type, arg, lp)) break;
    }
    return TRUE;
}
int w32_loadString(HMODULE h, UINT id, wstr& out) {
    auto* m = reinterpret_cast<ModuleRec*>(h);
    if (!m || !m->pe) return 0;
    ResEntry e{};
    if (!findRes(*m->pe, 6 /* RT_STRING */, (id >> 4) + 1, 0, e)) return 0;
    const auto* p = reinterpret_cast<const uint16_t*>(e.data);
    for (UINT i = 0; i < (id & 15); ++i) p += 1 + *p;
    out.assign(reinterpret_cast<const wchar_t*>(p + 1), *p);
    return static_cast<int>(out.size());
}

// ---- version information ------------------------------------------------------------------
// System DLLs do not exist here; the game's DirectX check (ConfigDetect) reads their file
// versions, so they get a synthetic VS_VERSIONINFO of Windows XP SP3 (DirectX 9.0c).
static void verNode(std::vector<uint8_t>& out, const char* key, const void* value, uint16_t valueLen, bool text,
                    const std::function<void(std::vector<uint8_t>&)>& children) {
    auto align = [&] { while (out.size() & 3) out.push_back(0); };
    align();
    const size_t start = out.size();
    out.resize(out.size() + 6);
    for (const char* k = key;; ++k) { out.push_back(static_cast<uint8_t>(*k)); out.push_back(0); if (!*k) break; }
    align();
    if (value) {
        const auto* v = static_cast<const uint8_t*>(value);
        out.insert(out.end(), v, v + (text ? valueLen * 2 : valueLen));
    }
    if (children) children(out);
    const uint16_t len = static_cast<uint16_t>(out.size() - start);
    std::memcpy(&out[start], &len, 2);
    std::memcpy(&out[start + 2], &valueLen, 2);
    const uint16_t type = text ? 1 : 0;
    std::memcpy(&out[start + 4], &type, 2);
}
static std::vector<uint8_t> syntheticVersion(const std::string& file) {
    VS_FIXEDFILEINFO fi{};
    fi.dwSignature = VS_FFI_SIGNATURE;
    fi.dwStrucVersion = VS_FFI_STRUCVERSION;
    fi.dwFileVersionMS = fi.dwProductVersionMS = (5u << 16) | 3u;
    fi.dwFileVersionLS = fi.dwProductVersionLS = (2600u << 16) | 5512u;
    fi.dwFileOS = VOS_NT_WINDOWS32;
    fi.dwFileType = VFT_DLL;
    std::vector<uint8_t> out;
    auto text = [](const char* s) { std::vector<uint16_t> w; for (const char* p = s;; ++p) { w.push_back(static_cast<uint8_t>(*p)); if (!*p) break; } return w; };
    verNode(out, "VS_VERSION_INFO", &fi, sizeof fi, false, [&](std::vector<uint8_t>& o) {
        verNode(o, "StringFileInfo", nullptr, 0, true, [&](std::vector<uint8_t>& o2) {
            verNode(o2, "040904B0", nullptr, 0, true, [&](std::vector<uint8_t>& o3) {
                const auto fv = text("5.03.2600.5512 (xpsp.080413-0845)");
                verNode(o3, "FileVersion", fv.data(), static_cast<uint16_t>(fv.size()), true, nullptr);
                const auto pv = text("5.03.2600.5512");
                verNode(o3, "ProductVersion", pv.data(), static_cast<uint16_t>(pv.size()), true, nullptr);
                const auto on = text(file.c_str());
                verNode(o3, "OriginalFilename", on.data(), static_cast<uint16_t>(on.size()), true, nullptr);
                const auto cn = text("Microsoft Corporation");
                verNode(o3, "CompanyName", cn.data(), static_cast<uint16_t>(cn.size()), true, nullptr);
            });
        });
        verNode(o, "VarFileInfo", nullptr, 0, true, [&](std::vector<uint8_t>& o2) {
            const uint32_t tr = 0x04B00409;
            verNode(o2, "Translation", &tr, 4, false, nullptr);
        });
    });
    return out;
}
static bool versionBlock(LPCSTR file, std::vector<uint8_t>& out) {
    PeModule pe;
    pe.path = toPosixPathA(file);
    ResEntry e{};
    if (loadPe(pe) && findRes(pe, 16 /* RT_VERSION */, 1, 0, e)) {
        out.assign(e.data, e.data + e.size);
        return true;
    }
    std::string name = file;
    if (auto p = name.find_last_of("\\/"); p != std::string::npos) name = name.substr(p + 1);
    std::string lc = file;
    for (auto& ch : lc) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    if (lc.find("system32") == std::string::npos && lc.find("windows") == std::string::npos) return false;
    out = syntheticVersion(name);
    return true;
}
DWORD WINAPI GetFileVersionInfoSizeA(LPCSTR file, LPDWORD handle) {
    if (handle) *handle = 0;
    std::vector<uint8_t> b;
    if (!versionBlock(file, b)) { setError(ERROR_RESOURCE_DATA_NOT_FOUND); return 0; }
    return static_cast<DWORD>(b.size());
}
BOOL WINAPI GetFileVersionInfoA(LPCSTR file, DWORD, DWORD n, LPVOID out) {
    std::vector<uint8_t> b;
    if (!versionBlock(file, b)) return fail(ERROR_RESOURCE_DATA_NOT_FOUND);
    std::memcpy(out, b.data(), std::min<size_t>(n, b.size()));
    return TRUE;
}
// VS_VERSIONINFO walk: "\\" -> VS_FIXEDFILEINFO; "\\StringFileInfo\\<lang>\\<key>" -> string;
// "\\VarFileInfo\\Translation" -> DWORD array. Keys are UTF-16 in the block.
static const uint8_t* verChild(const uint8_t* node, const std::string& key, const uint8_t** valueOut, uint16_t* valueLen) {
    auto align = [](const uint8_t* p, const uint8_t* base) { return base + ((p - base + 3) & ~3); };
    const uint16_t len = *reinterpret_cast<const uint16_t*>(node);
    const auto* k = reinterpret_cast<const wchar_t*>(node + 6);
    const uint8_t* p = align(reinterpret_cast<const uint8_t*>(k + wlen(k) + 1), node);
    p = align(p + *reinterpret_cast<const uint16_t*>(node + 2) * (*reinterpret_cast<const uint16_t*>(node + 4) ? 2 : 1), node);
    while (p < node + len) {
        const uint16_t clen = *reinterpret_cast<const uint16_t*>(p);
        if (!clen) break;
        const auto* ck = reinterpret_cast<const wchar_t*>(p + 6);
        bool eq = wlen(ck) == key.size();
        for (size_t i = 0; eq && i < key.size(); ++i) eq = lower(ck[i]) == lower(static_cast<wchar_t>(key[i]));
        if (eq) {
            if (valueOut) {
                *valueOut = align(reinterpret_cast<const uint8_t*>(ck + wlen(ck) + 1), p);
                *valueLen = *reinterpret_cast<const uint16_t*>(p + 2);
            }
            return p;
        }
        p = align(p + clen, node);
    }
    return nullptr;
}
BOOL WINAPI VerQueryValueA(LPCVOID block, LPCSTR sub, LPVOID* out, PUINT len) {
    const auto* root = static_cast<const uint8_t*>(block);
    std::string path = sub ? sub : "\\";
    if (path == "\\") {
        const auto* k = reinterpret_cast<const wchar_t*>(root + 6);
        const uint8_t* v = root + ((reinterpret_cast<const uint8_t*>(k + wlen(k) + 1) - root + 3) & ~3);
        *out = const_cast<uint8_t*>(v);
        *len = *reinterpret_cast<const uint16_t*>(root + 2);
        return *len != 0;
    }
    const uint8_t* node = root;
    const uint8_t* val = nullptr;
    uint16_t vlen = 0;
    size_t i = 1;
    while (i <= path.size()) {
        size_t j = path.find('\\', i);
        if (j == std::string::npos) j = path.size();
        const std::string key = path.substr(i, j - i);
        node = verChild(node, key, &val, &vlen);
        if (!node) return FALSE;
        i = j + 1;
    }
    if (path.find("StringFileInfo") != std::string::npos) {
        // Narrow the UTF-16 value in place into a side buffer that lives with the block.
        static thread_local std::string narrow;
        narrow = utf8ToAcp(toUtf8(reinterpret_cast<const wchar_t*>(val)).c_str());
        *out = narrow.data();
        *len = static_cast<UINT>(narrow.size());
    } else {
        *out = const_cast<uint8_t*>(val);
        *len = vlen;
    }
    return TRUE;
}

// ---- exceptions -----------------------------------------------------------------------------
PVOID WINAPI AddVectoredExceptionHandler(ULONG first, PVECTORED_EXCEPTION_HANDLER h) {
    static std::once_flag once;
    std::call_once(once, [] {
        // Signal handlers run on an alternate stack per thread where one is installed; the
        // handler itself only logs.
        struct sigaction sa{};
        sa.sa_sigaction = onSignal;
        sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
        sigemptyset(&sa.sa_mask);
        for (int s : {SIGSEGV, SIGBUS, SIGILL, SIGFPE}) sigaction(s, &sa, nullptr);
    });
    std::lock_guard<std::mutex> l(g_vehLock);
    if (first) g_veh.insert(g_veh.begin(), h);
    else g_veh.push_back(h);
    return reinterpret_cast<void*>(h);
}
ULONG WINAPI RemoveVectoredExceptionHandler(PVOID h) {
    std::lock_guard<std::mutex> l(g_vehLock);
    for (auto it = g_veh.begin(); it != g_veh.end(); ++it)
        if (reinterpret_cast<void*>(*it) == h) { g_veh.erase(it); return 1; }
    return 0;
}

// ---- error messages -------------------------------------------------------------------------
DWORD WINAPI FormatMessageW(DWORD flags, LPCVOID src, DWORD id, DWORD, LPWSTR buf, DWORD n, va_list*) {
    wstr msg;
    if (flags & FORMAT_MESSAGE_FROM_STRING) msg = static_cast<const wchar_t*>(src);
    else {
        char tmp[64];
        std::snprintf(tmp, sizeof tmp, "Error %lu.\r\n", static_cast<unsigned long>(id));
        msg = fromUtf8(tmp);
    }
    if (flags & FORMAT_MESSAGE_ALLOCATE_BUFFER) {
        auto* p = static_cast<wchar_t*>(LocalAlloc(LMEM_FIXED, (msg.size() + 1) * sizeof(wchar_t)));
        std::memcpy(p, msg.c_str(), (msg.size() + 1) * sizeof(wchar_t));
        *reinterpret_cast<LPWSTR*>(buf) = p;
        return static_cast<DWORD>(msg.size());
    }
    if (msg.size() + 1 > n) { setError(ERROR_INSUFFICIENT_BUFFER); return 0; }
    std::memcpy(buf, msg.c_str(), (msg.size() + 1) * sizeof(wchar_t));
    return static_cast<DWORD>(msg.size());
}
HLOCAL WINAPI LocalAlloc(UINT flags, SIZE_T n) {
    void* p = std::malloc(n ? n : 1);
    if (p && (flags & LMEM_ZEROINIT)) std::memset(p, 0, n);
    return p;
}
HLOCAL WINAPI LocalFree(HLOCAL p) { std::free(p); return nullptr; }

// ---- code pages, strings, locale ----------------------------------------------------------
UINT WINAPI GetACP(void) { return 1252; }
UINT WINAPI GetOEMCP(void) { return 437; }
BOOL WINAPI GetCPInfo(UINT, LPCPINFO info) {
    std::memset(info, 0, sizeof *info);
    info->MaxCharSize = 1;
    info->DefaultChar[0] = '?';
    return TRUE;
}
BOOL WINAPI GetCPInfoExA(UINT cp, DWORD, LPCPINFOEXA info) {
    std::memset(info, 0, sizeof *info);
    info->MaxCharSize = 1;
    info->DefaultChar[0] = '?';
    info->UnicodeDefaultChar = L'?';
    info->CodePage = cp == CP_ACP ? 1252 : cp;
    std::strcpy(info->CodePageName, "1252  (ANSI - Latin I)");
    return TRUE;
}
BOOL WINAPI IsDBCSLeadByteEx(UINT, BYTE) { return FALSE; }
BOOL WINAPI IsDBCSLeadByte(BYTE) { return FALSE; }

int WINAPI MultiByteToWideChar(UINT cp, DWORD, LPCCH src, int n, LPWSTR dst, int cap) {
    const size_t len = n < 0 ? std::strlen(src) + 1 : static_cast<size_t>(n);
    const wstr w = cp == CP_UTF8 ? fromUtf8(src, len) : fromUtf8(acpToUtf8(src, len).c_str(), acpToUtf8(src, len).size());
    if (!cap) return static_cast<int>(w.size());
    if (w.size() > static_cast<size_t>(cap)) { setError(ERROR_INSUFFICIENT_BUFFER); return 0; }
    std::memcpy(dst, w.data(), w.size() * sizeof(wchar_t));
    return static_cast<int>(w.size());
}
int WINAPI WideCharToMultiByte(UINT cp, DWORD, LPCWCH src, int n, LPSTR dst, int cap, LPCCH, LPBOOL usedDefault) {
    const size_t len = n < 0 ? wlen(src) + 1 : static_cast<size_t>(n);
    std::string s = toUtf8(src, len);
    if (cp != CP_UTF8) {
        // toUtf8 stops nothing at embedded NULs; convert piecewise to keep them
        std::string a;
        size_t start = 0;
        for (size_t i = 0; i <= s.size(); ++i)
            if (i == s.size() || s[i] == '\0') {
                a += utf8ToAcp(s.substr(start, i - start).c_str());
                if (i < s.size()) a += '\0';
                start = i + 1;
            }
        s = a;
    }
    if (usedDefault) *usedDefault = FALSE;
    if (!cap) return static_cast<int>(s.size());
    if (s.size() > static_cast<size_t>(cap)) { setError(ERROR_INSUFFICIENT_BUFFER); return 0; }
    std::memcpy(dst, s.data(), s.size());
    return static_cast<int>(s.size());
}
int WINAPI lstrlenA(LPCSTR s) { return s ? static_cast<int>(std::strlen(s)) : 0; }
int WINAPI lstrlenW(LPCWSTR s) { return static_cast<int>(wlen(s)); }
LPSTR WINAPI lstrcpyA(LPSTR d, LPCSTR s) { return std::strcpy(d, s); }
LPSTR WINAPI lstrcatA(LPSTR d, LPCSTR s) { return std::strcat(d, s); }
LPWSTR WINAPI lstrcpyW(LPWSTR d, LPCWSTR s) { size_t i = 0; do d[i] = s[i]; while (s[i++]); return d; }
LPWSTR WINAPI lstrcpynW(LPWSTR d, LPCWSTR s, int n) {
    if (n <= 0) return d;
    int i = 0;
    for (; i < n - 1 && s[i]; ++i) d[i] = s[i];
    d[i] = 0;
    return d;
}
LPWSTR WINAPI lstrcatW(LPWSTR d, LPCWSTR s) { lstrcpyW(d + wlen(d), s); return d; }
int WINAPI lstrcmpW(LPCWSTR a, LPCWSTR b) {
    for (;; ++a, ++b) {
        if (*a != *b) return static_cast<uint16_t>(*a) < static_cast<uint16_t>(*b) ? -1 : 1;
        if (!*a) return 0;
    }
}
int WINAPI lstrcmpiW(LPCWSTR a, LPCWSTR b) {
    for (;; ++a, ++b) {
        const wchar_t x = lower(*a), y = lower(*b);
        if (x != y) return static_cast<uint16_t>(x) < static_cast<uint16_t>(y) ? -1 : 1;
        if (!x) return 0;
    }
}
int WINAPI lstrcmpiA(LPCSTR a, LPCSTR b) { return strcasecmp(a, b) < 0 ? -1 : strcasecmp(a, b) > 0 ? 1 : 0; }
int WINAPI lstrcmpA(LPCSTR a, LPCSTR b) { const int r = std::strcmp(a, b); return r < 0 ? -1 : r > 0; }
int WINAPI MulDiv(int a, int b, int c) {
    if (!c) return -1;
    const int64_t r = static_cast<int64_t>(a) * b;
    return static_cast<int>((r >= 0) == (c > 0) ? (r + c / 2) / c : (r - c / 2) / c);
}
int WINAPI CompareStringA(LCID, DWORD flags, LPCSTR a, int na, LPCSTR b, int nb) {
    const std::string x(a, na < 0 ? std::strlen(a) : static_cast<size_t>(na)), y(b, nb < 0 ? std::strlen(b) : static_cast<size_t>(nb));
    int r;
    if (flags & NORM_IGNORECASE) r = strcasecmp(x.c_str(), y.c_str());
    else r = std::strcmp(x.c_str(), y.c_str());
    return r < 0 ? CSTR_LESS_THAN : r > 0 ? CSTR_GREATER_THAN : CSTR_EQUAL;
}
int WINAPI LCMapStringW(LCID, DWORD flags, LPCWSTR src, int n, LPWSTR dst, int cap) {
    const size_t len = n < 0 ? wlen(src) + 1 : static_cast<size_t>(n);
    if (!cap) return static_cast<int>(len);
    if (len > static_cast<size_t>(cap)) { setError(ERROR_INSUFFICIENT_BUFFER); return 0; }
    for (size_t i = 0; i < len; ++i) dst[i] = (flags & LCMAP_UPPERCASE) ? upper(src[i]) : (flags & LCMAP_LOWERCASE) ? lower(src[i]) : src[i];
    return static_cast<int>(len);
}
int WINAPI LCMapStringA(LCID, DWORD flags, LPCSTR src, int n, LPSTR dst, int cap) {
    const size_t len = n < 0 ? std::strlen(src) + 1 : static_cast<size_t>(n);
    if (!cap) return static_cast<int>(len);
    if (len > static_cast<size_t>(cap)) { setError(ERROR_INSUFFICIENT_BUFFER); return 0; }
    for (size_t i = 0; i < len; ++i)
        dst[i] = (flags & LCMAP_UPPERCASE) ? static_cast<char>(std::toupper(static_cast<unsigned char>(src[i])))
               : (flags & LCMAP_LOWERCASE) ? static_cast<char>(std::tolower(static_cast<unsigned char>(src[i])))
                                           : src[i];
    return static_cast<int>(len);
}
static WORD ctype1(uint32_t c) {
    WORD t = 0;
    if (c >= 'A' && c <= 'Z') t |= C1_UPPER | C1_ALPHA;
    if (c >= 'a' && c <= 'z') t |= C1_LOWER | C1_ALPHA;
    if (c >= '0' && c <= '9') t |= C1_DIGIT;
    if (c == ' ' || (c >= 9 && c <= 13)) t |= C1_SPACE;
    if (c == ' ' || c == '\t') t |= C1_BLANK;
    if ((c >= '!' && c <= '/') || (c >= ':' && c <= '@') || (c >= '[' && c <= '`') || (c >= '{' && c <= '~')) t |= C1_PUNCT;
    if (c < 32 || c == 127) t |= C1_CNTRL;
    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) t |= C1_XDIGIT;
    if (c >= 0xC0 && c <= 0x24F && c != 0xD7 && c != 0xF7) t |= C1_ALPHA | (lower(static_cast<wchar_t>(c)) == static_cast<wchar_t>(c) ? C1_LOWER : C1_UPPER);
    if (t & (C1_ALPHA | C1_DIGIT | C1_PUNCT)) t |= C1_DEFINED;
    return t ? t : C1_DEFINED;
}
BOOL WINAPI GetStringTypeW(DWORD type, LPCWCH src, int n, LPWORD out) {
    const size_t len = n < 0 ? wlen(src) + 1 : static_cast<size_t>(n);
    for (size_t i = 0; i < len; ++i) out[i] = type == CT_CTYPE1 ? ctype1(static_cast<uint16_t>(src[i])) : 0;
    return TRUE;
}
BOOL WINAPI GetStringTypeA(LCID, DWORD type, LPCSTR src, int n, LPWORD out) {
    const size_t len = n < 0 ? std::strlen(src) + 1 : static_cast<size_t>(n);
    for (size_t i = 0; i < len; ++i) out[i] = type == CT_CTYPE1 ? ctype1(static_cast<unsigned char>(src[i])) : 0;
    return TRUE;
}
LCID WINAPI GetUserDefaultLCID(void) { return 0x0409; }
LCID WINAPI GetSystemDefaultLCID(void) { return 0x0409; }
LANGID WINAPI GetSystemDefaultLangID(void) { return 0x0409; }
LANGID WINAPI GetUserDefaultLangID(void) { return 0x0409; }
LANGID WINAPI GetUserDefaultUILanguage(void) { return 0x0409; }
int WINAPI GetLocaleInfoA(LCID, LCTYPE type, LPSTR out, int n) {
    const char* v;
    switch (type & 0xFFFF) {
        case LOCALE_IDEFAULTANSICODEPAGE: v = "1252"; break;
        case LOCALE_IDEFAULTCODEPAGE: v = "437"; break;
        case LOCALE_SENGLANGUAGE: v = "English"; break;
        case LOCALE_SABBREVLANGNAME: v = "ENU"; break;
        case LOCALE_SENGCOUNTRY: v = "United States"; break;
        case LOCALE_SABBREVCTRYNAME: v = "USA"; break;
        case LOCALE_ILANGUAGE: v = "0409"; break;
        case LOCALE_SDECIMAL: v = "."; break;
        case LOCALE_STHOUSAND: v = ","; break;
        case LOCALE_SISO639LANGNAME: v = "en"; break;
        case LOCALE_SISO3166CTRYNAME: v = "US"; break;
        default: v = ""; break;
    }
    const int len = static_cast<int>(std::strlen(v)) + 1;
    if (!n) return len;
    if (n < len) { setError(ERROR_INSUFFICIENT_BUFFER); return 0; }
    std::memcpy(out, v, static_cast<size_t>(len));
    return len;
}

}  // extern "C"

namespace w32 {
uint64_t monotonicNs() {
    timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return static_cast<uint64_t>(t.tv_sec) * 1000000000ull + static_cast<uint64_t>(t.tv_nsec);
}
uint64_t nowFileTime() {
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (static_cast<uint64_t>(t.tv_sec) + 11644473600ull) * 10000000ull + static_cast<uint64_t>(t.tv_nsec) / 100;
}
}  // namespace w32
