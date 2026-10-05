// Kernel-call traps, guest threads, the global lock, dispatcher waits, handles, the kernel
// worker (timers, DPCs, interrupts).
#include "xhost.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <csetjmp>
#include <map>
#include <thread>
#include <unordered_map>
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

extern "C" {
void* recomp_landing_chain_get(void);
void recomp_landing_chain_set(void* chain);
}

namespace xb {

// ============================================================================================
// logging
// ============================================================================================
int g_logLevel = 1;
static std::mutex g_logLock;
static FILE* g_logFile;

void logf(int level, const char* fmt, ...) {
    (void)level;
    char buf[2048];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    std::lock_guard<std::mutex> l(g_logLock);
    XThread* t = curThread();
    fprintf(stderr, "[%u] %s\n", t ? t->id : 0, buf);
    if (!g_logFile) g_logFile = fopen("xbox_recomp.log", "w");
    if (g_logFile) {
        fprintf(g_logFile, "[%u] %s\n", t ? t->id : 0, buf);
        fflush(g_logFile);
    }
}

void die(const char* fmt, ...) {
    char buf[2048];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    logf(0, "FATAL: %s", buf);
    fflush(stderr);
    _exit(1);
}

// ============================================================================================
// kernel export registry and traps
// ============================================================================================
static std::vector<KExport>& exports() {
    static auto* v = new std::vector<KExport>(400);
    return *v;
}
static std::vector<uint32_t (*)()>& dataInits() {
    static auto* v = new std::vector<uint32_t (*)()>(400);
    return *v;
}

static int ordinalOf(const char* name) {
    struct Row { int ord; const char* name; };
    static const Row rows[] = {
#include "kernel_ordinals.inc"
    };
    for (const Row& r : rows)
        if (std::strcmp(r.name, name) == 0) return r.ord;
    fprintf(stderr, "kernel export %s has no ordinal\n", name);
    abort();
}
static const char* ordinalName(int ord) {
    struct Row { int ord; const char* name; };
    static const Row rows[] = {
#include "kernel_ordinals.inc"
    };
    for (const Row& r : rows)
        if (r.ord == ord) return r.name;
    return "?";
}
KReg::KReg(const char* name, int args, CC cc, KFn fn) {
    const int ord = ordinalOf(name);
    exports()[ord] = {ord, name, args, cc, fn, 0};
}
KReg::KReg(const char* name, uint32_t (*init)()) {
    const int ord = ordinalOf(name);
    exports()[ord] = {ord, name, 0, CC::Std, nullptr, 0};
    dataInits()[ord] = init;
}

void kernelInit() {
    for (size_t i = 0; i < exports().size(); ++i)
        if (dataInits()[i]) exports()[i].data = dataInits()[i]();
}

const char* kernelName(int ord) {
    if (ord <= 0 || ord >= static_cast<int>(exports().size())) return "?";
    return exports()[ord].name ? exports()[ord].name : ordinalName(ord);
}

uint32_t kernelResolve(int ord) {
    if (ord <= 0 || ord >= static_cast<int>(exports().size())) die("kernel import ordinal %d out of range", ord);
    const KExport& e = exports()[ord];
    if (e.data) return e.data;
    if (!e.fn && dataInits()[ord]) die("kernel variable %s has no data", e.name);
    return kTrapBase + 16u * static_cast<uint32_t>(ord);  // unimplemented functions trap with a message
}

namespace {
constexpr uint32_t kHostTrapBase = kTrapBase + 0x10000u;
struct HostTrap { KFn fn; int args; const char* name; };
std::vector<HostTrap>& hostTraps() { static std::vector<HostTrap> v; return v; }
}

uint32_t hostTrap(KFn fn, int args, const char* name) {
    hostTraps().push_back({fn, args, name});
    return kHostTrapBase + 16u * static_cast<uint32_t>(hostTraps().size() - 1);
}

static int onUnknownTarget(Ctx* c, uint32_t target) {
    if (target >= kHostTrapBase && target < kHostTrapBase + 16u * hostTraps().size()) {
        const HostTrap& t = hostTraps()[(target - kHostTrapBase) / 16u];
        XLOG(3, "host trap %s from %08X", t.name, rd32(c->esp));
        c->eax = t.fn(c);
        c->esp += 4 + 4u * static_cast<uint32_t>(t.args);
        return 1;
    }
    if (target < kTrapBase || target >= kTrapBase + 16u * 400u) return 0;
    const int ord = static_cast<int>((target - kTrapBase) / 16u);
    const KExport& e = exports()[ord];
    if (!e.fn) die("unimplemented kernel export #%d %s (called from 0x%08X)", ord, kernelName(ord), rd32(c->esp));
    const uint32_t ret = rd32(c->esp);
    if (g_logLevel >= 2) {
        if (e.cc == CC::Fast)
            logf(2, "%s(ecx=%08X, edx=%08X) from %08X", e.name, c->ecx, c->edx, ret);
        else {
            char args[160] = "";
            int n = 0;
            for (int i = 0; i < std::min(e.argWords, 10); ++i) n += snprintf(args + n, sizeof args - n, i ? ", %X" : "%X", ARG(c, i));
            logf(2, "%s(%s) from %08X", e.name, args, ret);
        }
    }
    const uint32_t r = e.fn(c);
    c->eax = r;
    c->esp += 4 + (e.cc == CC::Cdecl ? 0 : 4u * static_cast<uint32_t>(e.argWords));
    return 1;
}

// ============================================================================================
// threads
// ============================================================================================
FairLock g_gil;
std::condition_variable_any g_dispatchCv;
static thread_local XThread* t_cur;
static thread_local sigjmp_buf* t_exitJmp;
static std::mutex g_threadsLock;
static std::unordered_map<uint32_t, XThread*> g_threads;  // ethread -> thread
static std::atomic<uint32_t> g_nextId{1};

XThread* curThread() { return t_cur; }
XThread* threadByEthread(uint32_t e) {
    std::lock_guard<std::mutex> l(g_threadsLock);
    auto it = g_threads.find(e);
    return it == g_threads.end() ? nullptr : it->second;
}

void bindHostThread(XThread* t) { t_cur = t; }

XThread* newThread(uint32_t stackSize, uint32_t tlsSize, uint32_t extensionSize) {
    auto* t = new XThread;
    t->id = g_nextId++;
    stackSize = std::max<uint32_t>((stackSize + 0xFFFF) & ~0xFFFFu, 0x40000);
    // TLS must sit exactly TlsDataSize below StackBase: XAPI finds it at
    // StackBase + _tls_index * 4 with _tls_index = -TlsDataSize / 4.
    const uint32_t mem = physAlloc(stackSize + ((tlsSize + 0xFFF) & ~0xFFFu), 0x1000, 0x10000, kPhysSize, false);
    if (!mem) die("out of guest memory for a thread stack");
    std::memset(gp(mem), 0, stackSize + ((tlsSize + 0xFFF) & ~0xFFFu));
    t->stackLo = mem;
    t->stackHi = mem + stackSize + ((tlsSize + 0xFFF) & ~0xFFFu);  // NtTib.StackBase; TLS sits just below it
    t->tlsSize = tlsSize;
    t->tls = t->stackHi - tlsSize;
    // KPCR (one per thread: fs points at it) and ETHREAD.
    t->pcr = poolAllocZero(0x300);
    t->ethread = poolAllocZero(0x140 + extensionSize);
    const uint32_t pcr = t->pcr, et = t->ethread;
    wr32(pcr + 0x00, 0xFFFFFFFFu);   // NtTib.ExceptionList: end of the SEH chain
    wr32(pcr + 0x04, t->stackHi);    // StackBase
    wr32(pcr + 0x08, t->stackLo);    // StackLimit
    wr32(pcr + 0x18, pcr);           // NtTib.Self
    wr32(pcr + 0x1C, pcr);           // SelfPcr
    wr32(pcr + 0x20, pcr + 0x28);    // Prcb
    wr8(pcr + 0x24, 0);              // Irql
    wr32(pcr + 0x28, et);            // Prcb.CurrentThread
    wr32(pcr + 0x50, pcr + 0x50);    // Prcb.DpcListHead
    wr32(pcr + 0x54, pcr + 0x50);
    wr8(et + 0x00, DO_Thread);       // KTHREAD.Header
    wr8(et + 0x02, 0x110 / 4);
    wr32(et + 0x08, et + 0x08);
    wr32(et + 0x0C, et + 0x08);
    wr32(et + 0x1C, t->stackHi);     // StackBase
    wr32(et + 0x20, t->stackLo);     // StackLimit
    wr32(et + 0x24, t->tls);         // KernelStack
    wr32(et + 0x28, t->tls);         // TlsData
    wr8(et + 0x32, 8);               // Priority
    wr32(et + 0x12C, t->id);         // ETHREAD.UniqueThread
    t->ctx.fs_base = pcr;
    t->ctx.esp = (t->tls - 16) & ~15u;
    t->ctx.fpu.cw = 0x027F;
    t->ctx.mxcsr = 0x1F80;
    {
        std::lock_guard<std::mutex> l(g_threadsLock);
        g_threads[et] = t;
    }
    return t;
}

uint32_t guestCall(uint32_t fn, std::initializer_list<uint32_t> args) {
    Ctx* c = &t_cur->ctx;
    const Ctx saved = *c;
    uint32_t esp = c->esp;
    const uint32_t* a = args.end();
    while (a != args.begin()) {
        --a;
        esp -= 4;
        wr32(esp, *a);
    }
    esp -= 4;
    wr32(esp, 0xFFFFFFF0u);  // return address (never used: lifted returns are C returns)
    c->esp = esp;
    recomp_dispatch(c, fn);
    const uint32_t r = c->eax;
    const X87 fpu = c->fpu;
    *c = saved;
    c->fpu = fpu;
    c->eax = r;
    return r;
}

static void threadMain(XThread* t, uint32_t start, uint32_t ctx, uint32_t system) {
    g_gil.lock();
    t_cur = t;
    sigjmp_buf jb;
    t_exitJmp = &jb;
    if (sigsetjmp(jb, 0) == 0) {
        {
            std::unique_lock<FairLock> lk(g_gil, std::adopt_lock);
            g_dispatchCv.wait(lk, [&] { return t->suspend <= 0; });
            lk.release();
        }
        XLOG(1, "thread %u starts at 0x%08X (context 0x%08X%s)", t->id, start, ctx, system ? ", via system routine" : "");
        if (system) guestCall(system, {start, ctx});
        else guestCall(start, {ctx});
        wr32(t->ethread + 0x120, 0);
    }
    recomp_landing_chain_set(nullptr);
    XLOG(1, "thread %u exits (status 0x%08X)", t->id, rd32(t->ethread + 0x120));
    wr32(t->ethread + 0x04, 1);  // threads are signaled when they terminate
    wr8(t->ethread + 0x77, 1);   // HasTerminated
    g_dispatchCv.notify_all();
    t_cur = nullptr;
    g_gil.unlock();
}

void startThread(XThread* t, uint32_t start, uint32_t ctx, uint32_t system, bool suspended) {
    t->suspend = suspended ? 1 : 0;
    wr32(t->ethread + 0x130, start);
    std::thread(threadMain, t, start, ctx, system).detach();
}

void exitCurrentThread(uint32_t status) {
    XThread* t = t_cur;
    wr32(t->ethread + 0x120, status);
    if (!t_exitJmp) die("thread exit outside a guest thread");
    siglongjmp(*t_exitJmp, 1);
}

// ============================================================================================
// handles
// ============================================================================================
static std::mutex g_handleLock;
static std::map<uint32_t, Handle> g_handles;
static std::map<std::string, uint32_t> g_named;
static uint32_t g_nextHandle = 0x100;

uint32_t handleNew(const Handle& h) {
    std::lock_guard<std::mutex> l(g_handleLock);
    const uint32_t v = g_nextHandle;
    g_nextHandle += 4;
    g_handles[v] = h;
    return v;
}
Handle* handleGet(uint32_t h) {
    std::lock_guard<std::mutex> l(g_handleLock);
    auto it = g_handles.find(h);
    return it == g_handles.end() ? nullptr : &it->second;
}
void hostFileClose(HostFile* f);
void hostFileAddRef(HostFile* f);
bool handleClose(uint32_t h) {
    HostFile* f = nullptr;
    {
        std::lock_guard<std::mutex> l(g_handleLock);
        auto it = g_handles.find(h);
        if (it == g_handles.end()) return false;
        f = it->second.file;
        for (auto n = g_named.begin(); n != g_named.end();)
            n = n->second == h ? g_named.erase(n) : std::next(n);
        g_handles.erase(it);
    }
    if (f) hostFileClose(f);
    return true;
}
uint32_t handleDup(uint32_t h) {
    std::lock_guard<std::mutex> l(g_handleLock);
    auto it = g_handles.find(h);
    if (it == g_handles.end()) return 0;
    Handle copy = it->second;
    if (copy.file) hostFileAddRef(copy.file);
    const uint32_t v = g_nextHandle;
    g_nextHandle += 4;
    g_handles[v] = copy;
    return v;
}
uint32_t namedObjectFind(const std::string& name) {
    std::lock_guard<std::mutex> l(g_handleLock);
    auto it = g_named.find(name);
    return it == g_named.end() ? 0 : it->second;
}
void namedObjectAdd(const std::string& name, uint32_t h) {
    std::lock_guard<std::mutex> l(g_handleLock);
    g_named[name] = h;
}

// ============================================================================================
// time and waits
// ============================================================================================
uint64_t monoTime100ns() {
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(
                                     std::chrono::steady_clock::now().time_since_epoch()).count() / 100);
}
uint64_t systemTime100ns() {
    const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return static_cast<uint64_t>(ns / 100) + 116444736000000000ull;  // 1601 epoch
}

static bool isSignaled(uint32_t o, uint32_t me) {
    const uint8_t type = rd8(o) & 0x7F;
    const int32_t s = static_cast<int32_t>(rd32(o + 4));
    if (type == DO_Mutant) return s > 0 || rd32(o + 0x18) == me;
    return s > 0;
}
static void satisfy(uint32_t o, uint32_t me) {
    const uint8_t type = rd8(o) & 0x7F;
    switch (type) {
    case DO_SyncEvent: case DO_SyncTimer: wr32(o + 4, 0); break;
    case DO_Semaphore: wr32(o + 4, rd32(o + 4) - 1); break;
    case DO_Mutant:
        wr32(o + 4, rd32(o + 4) - 1);
        wr32(o + 0x18, me);
        break;
    default: break;
    }
}

void signalObject(uint32_t) { g_dispatchCv.notify_all(); }

uint32_t keSetEvent(uint32_t ev) {
    const uint32_t old = rd32(ev + 4);
    wr32(ev + 4, 1);
    g_dispatchCv.notify_all();
    return old;
}

void deliverApcs(Ctx* c) {
    (void)c;
    XThread* t = t_cur;
    while (!t->userApcs.empty()) {
        const XThread::Apc a = t->userApcs.front();
        t->userApcs.erase(t->userApcs.begin());
        guestCall(a.routine, {a.a1, a.a2, a.a3});
    }
}

uint32_t waitObjects(Ctx* c, const uint32_t* objs, int n, bool waitAll, bool alertable, uint32_t timeoutPtr) {
    XThread* t = t_cur;
    const uint32_t me = t->ethread;
    bool infinite = timeoutPtr == 0;
    uint64_t deadline = 0;
    if (!infinite) {
        const int64_t v = static_cast<int64_t>(rd64(timeoutPtr));
        const uint64_t now = monoTime100ns();
        if (v < 0) deadline = now + static_cast<uint64_t>(-v);
        else if (v == 0) deadline = now;
        else {
            const uint64_t sys = systemTime100ns();
            deadline = now + (static_cast<uint64_t>(v) > sys ? static_cast<uint64_t>(v) - sys : 0);
        }
    }
    std::unique_lock<FairLock> lk(g_gil, std::adopt_lock);
    uint32_t result = ST_TIMEOUT;
    for (;;) {
        if (alertable && !t->userApcs.empty()) {
            lk.release();
            deliverApcs(c);
            return ST_USER_APC;
        }
        if (waitAll) {
            bool all = true;
            for (int i = 0; i < n && all; ++i) all = isSignaled(objs[i], me);
            if (all) {
                for (int i = 0; i < n; ++i) satisfy(objs[i], me);
                result = 0;
                break;
            }
        } else {
            int hit = -1;
            for (int i = 0; i < n && hit < 0; ++i)
                if (isSignaled(objs[i], me)) hit = i;
            if (hit >= 0) {
                satisfy(objs[hit], me);
                result = static_cast<uint32_t>(hit);
                break;
            }
        }
        if (!infinite && monoTime100ns() >= deadline) break;
        if (infinite) g_dispatchCv.wait_for(lk, std::chrono::milliseconds(100));
        else {
            const uint64_t now = monoTime100ns();
            const uint64_t left = deadline > now ? deadline - now : 0;
            g_dispatchCv.wait_for(lk, std::chrono::nanoseconds(std::min<uint64_t>(left, 1000000) * 100));
        }
    }
    lk.release();
    return result;
}

// ============================================================================================
// kernel worker: timers, DPCs, interrupts
// ============================================================================================
namespace {
struct Timer { uint64_t due; int32_t period; uint32_t dpc; };
std::map<uint32_t, Timer> g_timers;  // KTIMER -> state
struct Dpc { uint32_t dpc, a1, a2; };
std::vector<Dpc> g_dpcs;
std::map<uint32_t, uint32_t> g_interrupts;  // vector -> KINTERRUPT
std::condition_variable_any g_workerCv;
XThread* g_worker;
}

void timerSet(uint32_t timer, int64_t due, int32_t periodMs, uint32_t dpc) {
    const uint64_t now = monoTime100ns();
    uint64_t at;
    if (due < 0) at = now + static_cast<uint64_t>(-due);
    else if (due == 0) at = now;
    else {
        const uint64_t sys = systemTime100ns();
        at = now + (static_cast<uint64_t>(due) > sys ? static_cast<uint64_t>(due) - sys : 0);
    }
    wr32(timer + 4, 0);  // SignalState
    wr8(timer + 3, 1);   // Inserted
    wr32(timer + 0x20, dpc);
    wr32(timer + 0x24, static_cast<uint32_t>(periodMs));
    g_timers[timer] = {at, periodMs, dpc};
    g_workerCv.notify_all();
}

bool timerCancel(uint32_t timer) {
    wr8(timer + 3, 0);
    return g_timers.erase(timer) != 0;
}

bool dpcQueue(uint32_t dpc, uint32_t a1, uint32_t a2) {
    if (rd8(dpc + 2)) return false;  // already queued
    wr8(dpc + 2, 1);
    wr32(dpc + 0x14, a1);
    wr32(dpc + 0x18, a2);
    g_dpcs.push_back({dpc, a1, a2});
    g_workerCv.notify_all();
    return true;
}

bool dpcRemove(uint32_t dpc) {
    if (!rd8(dpc + 2)) return false;
    wr8(dpc + 2, 0);
    g_dpcs.erase(std::remove_if(g_dpcs.begin(), g_dpcs.end(), [&](const Dpc& d) { return d.dpc == dpc; }), g_dpcs.end());
    return true;
}

void interruptConnect(uint32_t vector, uint32_t kint) { g_interrupts[vector] = kint; }

// Lock-free: device threads raise interrupts while holding their own locks; the worker
// drains the mask (it wakes at least every 2 ms, so a missed notify costs that much).
static std::atomic<uint32_t> g_irqMask{0};
void (*g_irqEoi)(uint32_t vector);
static std::vector<void (*)()> g_services;
void workerAddService(void (*fn)()) { g_services.push_back(fn); }  // GIL held
void workerKick() { g_workerCv.notify_all(); }
void interruptRaise(uint32_t vector) {
    g_irqMask.fetch_or(1u << vector);
    g_workerCv.notify_all();
}

static void workerMain() {
    g_gil.lock();
    t_cur = g_worker;
    std::unique_lock<FairLock> lk(g_gil, std::adopt_lock);
    for (;;) {
        // Interrupts first (ISRs queue DPCs), then expired timers, then DPCs.
        for (uint32_t mask = g_irqMask.exchange(0); mask; mask &= mask - 1) {
            const uint32_t v = static_cast<uint32_t>(__builtin_ctz(mask));
            auto it = g_interrupts.find(v);
            if (it != g_interrupts.end()) {
                const uint32_t ki = it->second;
                XLOG(3, "ISR for interrupt %u", v);
                wr8(t_cur->pcr + 0x24, static_cast<uint8_t>(rd32(ki + 0x0C)));
                guestCall(rd32(ki + 0x00), {ki, rd32(ki + 0x04)});
                wr8(t_cur->pcr + 0x24, 0);
            }
            if (g_irqEoi) g_irqEoi(v);
        }
        const uint64_t now = monoTime100ns();
        uint64_t next = now + 100000;  // 10 ms
        for (auto it = g_timers.begin(); it != g_timers.end();) {
            if (it->second.due <= now) {
                const uint32_t tm = it->first;
                const Timer tv = it->second;
                wr32(tm + 4, 1);
                g_dispatchCv.notify_all();
                if (tv.dpc) dpcQueue(tv.dpc, static_cast<uint32_t>(systemTime100ns()), static_cast<uint32_t>(systemTime100ns() >> 32));
                if (tv.period > 0) {
                    it->second.due = now + static_cast<uint64_t>(tv.period) * 10000;
                    ++it;
                } else {
                    wr8(tm + 3, 0);
                    it = g_timers.erase(it);
                }
                continue;
            }
            next = std::min(next, it->second.due);
            ++it;
        }
        while (!g_dpcs.empty()) {
            const Dpc d = g_dpcs.front();
            g_dpcs.erase(g_dpcs.begin());
            wr8(d.dpc + 2, 0);
            wr8(t_cur->pcr + 0x24, 2);  // DISPATCH_LEVEL
            guestCall(rd32(d.dpc + 0x0C), {d.dpc, rd32(d.dpc + 0x10), d.a1, d.a2});
            wr8(t_cur->pcr + 0x24, 0);
        }
        for (auto fn : g_services) fn();
        if (g_irqMask.load() || !g_dpcs.empty()) continue;
        const uint64_t now2 = monoTime100ns();
        if (next > now2) g_workerCv.wait_for(lk, std::chrono::nanoseconds(std::min<uint64_t>(next - now2, 20000) * 100));
    }
}

void workerStart() {
    g_worker = newThread(0x40000, 0, 0);
    g_worker->system = true;
    std::thread(workerMain).detach();
}

// ============================================================================================
// faults
// ============================================================================================
static void onFatal(Ctx* c, uint32_t eip, const char* what) {
    std::string chain;
    if (c->esp > 0x10000 && c->esp < kPhysSize)
        for (uint32_t a = c->esp, n = 0; a < c->esp + 0x400 && n < 12; a += 4) {
            const uint32_t v = rd32(a);
            if (v >= 0x11000 && v < 0x9D0000) {
                char b[16];
                snprintf(b, sizeof b, " %08X", v);
                chain += b;
                ++n;
            }
        }
    die("guest fault: %s at 0x%08X (eax=%08X ecx=%08X edx=%08X ebx=%08X esp=%08X ebp=%08X esi=%08X edi=%08X) stack:%s",
        what, eip, c->eax, c->ecx, c->edx, c->ebx, c->esp, c->ebp, c->esi, c->edi, chain.c_str());
}

static void onSignal(int sig, siginfo_t* si, void*) {
    const uintptr_t a = reinterpret_cast<uintptr_t>(si->si_addr), base = reinterpret_cast<uintptr_t>(g_mem);
    char msg[256];
    int n;
    if (a >= base && a < base + 0x100000000ull)
        n = snprintf(msg, sizeof msg, "\nhost signal %d: guest address 0x%08X (thread %u)\n", sig, static_cast<uint32_t>(a - base),
                     t_cur ? t_cur->id : 0);
    else
        n = snprintf(msg, sizeof msg, "\nhost signal %d at host address %p (thread %u)\n", sig, si->si_addr, t_cur ? t_cur->id : 0);
    if (write(2, msg, static_cast<size_t>(n)) < 0) {}
    void* frames[48];
    const int k = backtrace(frames, 48);
    backtrace_symbols_fd(frames, k, 2);
    _exit(3);
}

// Lifted loops call this when recomp_preempt_flag is set (every millisecond, by the ticker):
// if another thread waits for the guest lock, hand it over, as the console's timer interrupt
// would switch threads.
extern "C" void recomp_safepoint(void) {
    recomp_preempt_flag = 0;
    if (!t_cur || !g_gil.contended()) return;
    g_gil.unlock();
    g_gil.lock();
}

void coreInit() {
    recomp_on_unknown_target = onUnknownTarget;
    recomp_on_fatal = onFatal;
    struct sigaction sa{};
    sa.sa_sigaction = onSignal;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGBUS, &sa, nullptr);
    sigaction(SIGILL, &sa, nullptr);
    sigaction(SIGFPE, &sa, nullptr);
}

} // namespace xb

// Port I/O from ring-0 XBE code (PCI config, SMBus, the PIC): nothing behind it yet.
extern "C" uint32_t recomp_port_in(Ctx* c, uint32_t port, int size) {
    XLOG(2, "port in 0x%X (%d) at esp %08X", port, size, c->esp);
    if (port == 0x8008) return static_cast<uint32_t>(xb::monoTime100ns() * 3375000 / 10000000);  // ACPI timer
    return 0;
}
extern "C" void recomp_port_out(Ctx* c, uint32_t port, uint32_t value, int size) {
    XLOG(2, "port out 0x%X = 0x%X (%d) at esp %08X", port, value, size, c->esp);
}
