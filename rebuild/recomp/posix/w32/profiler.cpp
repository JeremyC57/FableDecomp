// A sampling profiler for the handheld builds (FABLE_PROFILE=1, or the report interval in
// seconds): SIGPROF at 1 kHz of process CPU time records the interrupted program counter, the
// link register, the thread and, in library code (which keeps frame records), the return
// address of the library function's frame. A thread resolves the samples every report interval
// and logs
//   - the hottest entries: lifted guest functions through the recompiled tables; the host code
//     of the stripped binary through FableRecomp.syms (package_rg34xx.sh writes it next to the
//     binary from the unstripped build), else as FableRecomp!sub_<offset> of the function's
//     start in the unwind tables; libraries through their dynamic symbols or unwind tables,
//   - each thread's share, its CPU use and context switches (/proc/self/task) and its hottest
//     entries, and how busy each core was at which clock (/proc/stat, cpufreq),
//   - which code of the binary the time in libraries (locks, the clock, the GL driver) is for.
// Costs about 1% of the CPU.
#include "w32.hpp"
#include "recomp_clock.h"

#include <dirent.h>
#include <dlfcn.h>
#include <elf.h>
#include <link.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <ucontext.h>
#include <unistd.h>

#include <cxxabi.h>

#include <algorithm>
#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// the runtime and COM entry points named in the profile (file scope: see the markers below)
struct Ctx;
namespace host::com { void comAddRef(Ctx* c); void comRelease(Ctx* c); void comQueryInterface(Ctx* c); }
typedef void (*GuestFnPtr)(Ctx*);
extern "C" GuestFnPtr recomp_lookup(uint32_t target);
extern "C" void recomp_dispatch(Ctx* c, uint32_t target);
extern "C" void recomp_dispatch_ic(Ctx* c, uint32_t target, uint64_t* ic);
extern "C" uint64_t recomp_rdtsc(void);
extern "C" void recomp_fatal(Ctx* c, uint32_t eip, const char* what);
extern "C" void recomp_cpuid(Ctx* c);


extern "C" {
typedef void (*GuestFn)(void*);
typedef struct RecompEntry { uint32_t addr; GuestFn fn; } RecompEntry;
uint32_t recomp_num_tables(void);
const RecompEntry* recomp_table_entries(uint32_t i, uint32_t* n);
}
namespace host { void log(const char* fmt, ...); }

namespace w32 {
namespace {

struct Sample {
    uintptr_t pc, lr;
    uintptr_t ret1, ret2;  // library code: the return addresses in the first two frame records (0: not read)
    uint32_t tid;
};
constexpr size_t kRing = 1 << 16;
Sample g_ring[kRing];
std::atomic<size_t> g_head{0};
std::atomic<uint64_t> g_total{0};
uintptr_t g_mainBase = 0, g_mainLo = 0, g_mainHi = 0;  // the binary's load base and executable segment
pid_t g_pid = 0;
std::atomic<bool> g_frameReads{true};
#if !defined(__ANDROID__)
// the thread's id, cached (the executable's own TLS: no allocation in the signal handler)
__attribute__((tls_model("initial-exec"))) thread_local pid_t t_tid = 0;
#endif

void onProf(int, siginfo_t*, void* ctx) {
    const int savedErrno = errno;
    auto* uc = static_cast<ucontext_t*>(ctx);
    Sample s{};
    uintptr_t fp = 0, sp = 0;
#if defined(__aarch64__)
    s.pc = uc->uc_mcontext.pc;
    s.lr = uc->uc_mcontext.regs[30];
    fp = uc->uc_mcontext.regs[29];
    sp = uc->uc_mcontext.sp;
#elif defined(__x86_64__)
    s.pc = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RIP]);
    fp = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RBP]);
    sp = static_cast<uintptr_t>(uc->uc_mcontext.gregs[REG_RSP]);
#else
    (void)uc;
#endif
#if !defined(__ANDROID__)
    // The libraries keep frame records ([fp]: the caller's record, [fp + 8]: the return address;
    // the host's own code omits frame pointers): two records name the binary's caller of a
    // library function also from a helper inside the library (the vDSO under libc's wrapper,
    // a futex wait under pthread_mutex_lock). Read through the kernel, which never faults.
    auto plausible = [sp](uintptr_t f) { return f >= sp && f - sp < (1u << 20) && !(f & 7); };
    if ((s.pc < g_mainLo || s.pc >= g_mainHi) && plausible(fp) && g_frameReads.load(std::memory_order_relaxed)) {
        uintptr_t rec[2] = {};  // caller's fp, return address
        iovec local{rec, sizeof rec}, remote{reinterpret_cast<void*>(fp), sizeof rec};
        if (process_vm_readv(g_pid, &local, 1, &remote, 1, 0) == static_cast<ssize_t>(sizeof rec)) {
            s.ret1 = rec[1];
            if (plausible(rec[0]) && rec[0] > fp) {
                local = {&s.ret2, sizeof s.ret2}, remote = {reinterpret_cast<void*>(rec[0] + sizeof(void*)), sizeof s.ret2};
                if (process_vm_readv(g_pid, &local, 1, &remote, 1, 0) != static_cast<ssize_t>(sizeof s.ret2)) s.ret2 = 0;
            }
        } else if (errno == EPERM || errno == ENOSYS) {
            g_frameReads.store(false, std::memory_order_relaxed);
        }
    }
#else
    (void)fp, (void)sp;
#endif
#if defined(__ANDROID__)
    s.tid = static_cast<uint32_t>(gettid());  // bionic's cached id: a shared library's thread_local may allocate on first use
#else
    if (!t_tid) t_tid = static_cast<pid_t>(syscall(SYS_gettid));
    s.tid = static_cast<uint32_t>(t_tid);
#endif
    const size_t i = g_head.fetch_add(1, std::memory_order_relaxed);
    g_ring[i & (kRing - 1)] = s;
    g_total.fetch_add(1, std::memory_order_relaxed);
    errno = savedErrno;
}

// ---- the binary ------------------------------------------------------------------------------
std::string g_exePath, g_buildId;

int onObject(dl_phdr_info* info, size_t, void* data) {
    const auto probe = *static_cast<const uintptr_t*>(data);
    uintptr_t lo = 0, hi = 0;
    bool mine = false;
    for (int i = 0; i < info->dlpi_phnum && !mine; ++i) {
        const auto& ph = info->dlpi_phdr[i];
        if (ph.p_type != PT_LOAD || !(ph.p_flags & PF_X)) continue;
        lo = info->dlpi_addr + ph.p_vaddr, hi = lo + ph.p_memsz;
        mine = probe >= lo && probe < hi;
    }
    if (!mine) return 0;
    g_mainBase = info->dlpi_addr, g_mainLo = lo, g_mainHi = hi;
    for (int i = 0; i < info->dlpi_phnum; ++i) {  // NT_GNU_BUILD_ID: FableRecomp.syms must be for this build
        const auto& ph = info->dlpi_phdr[i];
        if (ph.p_type != PT_NOTE) continue;
        const char* p = reinterpret_cast<const char*>(info->dlpi_addr + ph.p_vaddr);
        const char* end = p + ph.p_memsz;
        while (p + sizeof(ElfW(Nhdr)) <= end) {
            const auto* nh = reinterpret_cast<const ElfW(Nhdr)*>(p);
            const char* name = p + sizeof *nh;
            const char* desc = name + ((nh->n_namesz + 3) & ~3u);
            if (nh->n_type == NT_GNU_BUILD_ID && nh->n_namesz == 4 && std::memcmp(name, "GNU", 4) == 0 && desc + nh->n_descsz <= end) {
                g_buildId.clear();
                for (uint32_t k = 0; k < nh->n_descsz; ++k) {
                    char b[3];
                    std::snprintf(b, sizeof b, "%02x", static_cast<unsigned char>(desc[k]));
                    g_buildId += b;
                }
            }
            p = desc + ((nh->n_descsz + 3) & ~3u);
        }
    }
    return 1;
}

void findSelf() {
    uintptr_t probe = reinterpret_cast<uintptr_t>(&findSelf);
    dl_iterate_phdr(onObject, &probe);
    Dl_info info{};
    if (dladdr(reinterpret_cast<void*>(&findSelf), &info) && info.dli_fname && *info.dli_fname) {
        g_exePath = info.dli_fname;
        if (g_exePath[0] != '/') {  // relative to the start directory (the game changes it later)
            char cwd[4096];
            if (getcwd(cwd, sizeof cwd)) g_exePath = std::string(cwd) + "/" + g_exePath;
        }
    }
}

// Host symbols: FableRecomp.syms next to the binary ("# build-id <hex>", then "<hex address> <name>"
// lines in link-time addresses).
struct HostSym { uintptr_t addr; std::string name; std::string link; };  // demangled, link name
std::vector<HostSym> g_syms;  // absolute addresses, sorted
std::unordered_map<std::string, std::string> g_linkNames;  // profile name -> link name, for the dump

std::string clip(std::string s) {
    if (s.size() > 120) s.resize(117), s += "...";
    return s;
}

void loadSyms() {
    if (g_exePath.empty()) return;
    const std::string path = g_exePath + ".syms";
    FILE* f = std::fopen(path.c_str(), "r");
    if (!f) { host::log("profile: no %s: host code in the binary is named by its function's offset (sub_...)", path.c_str()); return; }
    char* line = nullptr;
    size_t cap = 0;
    std::string id;
    while (getline(&line, &cap, f) > 0) {
        line[std::strcspn(line, "\r\n")] = 0;
        if (line[0] == '#') {
            if (std::strncmp(line, "# build-id ", 11) == 0) id = line + 11;
            continue;
        }
        char* rest = nullptr;
        const unsigned long long a = std::strtoull(line, &rest, 16);
        if (rest == line || *rest != ' ' || !a) continue;
        int st = -1;
        char* dm = rest[1] == '_' && rest[2] == 'Z' ? abi::__cxa_demangle(rest + 1, nullptr, nullptr, &st) : nullptr;
        g_syms.push_back({g_mainBase + static_cast<uintptr_t>(a), st == 0 && dm ? dm : rest + 1, rest + 1});
        std::free(dm);
    }
    std::free(line);
    std::fclose(f);
    if (!id.empty() && !g_buildId.empty() && id != g_buildId) {
        host::log("profile: %s is for build %s, this is %s: not used", path.c_str(), id.c_str(), g_buildId.c_str());
        g_syms.clear();
        return;
    }
    std::sort(g_syms.begin(), g_syms.end(), [](const HostSym& a, const HostSym& b) { return a.addr < b.addr; });
    host::log("profile: %zu host symbols from %s", g_syms.size(), path.c_str());
}

const HostSym* symAt(uintptr_t pc) {
    auto it = std::upper_bound(g_syms.begin(), g_syms.end(), pc, [](uintptr_t v, const HostSym& s) { return v < s.addr; });
    return it == g_syms.begin() ? nullptr : &*(it - 1);
}

// The start of the function holding pc, from the unwind tables (any loaded object), or 0.
struct EhBases { void* tbase; void* dbase; void* func; };
using FindFdeFn = const void* (*)(void*, EhBases*);
FindFdeFn g_findFde = nullptr;
uintptr_t funcStart(uintptr_t pc) {
    if (!g_findFde) return 0;
    EhBases b{};
    return g_findFde(reinterpret_cast<void*>(pc), &b) ? reinterpret_cast<uintptr_t>(b.func) : 0;
}

// ---- lifted code -----------------------------------------------------------------------------
struct GuestFnEntry { uintptr_t host; uint32_t guest; const char* marker; };  // marker: a known host function instead
std::vector<GuestFnEntry> g_guestFns;  // sorted by host address
uintptr_t g_guestLo = 0, g_guestHi = 0;

// The runtime and COM functions the linker lays out after the lifted code are named here too
// (for a binary without FableRecomp.syms). A sample counts for an entry only inside that
// entry's own function (FableRecomp.syms, else the unwind tables): the last entry used to
// collect every sample in the 128 KB of host code after it.
void collectGuestFns() {
    uintptr_t lastLifted = 0;
    for (uint32_t t = 0; t < recomp_num_tables(); ++t) {
        uint32_t n = 0;
        const RecompEntry* e = recomp_table_entries(t, &n);
        for (uint32_t i = 0; i < n; ++i) {
            const auto h = reinterpret_cast<uintptr_t>(e[i].fn);
            g_guestFns.push_back({h, e[i].addr, nullptr});
            lastLifted = std::max(lastLifted, h);
        }
    }
    const std::pair<void*, const char*> markers[] = {
        {reinterpret_cast<void*>(&recomp_dispatch), "recomp_dispatch"}, {reinterpret_cast<void*>(&recomp_dispatch_ic), "recomp_dispatch_ic"},
        {reinterpret_cast<void*>(&recomp_lookup), "recomp_lookup"},     {reinterpret_cast<void*>(&recomp_rdtsc), "recomp_rdtsc"},
        {reinterpret_cast<void*>(&recomp_fatal), "recomp_fatal"},       {reinterpret_cast<void*>(&recomp_cpuid), "recomp_cpuid"},
        {reinterpret_cast<void*>(&host::com::comAddRef), "com::AddRef"}, {reinterpret_cast<void*>(&host::com::comRelease), "com::Release"},
        {reinterpret_cast<void*>(&host::com::comQueryInterface), "com::QueryInterface"},
    };
    for (const auto& [fn, name] : markers) g_guestFns.push_back({reinterpret_cast<uintptr_t>(fn), 0, name});
    std::sort(g_guestFns.begin(), g_guestFns.end(), [](const GuestFnEntry& a, const GuestFnEntry& b) { return a.host < b.host; });
    if (g_guestFns.empty()) return;
    g_guestLo = g_guestFns.front().host;
    g_guestHi = g_guestFns.back().host + 0x20000;  // a window only: resolve() checks each sample's function
    auto next = std::upper_bound(g_syms.begin(), g_syms.end(), lastLifted, [](uintptr_t v, const HostSym& x) { return v < x.addr; });
    if (next != g_syms.end() && next->addr > g_guestFns.back().host) g_guestHi = next->addr;  // the lifted code ends there
}

const char* basenameOf(const char* p) {
    const char* b = std::strrchr(p, '/');
    return b ? b + 1 : p;
}

// The profile's name for pc: "guest 0x...", "host FableRecomp!...", "host runtime!<marker>",
// "host lib!symbol", "host lib!sub_<function offset>" or "host lib+0x<offset>".
std::string resolve(uintptr_t pc) {
    char buf[96];
    if (pc >= g_mainLo && pc < g_mainHi) {
        const GuestFnEntry* g = nullptr;
        if (pc >= g_guestLo && pc < g_guestHi) {
            auto it = std::upper_bound(g_guestFns.begin(), g_guestFns.end(), pc, [](uintptr_t v, const GuestFnEntry& e) { return v < e.host; });
            if (it != g_guestFns.begin()) g = &*(it - 1);
        }
        if (!g_syms.empty()) {  // the lifted functions are not in FableRecomp.syms: whichever starts later holds pc
            const HostSym* h = symAt(pc);
            if (g && g->guest && (!h || g->host > h->addr)) { std::snprintf(buf, sizeof buf, "guest 0x%08X", g->guest); return buf; }
            if (h) {
                std::string n = clip("host FableRecomp!" + h->name);
                if (h->link != h->name) g_linkNames.emplace(n, h->link);
                return n;
            }
        }
        const uintptr_t fs = funcStart(pc);
        if (g && (fs == 0 || fs == g->host)) {
            if (g->marker) return std::string("host runtime!") + g->marker;
            std::snprintf(buf, sizeof buf, "guest 0x%08X", g->guest);
            return buf;
        }
        if (fs) std::snprintf(buf, sizeof buf, "host FableRecomp!sub_%lx", static_cast<unsigned long>(fs - g_mainBase));
        else std::snprintf(buf, sizeof buf, "host FableRecomp+0x%lx", static_cast<unsigned long>(pc - g_mainBase));
        return buf;
    }
    Dl_info info{};
    if (dladdr(reinterpret_cast<void*>(pc), &info) && info.dli_fname) {
        const char* base = basenameOf(info.dli_fname);
        if (info.dli_sname) return clip(std::string("host ") + base + "!" + info.dli_sname);
        const auto fbase = reinterpret_cast<uintptr_t>(info.dli_fbase);
        if (const uintptr_t fs = funcStart(pc); fs && fs >= fbase) {
            std::snprintf(buf, sizeof buf, "!sub_%lx", static_cast<unsigned long>(fs - fbase));
            return std::string("host ") + base + buf;
        }
        std::snprintf(buf, sizeof buf, "+0x%lx", static_cast<unsigned long>(pc - fbase));
        return std::string("host ") + base + buf;
    }
    std::snprintf(buf, sizeof buf, "host ?+0x%lx", static_cast<unsigned long>(pc));
    return buf;
}

std::unordered_map<uintptr_t, std::string> g_names;
const std::string& nameOf(uintptr_t pc) {
    auto it = g_names.find(pc);
    if (it != g_names.end()) return it->second;
    if (g_names.size() > 50000) g_names.clear();  // a few thousand distinct addresses per window
    return g_names.emplace(pc, resolve(pc)).first->second;
}

bool inMain(uintptr_t a) { return a >= g_mainLo && a < g_mainHi; }
std::string shortName(const std::string& n) { return n.compare(0, 5, "host ") == 0 ? n.substr(5) : n; }

// ---- threads and cores (/proc) -----------------------------------------------------------------
struct TaskStat {
    std::string comm;
    unsigned long long ticks = 0, sysTicks = 0, vcs = 0, ivcs = 0;
    int cpu = -1;
};
std::map<uint32_t, TaskStat> readTasks() {
    std::map<uint32_t, TaskStat> out;
    DIR* d = opendir("/proc/self/task");
    if (!d) return out;
    while (dirent* e = readdir(d)) {
        const long tid = std::strtol(e->d_name, nullptr, 10);
        if (tid <= 0) continue;
        TaskStat t;
        char path[64], buf[1024];
        std::snprintf(path, sizeof path, "/proc/self/task/%ld/stat", tid);
        FILE* f = std::fopen(path, "r");
        if (!f) continue;
        const size_t n = std::fread(buf, 1, sizeof buf - 1, f);
        std::fclose(f);
        buf[n] = 0;
        char* lp = std::strchr(buf, '(');
        char* rp = std::strrchr(buf, ')');
        if (!lp || !rp || rp < lp) continue;
        t.comm.assign(lp + 1, static_cast<size_t>(rp - lp - 1));
        // after ")": field 3 (state) is token 0, utime (14) token 11, stime (15) token 12, processor (39) token 36
        std::vector<unsigned long long> v;
        char* save = nullptr;
        for (char* tok = strtok_r(rp + 1, " ", &save); tok && v.size() < 40; tok = strtok_r(nullptr, " ", &save)) v.push_back(std::strtoull(tok, nullptr, 10));
        if (v.size() > 12) t.ticks = v[11] + v[12], t.sysTicks = v[12];
        if (v.size() > 36) t.cpu = static_cast<int>(v[36]);
        std::snprintf(path, sizeof path, "/proc/self/task/%ld/status", tid);
        if ((f = std::fopen(path, "r"))) {
            char line[256];
            while (std::fgets(line, sizeof line, f)) {
                std::sscanf(line, "voluntary_ctxt_switches: %llu", &t.vcs);
                std::sscanf(line, "nonvoluntary_ctxt_switches: %llu", &t.ivcs);
            }
            std::fclose(f);
        }
        out[static_cast<uint32_t>(tid)] = t;
    }
    closedir(d);
    return out;
}

struct CpuStat { unsigned long long busy = 0, total = 0; };
std::vector<CpuStat> readCpus() {
    std::vector<CpuStat> out;
    FILE* f = std::fopen("/proc/stat", "r");
    if (!f) return out;
    char line[512];
    while (std::fgets(line, sizeof line, f)) {
        unsigned n = 0;
        unsigned long long v[8] = {};
        if (std::strncmp(line, "cpu", 3) != 0 || line[3] < '0' || line[3] > '9') continue;
        if (std::sscanf(line, "cpu%u %llu %llu %llu %llu %llu %llu %llu %llu", &n, &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7]) < 5) continue;
        if (n > 63) continue;
        if (out.size() <= n) out.resize(n + 1);
        unsigned long long total = 0;
        for (auto x : v) total += x;
        out[n].total = total;
        out[n].busy = total - v[3] - v[4];  // minus idle and iowait
    }
    std::fclose(f);
    return out;
}

std::map<uint32_t, TaskStat> g_prevTasks;
std::vector<CpuStat> g_prevCpus;
std::chrono::steady_clock::time_point g_prevTime;

using Top = std::vector<std::pair<unsigned, std::string>>;
Top topOf(const std::map<std::string, unsigned>& hist) {
    Top top;
    for (auto& [k, v] : hist) top.push_back({v, k});
    std::sort(top.begin(), top.end(), [](auto& a, auto& b) { return a.first > b.first; });
    return top;
}

// Threads: their share of the samples, the CPU each used (of one core), how often each switched
// (voluntary: it blocked; involuntary: preempted, e.g. sharing a core) and its hottest entries.
void reportThreads(const std::map<uint32_t, std::map<std::string, unsigned>>& perThread, size_t n) {
    const auto now = std::chrono::steady_clock::now();
    const double secs = std::max(1e-3, std::chrono::duration<double>(now - g_prevTime).count());
    const double hz = static_cast<double>(sysconf(_SC_CLK_TCK));
    std::map<uint32_t, TaskStat> tasks = readTasks();
    std::vector<std::pair<double, uint32_t>> order;  // cpu %, tid
    for (auto& [tid, t] : tasks) {
        const auto p = g_prevTasks.find(tid);
        const unsigned long long t0 = p != g_prevTasks.end() ? p->second.ticks : 0;
        order.push_back({100.0 * static_cast<double>(t.ticks - t0) / hz / secs, tid});
    }
    for (auto& [tid, h] : perThread)
        if (!tasks.count(tid)) order.push_back({0.0, tid});  // exited since
    std::sort(order.begin(), order.end(), [](auto& a, auto& b) { return a.first > b.first; });
    host::log("profile: threads over %.0f s (cpu: %% of one core; switches per second: voluntary + involuntary):", secs);
    for (auto& [cpu, tid] : order) {
        unsigned count = 0;
        const auto pt = perThread.find(tid);
        if (pt != perThread.end()) for (auto& [k, v] : pt->second) count += v;
        if (cpu < 1.0 && count * 200u < n) continue;  // under 1 % of a core and 0.5 % of the samples
        const auto ti = tasks.find(tid);
        const auto pi = g_prevTasks.find(tid);
        const TaskStat* t = ti != tasks.end() ? &ti->second : nullptr;
        const TaskStat* p = pi != g_prevTasks.end() ? &pi->second : nullptr;
        const double sys = t ? 100.0 * static_cast<double>(t->sysTicks - (p ? p->sysTicks : 0)) / hz / secs : 0.0;
        const double vcs = t ? static_cast<double>(t->vcs - (p ? p->vcs : 0)) / secs : 0.0;
        const double ivcs = t ? static_cast<double>(t->ivcs - (p ? p->ivcs : 0)) / secs : 0.0;
        std::string hot;
        if (pt != perThread.end()) {
            const Top tt = topOf(pt->second);
            for (size_t i = 0; i < tt.size() && i < 6; ++i) hot += (i ? ", " : ": ") + tt[i].second + " (" + std::to_string(tt[i].first) + ")";
        }
        host::log("profile: thread %u %s%s: cpu %.1f%% (sys %.1f%%), switches %.0f + %.0f/s, last on cpu %d, %u samples (%.1f%%)%s", tid,
                  t ? t->comm.c_str() : "(exited)", static_cast<pid_t>(tid) == g_pid ? " (main)" : "", cpu, sys, vcs, ivcs, t ? t->cpu : -1, count,
                  100.0 * count / static_cast<double>(n), hot.c_str());
    }
    const std::vector<CpuStat> cpus = readCpus();
    std::string line;
    for (size_t i = 0; i < cpus.size(); ++i) {
        const CpuStat p = i < g_prevCpus.size() ? g_prevCpus[i] : CpuStat{};
        const double busy = cpus[i].total > p.total ? 100.0 * static_cast<double>(cpus[i].busy - p.busy) / static_cast<double>(cpus[i].total - p.total) : 0.0;
        char path[96], b[64];
        unsigned long khz = 0;
        std::snprintf(path, sizeof path, "/sys/devices/system/cpu/cpu%zu/cpufreq/scaling_cur_freq", i);
        if (FILE* f = std::fopen(path, "r")) {
            if (std::fscanf(f, "%lu", &khz) != 1) khz = 0;
            std::fclose(f);
        }
        if (khz) std::snprintf(b, sizeof b, "%scpu%zu %.0f%% at %lu MHz", i ? ", " : "", i, busy, khz / 1000);
        else std::snprintf(b, sizeof b, "%scpu%zu %.0f%%", i ? ", " : "", i, busy);
        line += b;
    }
    if (!line.empty()) host::log("profile: cores busy: %s", line.c_str());
    g_prevTasks = std::move(tasks);
    g_prevCpus = cpus;
    g_prevTime = now;
}

void report() {
    const size_t head = g_head.load();
    const size_t n = std::min(head, kRing);
    if (n < 100) return;
    std::vector<Sample> samples(g_ring + 0, g_ring + n);
    g_head.store(0);
    std::map<std::string, unsigned> hist;
    std::map<uint32_t, std::map<std::string, unsigned>> perThread;
    std::map<std::string, unsigned> libCallers;
    unsigned guest = 0, host = 0, libTotal = 0;
    for (const Sample& s : samples) {
        const std::string name = nameOf(s.pc);  // a copy: nameOf below may clear the cache
        ++hist[name];
        ++perThread[s.tid][name];
        if (name.compare(0, 6, "guest ") == 0) ++guest;
        else ++host;
        // time in libraries and in the binary's PLT stubs: the binary's caller is the link
        // register (a stub, or a library function that has called nothing yet) or a return
        // address in the frame records (a library function past its prologue, a helper under
        // it); else the library function that called it
        if (inMain(s.pc) && name != "host FableRecomp![plt]") continue;
        const uintptr_t caller = inMain(s.lr) ? s.lr : inMain(s.ret1) ? s.ret1 : inMain(s.ret2) ? s.ret2 : s.ret1 ? s.ret1 : s.lr;
        ++libTotal;
        ++libCallers[shortName(name) + " <- " + (caller ? shortName(nameOf(caller - 1)) : std::string("?"))];  // caller - 1: the call itself
    }
    host::log("profile: %zu samples (%u guest, %u host) over the last window, %llu in all", n, guest, host, static_cast<unsigned long long>(g_total.load()));
    {   // per-module totals: a stripped library's samples are spread over its addresses and never reach the top list
        std::map<std::string, unsigned> mod;
        for (auto& [k, v] : hist) {
            std::string m = k.substr(0, k.find_first_of("!+"));
            if (m.rfind("guest", 0) == 0) m = "guest";
            mod[m] += v;
        }
        const Top ms = topOf(mod);
        std::string line;
        for (size_t i = 0; i < ms.size() && i < 10; ++i) { char b[96]; std::snprintf(b, sizeof b, "%s%s %.1f%%", i ? ", " : "", ms[i].second.c_str(), 100.0 * ms[i].first / n); line += b; }
        host::log("profile: by module: %s", line.c_str());
    }
    const Top top = topOf(hist);
    for (size_t i = 0; i < top.size() && i < 60; ++i)
        host::log("profile: %5.1f%% %s (%u)", 100.0 * top[i].first / n, top[i].second.c_str(), top[i].first);
    // FABLE_PROFILE_DUMP=<file>: every entry of the window appended ("<samples> <name>", link names), the
    // complete profile rg34xx/gen_hot_order.py --counts orders the hot code by
    if (const char* dump = std::getenv("FABLE_PROFILE_DUMP"); dump && *dump) {
        if (FILE* f = std::fopen(dump, "a")) {
            std::fprintf(f, "# window: %zu samples\n", n);
            for (const auto& [count, name] : top) {  // this binary's functions by their link names
                auto l = g_linkNames.find(name);
                std::fprintf(f, "%u %s%s\n", count, l != g_linkNames.end() ? "host FableRecomp!" : "", l != g_linkNames.end() ? l->second.c_str() : name.c_str());
            }
            std::fclose(f);
        }
    }
    if (const uint64_t nominal = recomp_clock_state.nominal) {  // the counter clock's anchors this window (recomp_clock.h)
        static uint64_t lastAnchors = 0, lastRebases = 0;
        const uint64_t anchors = __atomic_load_n(&recomp_clock_state.anchors, __ATOMIC_RELAXED), rebases = __atomic_load_n(&recomp_clock_state.rebases, __ATOMIC_RELAXED);
        const uint64_t rate = __atomic_load_n(&recomp_clock_state.rate, __ATOMIC_RELAXED);
        host::log("profile: clock: %llu anchors (%llu re-based after a counter jump), CLOCK_MONOTONIC %+.1f ppm against CNTFRQ_EL0",
                  static_cast<unsigned long long>(anchors - lastAnchors), static_cast<unsigned long long>(rebases - lastRebases),
                  (static_cast<double>(rate) / static_cast<double>(nominal) - 1.0) * 1e6);
        lastAnchors = anchors, lastRebases = rebases;
    }
    reportThreads(perThread, n);
    if (libTotal) {
        const Top lt = topOf(libCallers);
        host::log("profile: library time %.1f%% of the samples, by caller (library function <- caller):", 100.0 * libTotal / n);
        for (size_t i = 0; i < lt.size() && i < 30; ++i)
            host::log("profile: lib %5.1f%% %s (%u)", 100.0 * lt[i].first / n, lt[i].second.c_str(), lt[i].first);
    }
}

}  // namespace

void startProfiler() {
    const char* e = std::getenv("FABLE_PROFILE");
    if (!e || std::atoi(e) == 0) return;
    const int seconds = std::atoi(e) > 1 ? std::atoi(e) : 60;
    g_pid = getpid();
    if (const char* c = std::getenv("FABLE_PROFILE_CALLERS"); c && *c == '0') g_frameReads.store(false);  // no frame reads (library callers from lr only)
    findSelf();  // before the signal handler runs: it reads the executable range
    g_findFde = reinterpret_cast<FindFdeFn>(dlsym(RTLD_DEFAULT, "_Unwind_Find_FDE"));
    g_prevTasks = readTasks(), g_prevCpus = readCpus(), g_prevTime = std::chrono::steady_clock::now();
    struct sigaction sa{};
    sa.sa_sigaction = onProf;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGPROF, &sa, nullptr);
    itimerval tv{};
    tv.it_interval.tv_usec = 1000;
    tv.it_value.tv_usec = 1000;
    setitimer(ITIMER_PROF, &tv, nullptr);
    std::fprintf(stderr, "profile: sampling at 1 kHz, report every %d s\n", seconds);
    (void)recomp_clock_ns();  // the clock's one-time check has run
    if (const uint64_t hz = recomp_clock_state.freq)
        host::log("profile: clock: the counter (CNTVCT_EL0, %.3f MHz) anchored to CLOCK_MONOTONIC every 64 ms, for QueryPerformanceCounter / GetTickCount / timeGetTime / rdtsc", static_cast<double>(hz) / 1e6);
    else
        host::log("profile: clock: clock_gettime (no usable counter, or FABLE_FAST_CLOCK=0)");
    std::thread([seconds] {
#if defined(__linux__)
        pthread_setname_np(pthread_self(), "profiler");
#endif
        sleep(5);
        loadSyms();
        collectGuestFns();
        for (;;) {
            sleep(static_cast<unsigned>(seconds));
            report();
        }
    }).detach();
}

}  // namespace w32
