// Code layout support for the Linux builds (CMake: FABLE_SYMBOL_ORDER, FABLE_HUGE_TEXT, FABLE_PGO;
// rg34xx/linux-a64.cmake turns the first two on for the handheld).
//
// * fable_hot_text_begin / fable_hot_text_end bracket the hot code: the ordering file
//   (rg34xx/hot_symbols.txt, written by rg34xx/gen_hot_order.py) lists them first and last, so
//   lld places every hot lifted function between them.
// * fableCodeLayoutInit() runs first thing in main(), before any other thread exists. With
//   FABLE_HUGE_TEXT it copies the hot range, widened to whole 2 MiB pages, into anonymous memory
//   backed by transparent huge pages and moves that over the file-backed text: the hot code then
//   needs a handful of TLB entries instead of one per 4 KiB page (the A53 has 10 L1 iTLB entries
//   and 512 main TLB entries shared with data). It does nothing unless the kernel offers THP
//   (enabled = always or madvise), the ordering put the markers at least 1 MiB apart and huge
//   pages were actually obtained. A failure before the move leaves the original mapping alone; a
//   failed move (mremap unmaps its destination first) is repaired from the staging copy, and the
//   process aborts if even that fails. The bytes do not change, so return addresses, function
//   pointers, dladdr, the sampling profiler and the unwinder are unaffected; only tools that
//   symbolise through /proc/<pid>/maps (perf) see the range as anonymous memory.
//   Environment: FABLE_HUGE_TEXT=0 off, =1 on (also in builds without the option), =2 moves the
//   range whatever the kernel's THP setting and whether or not huge pages were obtained (a test
//   of the move itself); FABLE_HUGE_TEXT_MB caps the size (default 16).
//   With FABLE_PGO=gen (the instrumented training build) it starts a thread that writes the
//   profile every 30 s, since the harness ends the game with a signal, which skips the exit-time
//   write. The runtime appends each write to the file it truncated at start, so the thread
//   resets the counters after each: the file holds the deltas, which llvm-profdata merge sums
//   to the session's counts. An atexit handler stops the thread before the runtime's own write.
// * FABLE_PMU=<seconds> (opt-in): hardware counters of the busiest thread (the game thread when
//   it is the bottleneck) in the log every <seconds>: IPC, instruction-cache and branch misses
//   and, on the Cortex-A53, the share of cycles the core starves on an instruction-cache or
//   micro-TLB miss (or stalls on a load miss): the evidence for (or against) the code layout
//   work above. fablePmuProbe() (--pmu-probe, run by Fable-diag.sh) tells whether they work.
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <mutex>
#include <system_error>
#include <thread>

#include <dirent.h>
#include <link.h>
#include <linux/perf_event.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

extern "C" {
// Own sections, so the ordering file can place them; noinline + used keep them distinct.
__attribute__((section(".text.fable_hot_text_begin"), used, noinline)) void fable_hot_text_begin(void) {}
__attribute__((section(".text.fable_hot_text_end"), used, noinline)) void fable_hot_text_end(void) {}
#if defined(FABLE_PGO_GEN)
int __llvm_profile_write_file(void);
void __llvm_profile_reset_counters(void);
#endif
}

namespace host { void log(const char* fmt, ...); }

namespace {

constexpr uintptr_t kHuge = uintptr_t(2) << 20;  // PMD size with 4 KiB pages (arm64 and x86-64)

struct Segment { uintptr_t lo = 0, hi = 0; };

// The executable PT_LOAD of the main program that holds the hot range.
int findSegment(dl_phdr_info* info, size_t, void* arg) {
    auto* seg = static_cast<Segment*>(arg);
    const uintptr_t want = reinterpret_cast<uintptr_t>(&fable_hot_text_begin);
    for (int i = 0; i < info->dlpi_phnum; ++i) {
        const ElfW(Phdr)& ph = info->dlpi_phdr[i];
        if (ph.p_type != PT_LOAD || !(ph.p_flags & PF_X)) continue;
        const uintptr_t lo = info->dlpi_addr + ph.p_vaddr, hi = lo + ph.p_filesz;
        if (want >= lo && want < hi) {
            seg->lo = lo;
            seg->hi = hi;
            return 1;
        }
    }
    return 0;
}

// The selected mode in /sys/kernel/mm/transparent_hugepage/enabled ("always [madvise] never").
bool thpMode(char* out, size_t n) {
    out[0] = 0;
    FILE* f = std::fopen("/sys/kernel/mm/transparent_hugepage/enabled", "r");
    if (!f) return false;
    char line[128] = {};
    const bool ok = std::fgets(line, sizeof line, f) != nullptr;
    std::fclose(f);
    const char* a = ok ? std::strchr(line, '[') : nullptr;
    const char* b = a ? std::strchr(a, ']') : nullptr;
    if (!a || !b || size_t(b - a) >= n) return false;
    std::memcpy(out, a + 1, size_t(b - a - 1));
    out[b - a - 1] = 0;
    return true;
}

// AnonHugePages of the mapping that starts at `start` (/proc/self/smaps), in KiB; -1 if unknown.
long anonHugeKb(uintptr_t start) {
    FILE* f = std::fopen("/proc/self/smaps", "r");
    if (!f) return -1;
    char line[512];
    bool in = false;
    long kb = -1;
    while (std::fgets(line, sizeof line, f)) {
        unsigned long lo = 0, hi = 0;
        char c = 0;
        if (std::sscanf(line, "%lx-%lx%c", &lo, &hi, &c) == 3 && c == ' ') {
            if (in) break;
            in = lo == start;
            continue;
        }
        if (in && std::sscanf(line, "AnonHugePages: %ld kB", &kb) == 1) break;
    }
    std::fclose(f);
    return kb;
}

// Whether every page of [p, p + len) is mapped (msync reports ENOMEM for a hole).
bool mapped(uintptr_t p, size_t len) {
    return msync(reinterpret_cast<void*>(p), len, MS_ASYNC) == 0 || errno != ENOMEM;
}

__attribute__((noinline)) void hugeText() {
    const char* env = std::getenv("FABLE_HUGE_TEXT");
#if defined(FABLE_HUGE_TEXT)
    if (env && std::atoi(env) == 0) return;
#else
    if (!env || std::atoi(env) == 0) return;
#endif
    const bool force = env && std::atoi(env) == 2;
    const uintptr_t begin = reinterpret_cast<uintptr_t>(&fable_hot_text_begin);
    const uintptr_t end = reinterpret_cast<uintptr_t>(&fable_hot_text_end);
    char mode[32];
    const bool thp = thpMode(mode, sizeof mode) && std::strcmp(mode, "never") != 0;
    if (!thp && !force) {
        std::fprintf(stderr, "huge text: off (transparent huge pages: %s)\n", mode[0] ? mode : "not in this kernel");
        return;
    }
    if (end < begin + kHuge / 2) {
        std::fprintf(stderr, "huge text: off (hot range %lu KiB: this build has no ordering file)\n",
                     static_cast<unsigned long>(end > begin ? (end - begin) >> 10 : 0));
        return;
    }
    Segment seg;
    if (!dl_iterate_phdr(findSegment, &seg) || end > seg.hi) {
        std::fprintf(stderr, "huge text: off (text segment not found)\n");
        return;
    }
    // Whole huge pages around the hot range, inside the executable segment (lld's --no-rosegment
    // keeps the read-only data ahead of .text in it, so the lower bound stays inside).
    uintptr_t lo = begin & ~(kHuge - 1), hi = (end + kHuge - 1) & ~(kHuge - 1);
    if (lo < seg.lo) lo = (seg.lo + kHuge - 1) & ~(kHuge - 1);
    if (hi > (seg.hi & ~(kHuge - 1))) hi = seg.hi & ~(kHuge - 1);
    const char* capEnv = std::getenv("FABLE_HUGE_TEXT_MB");
    const uintptr_t cap = (uintptr_t(capEnv && std::atoi(capEnv) > 0 ? std::atoi(capEnv) : 16) << 20) & ~(kHuge - 1);
    if (hi > lo + cap) hi = lo + cap;
    // the code running now (this function, its caller) must not move under itself
    const uintptr_t self = reinterpret_cast<uintptr_t>(&hugeText);
    const uintptr_t ret = reinterpret_cast<uintptr_t>(__builtin_return_address(0));
    if (hi <= lo || (self >= lo && self < hi) || (ret >= lo && ret < hi)) {
        std::fprintf(stderr, "huge text: off (no whole 2 MiB page in the hot range)\n");
        return;
    }
    const size_t len = hi - lo;
    // a 2 MiB-aligned staging area, filled through huge-page faults, then moved over the text
    void* raw = mmap(nullptr, len + kHuge, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (raw == MAP_FAILED) {
        std::fprintf(stderr, "huge text: off (mmap: %s)\n", std::strerror(errno));
        return;
    }
    const uintptr_t r = reinterpret_cast<uintptr_t>(raw), tmp = (r + kHuge - 1) & ~(kHuge - 1);
    if (tmp > r) munmap(raw, tmp - r);
    if (r + len + kHuge > tmp + len) munmap(reinterpret_cast<void*>(tmp + len), r + len + kHuge - (tmp + len));
    void* stage = reinterpret_cast<void*>(tmp);
    if (thp) madvise(stage, len, MADV_HUGEPAGE);
    std::memcpy(stage, reinterpret_cast<const void*>(lo), len);
    const long stagedKb = thp ? anonHugeKb(tmp) : 0;
    if (stagedKb <= 0 && !force) {  // none to be had (fragmentation, defrag policy) or unknown: keep the file's pages
        munmap(stage, len);
        std::fprintf(stderr, "huge text: off (THP %s, but no huge pages were allocated)\n", mode);
        return;
    }
    if (mprotect(stage, len, PROT_READ | PROT_EXEC) != 0) {
        std::fprintf(stderr, "huge text: off (mprotect: %s)\n", std::strerror(errno));
        munmap(stage, len);
        return;
    }
    __builtin___clear_cache(static_cast<char*>(stage), static_cast<char*>(stage) + len);
    if (mremap(stage, len, len, MREMAP_MAYMOVE | MREMAP_FIXED, reinterpret_cast<void*>(lo)) == MAP_FAILED) {
        const int err = errno;
        // mremap unmaps the destination before it moves anything: if the move then failed, the
        // hot code may be gone. Put the staged copy (identical bytes) back in its place.
        if (!mapped(lo, len)) {
            if (!mapped(tmp, len) ||
                mmap(reinterpret_cast<void*>(lo), len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0) == MAP_FAILED) {
                std::fprintf(stderr, "huge text: FATAL: mremap failed (%s) and took the code at %#lx with it\n", std::strerror(err),
                             static_cast<unsigned long>(lo));
                std::abort();
            }
            std::memcpy(reinterpret_cast<void*>(lo), stage, len);
            if (mprotect(reinterpret_cast<void*>(lo), len, PROT_READ | PROT_EXEC) != 0) {
                std::fprintf(stderr, "huge text: FATAL: mremap failed (%s), restoring the code at %#lx failed (mprotect: %s)\n",
                             std::strerror(err), static_cast<unsigned long>(lo), std::strerror(errno));
                std::abort();
            }
            __builtin___clear_cache(reinterpret_cast<char*>(lo), reinterpret_cast<char*>(hi));
            std::fprintf(stderr, "huge text: off (mremap: %s; the code was restored from the staged copy)\n", std::strerror(err));
        } else {
            std::fprintf(stderr, "huge text: off (mremap: %s)\n", std::strerror(err));
        }
        munmap(stage, len);
        return;
    }
    __builtin___clear_cache(reinterpret_cast<char*>(lo), reinterpret_cast<char*>(hi));
    const long hugeKb = anonHugeKb(lo);
    if (hugeKb > 0)
        std::fprintf(stderr, "huge text: %lu KiB of code at %#lx moved onto huge pages (%ld KiB huge; hot range %lu KiB; THP %s)\n",
                     static_cast<unsigned long>(len >> 10), static_cast<unsigned long>(lo), hugeKb,
                     static_cast<unsigned long>((end - begin) >> 10), mode[0] ? mode : "none");
    else
        std::fprintf(stderr, "huge text: %lu KiB of code at %#lx moved to anonymous memory without huge pages (FABLE_HUGE_TEXT=2 test; THP %s)\n",
                     static_cast<unsigned long>(len >> 10), static_cast<unsigned long>(lo), mode[0] ? mode : "none");
}

// ---- hardware counters ----------------------------------------------------------------------
// Attached to the busiest thread of the last interval (the game thread when the game is the
// bottleneck): the guest's threads have no names to find them by.
struct Counter {
    const char* name = nullptr;
    int fd = -1;
    uint64_t value = 0, enabled = 0, running = 0;  // at the last read
};
Counter g_counters[8];
int g_numCounters = 0;
bool g_a53 = false;

unsigned cpuPart() {  // MIDR part number of the first CPU (0xd03 = Cortex-A53), 0 if unknown
    FILE* f = std::fopen("/proc/cpuinfo", "r");
    if (!f) return 0;
    char line[256];
    unsigned part = 0;
    while (std::fgets(line, sizeof line, f))
        if (std::sscanf(line, "CPU part : %x", &part) == 1) break;
    std::fclose(f);
    return part;
}

void closeCounters() {
    for (int i = 0; i < g_numCounters; ++i) close(g_counters[i].fd);
    g_numCounters = 0;
}

// 0 when the counter is open, else perf_event_open's errno.
int addCounter(pid_t tid, const char* name, uint32_t type, uint64_t config) {
    if (g_numCounters == 8) return EMFILE;
    perf_event_attr a{};
    a.size = sizeof a;
    a.type = type;
    a.config = config;
    a.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED | PERF_FORMAT_TOTAL_TIME_RUNNING;
    a.exclude_hv = 1;
    long fd = syscall(SYS_perf_event_open, &a, tid, -1, -1, PERF_FLAG_FD_CLOEXEC);
    if (fd < 0 && (errno == EACCES || errno == EPERM)) {
        a.exclude_kernel = 1;  // perf_event_paranoid may allow user space only
        fd = syscall(SYS_perf_event_open, &a, tid, -1, -1, PERF_FLAG_FD_CLOEXEC);
    }
    if (fd < 0) return errno;
    Counter& c = g_counters[g_numCounters++];
    c = Counter{};
    c.name = name;
    c.fd = static_cast<int>(fd);
    return 0;
}

// Opens the counters on `tid` (tid 0: the calling thread). Cycles and instructions are required
// (their errno is returned, with nothing left open); the rest are added where the core has them.
int attachCounters(pid_t tid) {
    closeCounters();
    int err = addCounter(tid, "cycles", PERF_TYPE_HARDWARE, PERF_COUNT_HW_CPU_CYCLES);
    if (!err) err = addCounter(tid, "instructions", PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS);
    if (err) {
        closeCounters();
        return err;
    }
    addCounter(tid, "branch-misses", PERF_TYPE_HARDWARE, PERF_COUNT_HW_BRANCH_MISSES);
    addCounter(tid, "L1I-misses", PERF_TYPE_HW_CACHE,
               PERF_COUNT_HW_CACHE_L1I | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16));
    if (g_a53) {  // Cortex-A53 TRM PMU events: cycles the issue queue is empty ...
        addCounter(tid, "icache-stall", PERF_TYPE_RAW, 0xE1);  // ... on an instruction-cache miss
        addCounter(tid, "itlb-stall", PERF_TYPE_RAW, 0xE2);    // ... on an instruction micro-TLB miss
        addCounter(tid, "load-stall", PERF_TYPE_RAW, 0xE7);    // Wr stage stalled on a load miss
    } else {
        addCounter(tid, "iTLB-misses", PERF_TYPE_HW_CACHE,
                   PERF_COUNT_HW_CACHE_ITLB | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16));
    }
    return 0;
}

// No point retrying: the kernel or the hardware has no such counters, or we may not use them.
bool permanent(int err) { return err == ENOENT || err == EOPNOTSUPP || err == EACCES || err == EPERM || err == ENODEV; }

// Counts since the last call, scaled up when the kernel had to multiplex the counters.
void readDeltas(double* out) {
    for (int i = 0; i < g_numCounters; ++i) {
        Counter& c = g_counters[i];
        uint64_t v[3] = {};
        out[i] = 0;
        if (read(c.fd, v, sizeof v) != sizeof v) continue;
        const uint64_t dv = v[0] - c.value, de = v[1] - c.enabled, dr = v[2] - c.running;
        c.value = v[0], c.enabled = v[1], c.running = v[2];
        out[i] = dr ? double(dv) * double(de) / double(dr) : 0;
    }
}

double find(const double* d, const char* name) {
    for (int i = 0; i < g_numCounters; ++i)
        if (std::strcmp(g_counters[i].name, name) == 0) return d[i];
    return -1;
}

// The thread with the most CPU time (utime + stime) since the last call; *share: how much of one
// core thread `of` used in that time.
pid_t busiestThread(double seconds, pid_t of, double* share) {
    static std::map<pid_t, unsigned long long> last;
    std::map<pid_t, unsigned long long> now;
    pid_t best = 0;
    unsigned long long bestDelta = 0, ofDelta = 0;
    if (DIR* d = opendir("/proc/self/task")) {
        while (dirent* e = readdir(d)) {
            const pid_t tid = static_cast<pid_t>(std::atoi(e->d_name));
            if (tid <= 0) continue;
            char path[64], buf[1024];
            std::snprintf(path, sizeof path, "/proc/self/task/%d/stat", tid);
            FILE* f = std::fopen(path, "r");
            if (!f) continue;
            const size_t n = std::fread(buf, 1, sizeof buf - 1, f);
            std::fclose(f);
            buf[n] = 0;
            const char* p = std::strrchr(buf, ')');  // the name may hold spaces; fields 3.. follow it
            unsigned long long ut = 0, st = 0;
            if (!p || std::sscanf(p + 2, "%*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %llu %llu", &ut, &st) != 2) continue;
            now[tid] = ut + st;
            const auto it = last.find(tid);
            const unsigned long long delta = ut + st - (it == last.end() ? 0 : it->second);
            if (delta > bestDelta) bestDelta = delta, best = tid;
            if (tid == of) ofDelta = delta;
        }
        closedir(d);
    }
    last.swap(now);
    *share = seconds > 0 ? double(ofDelta) / double(sysconf(_SC_CLK_TCK)) / seconds : 0;
    return best;
}

// "IPC ..., L1I misses/1k instr ..." for the deltas in d (empty if cycles or instructions are 0).
int formatCounters(char* line, size_t size, const double* d) {
    const double cyc = find(d, "cycles"), ins = find(d, "instructions");
    if (cyc <= 0 || ins <= 0) return 0;
    int n = std::snprintf(line, size, "%.0f M cycles, IPC %.2f", cyc / 1e6, ins / cyc);
    const struct { const char* name; const char* text; bool perCycle; } shown[] = {
        {"L1I-misses", "L1I misses/1k instr", false}, {"iTLB-misses", "iTLB misses/1k instr", false},
        {"branch-misses", "branch misses/1k instr", false}, {"icache-stall", "I-cache miss stall", true},
        {"itlb-stall", "I-TLB miss stall", true}, {"load-stall", "load miss stall", true},
    };
    for (const auto& x : shown) {
        const double v = find(d, x.name);
        if (v < 0 || n >= int(size)) continue;
        n += x.perCycle ? std::snprintf(line + n, size - n, ", %s %.1f%% of cycles", x.text, 100 * v / cyc)
                        : std::snprintf(line + n, size - n, ", %s %.2f", x.text, 1000 * v / ins);
    }
    return n;
}

void pmuLoop(int seconds) {
    double share = 0;
    sleep(10);  // past the start-up; then measure the busiest thread of each interval
    pid_t tid = busiestThread(10, 0, &share);
    for (int failures = 0;;) {
        if (const int err = attachCounters(tid)) {
            if (permanent(err) || ++failures > 5) {
                host::log("pmu: no hardware counters for thread %d (perf_event_open: %s); FABLE_PMU off", tid, std::strerror(err));
                return;
            }
            // ESRCH: the thread ended between the scan and the attach; others may pass: try again
            sleep(err == ESRCH ? 1 : static_cast<unsigned>(seconds));
            tid = busiestThread(err == ESRCH ? 1 : seconds, 0, &share);
            continue;
        }
        failures = 0;
        double d[8];
        readDeltas(d);
        sleep(static_cast<unsigned>(seconds));
        readDeltas(d);
        const pid_t measured = tid;
        tid = busiestThread(seconds, measured, &share);
        char line[320];
        if (formatCounters(line, sizeof line, d))
            host::log("pmu: thread %d (%.0f%% of a core): %s", measured, 100 * share, line);
    }
}

void pmuStart() {
    const char* e = std::getenv("FABLE_PMU");
    if (!e || std::atoi(e) <= 0) return;
    const int seconds = std::atoi(e) > 1 ? std::atoi(e) : 60;
    g_a53 = cpuPart() == 0xd03;
    try {
        std::thread(pmuLoop, seconds).detach();
    } catch (const std::system_error& x) {
        std::fprintf(stderr, "pmu: off (no thread: %s)\n", x.what());
    }
}

#if defined(FABLE_PGO_GEN)
// Each write appends the counts since the last one (then zeroes them). It must not overlap the
// runtime's own exit-time write: an atexit handler registered after the runtime's (so it runs
// before it) waits out a write in progress and stops the thread.
std::mutex g_pgoLock;
bool g_pgoStop = false;

void pgoWriterStart() {
    std::atexit([] {
        std::lock_guard<std::mutex> lock(g_pgoLock);
        g_pgoStop = true;
    });
    try {
        std::thread([] {
            for (;;) {
                sleep(30);
                std::lock_guard<std::mutex> lock(g_pgoLock);
                if (g_pgoStop) return;
                if (__llvm_profile_write_file() == 0) __llvm_profile_reset_counters();
            }
        }).detach();
    } catch (const std::system_error& x) {
        std::fprintf(stderr, "pgo: no periodic profile writes (no thread: %s)\n", x.what());
    }
}
#endif

}  // namespace

void fableCodeLayoutInit() {
    hugeText();
    pmuStart();
#if defined(FABLE_PGO_GEN)
    pgoWriterStart();
#endif
}

// --pmu-probe (Fable-diag.sh): counts a short busy loop on this thread and prints the result.
int fablePmuProbe() {
    g_a53 = cpuPart() == 0xd03;
    if (const unsigned part = cpuPart()) std::printf("pmu probe: CPU part %#x%s\n", part, g_a53 ? " (Cortex-A53)" : "");
    if (const int err = attachCounters(0)) {
        std::printf("pmu probe: no hardware counters (perf_event_open: %s)\n", std::strerror(err));
        return 1;
    }
    double d[8];
    readDeltas(d);
    volatile uint64_t x = 0;
    for (uint64_t i = 0; i < 200000000; ++i) x = x + (i ^ (x >> 3));
    readDeltas(d);
    char line[320];
    const bool ok = formatCounters(line, sizeof line, d) > 0;
    std::printf("pmu probe: %d counters open; %s\n", g_numCounters, ok ? line : "they counted nothing");
    closeCounters();
    return ok ? 0 : 1;
}
