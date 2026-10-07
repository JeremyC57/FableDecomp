// Host side of the recompiled original-Xbox game (default.xbe): shared declarations.
//
// Guest memory is a flat 4 GiB reservation at g_mem (recomp.h). The Xbox's physical RAM
// (kPhysSize) is one shared mapping visible at three guest addresses, as on the console:
//   0x00000000  identity (the XBE image, stacks, heaps: virtual == physical here)
//   0x80000000  contiguous memory (MmAllocateContiguousMemory returns these)
//   0xF0000000  the write-combined view the GPU code uses
// Everything else in the 4 GiB (NV2A registers at 0xFD000000, APU, USB...) is plain memory:
// the GPU thread watches the registers it cares about.
//
// Guest threads are host threads, but only one runs guest code at a time (the global lock
// `g_gil`): the Xbox has one CPU and its code relies on that (IRQL raises, unlocked
// read-modify-write). Blocking kernel calls release the lock.
#pragma once

#include "recomp.h"

#include <atomic>
#include <condition_variable>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <mutex>
#include <utility>
#include <chrono>
#include <string>
#include <vector>

extern "C" {
extern void (*recomp_on_fatal)(Ctx* c, uint32_t eip, const char* what);
extern int (*recomp_on_unknown_target)(Ctx* c, uint32_t target);
int recomp_init_memory(void);
}

namespace xb {

// ---- logging -------------------------------------------------------------------------------
extern int g_logLevel;  // XBOX_LOG: 0 errors, 1 info (default), 2 kernel calls, 3 verbose
void logf(int level, const char* fmt, ...) __attribute__((format(printf, 2, 3)));
[[noreturn]] void die(const char* fmt, ...) __attribute__((format(printf, 1, 2)));
#define XLOG(lvl, ...) do { if (::xb::g_logLevel >= (lvl)) ::xb::logf((lvl), __VA_ARGS__); } while (0)

// ---- guest memory --------------------------------------------------------------------------
constexpr uint32_t kPhysSize = 128u << 20;        // a development-kit sized RAM (retail: 64 MB)
constexpr uint32_t kContigBase = 0x80000000u;
constexpr uint32_t kWcBase = 0xF0000000u;
constexpr uint32_t kTrapBase = 0xFFFC0000u;       // kernel export i -> kTrapBase + 16 * i
constexpr uint32_t kGpuInstanceSize = 0x100000u;  // top of RAM: NV2A instance memory (RAMIN)

inline uint8_t* gp(uint32_t a) { return GP(a); }
inline char* gstr(uint32_t a) { return reinterpret_cast<char*>(GP(a)); }
inline uint32_t physOf(uint32_t a) {
    if (a >= kWcBase && a < kWcBase + kPhysSize) return a - kWcBase;
    if (a >= kContigBase && a < kContigBase + kPhysSize) return a - kContigBase;
    return a;
}

void memInit();
// Page allocator over physical RAM. Contiguous allocations come from the top, virtual
// allocations (heaps, stacks) from the bottom, like the Xbox kernel. Returns a physical
// address (== the identity-mapped guest address) or 0.
uint32_t physAlloc(uint32_t size, uint32_t align, uint32_t lo, uint32_t hi, bool fromTop);
bool physAllocAt(uint32_t addr, uint32_t size);
void physFree(uint32_t addr);
uint32_t physAllocSize(uint32_t addr);  // size of the allocation starting at addr, or 0
uint32_t physFreeBytes();
// Virtual memory range (NtAllocateVirtualMemory), backed lazily by the host.
constexpr uint32_t kVaBase = 0x10000000u, kVaEnd = 0x70000000u;
uint32_t vaReserve(uint32_t base, uint32_t size, bool topDown);  // 0 on failure
std::pair<uint32_t, uint32_t> vaRegion(uint32_t a);             // {base, size} or {0, 0}
void vaDecommit(uint32_t base, uint32_t size);
bool vaRelease(uint32_t base);
// Small-block pool (ExAllocatePoolWithTag and the kernel's own objects).
uint32_t poolAlloc(uint32_t size);
void poolFree(uint32_t addr);
uint32_t poolSize(uint32_t addr);
uint32_t poolAllocZero(uint32_t size);

// ---- guest strings -------------------------------------------------------------------------
// Xbox STRING / ANSI_STRING: { USHORT Length; USHORT MaximumLength; PCHAR Buffer; }
std::string readAnsiString(uint32_t pstr);
uint32_t newAnsiString(const std::string& s);  // allocates the STRING and its buffer in the pool

// ---- threads and the global lock -----------------------------------------------------------
// Ticket lock: FIFO hand-over, so a thread giving the lock up at a safepoint really lets the
// next waiter in (std::mutex makes no such promise).
class FairLock {
public:
    void lock() {
        const uint32_t t = next_.fetch_add(1, std::memory_order_relaxed);
        for (uint32_t s = serving_.load(std::memory_order_acquire); s != t; s = serving_.load(std::memory_order_acquire))
            serving_.wait(s, std::memory_order_acquire);
    }
    void unlock() {
        serving_.fetch_add(1, std::memory_order_release);
        serving_.notify_all();
    }
    bool contended() const { return next_.load(std::memory_order_relaxed) - serving_.load(std::memory_order_relaxed) > 1; }

private:
    std::atomic<uint32_t> next_{0}, serving_{0};
};
extern FairLock g_gil;
extern std::condition_variable_any g_dispatchCv;  // notified whenever a dispatcher object changes

struct XThread {
    Ctx ctx{};
    uint32_t pcr = 0, ethread = 0, stackLo = 0, stackHi = 0, tls = 0, tlsSize = 0;
    uint32_t handle = 0, id = 0;
    struct Apc { uint32_t routine, a1, a2, a3; };
    std::vector<Apc> userApcs;
    int suspend = 0;
    bool system = false;  // a host-created kernel thread (DPC/timer worker)
};
XThread* curThread();
XThread* threadByEthread(uint32_t ethread);
// Guest thread with its own KPCR, ETHREAD, stack and TLS area (PsCreateSystemThreadEx layout).
XThread* newThread(uint32_t stackSize, uint32_t tlsSize, uint32_t extensionSize);
void startThread(XThread* t, uint32_t startRoutine, uint32_t startContext, uint32_t systemRoutine, bool suspended);
[[noreturn]] void exitCurrentThread(uint32_t status);
void bindHostThread(XThread* t);  // makes t current on this host thread (GIL must be held)

// Calls guest code `fn` (stdcall/cdecl) with args on the current thread's guest stack.
uint32_t guestCall(uint32_t fn, std::initializer_list<uint32_t> args);

// Releases the GIL around a blocking host operation.
struct GilRelease {
    GilRelease() { g_gil.unlock(); }
    ~GilRelease() { g_gil.lock(); }
};

// ---- kernel exports ------------------------------------------------------------------------
enum class CC { Std, Fast, Cdecl };
using KFn = uint32_t (*)(Ctx*);
struct KExport {
    int ordinal;
    const char* name;
    int argWords;  // stack words popped (stdcall) / read
    CC cc;
    KFn fn;
    uint32_t data;  // variable exports: guest address of the variable
};
struct KReg {  // registers by name; the ordinal comes from kernel_ordinals.inc
    KReg(const char* name, int args, CC cc, KFn fn);
    KReg(const char* name, uint32_t (*dataInit)());
};
void kernelInit();                         // creates kernel variables
uint32_t kernelResolve(int ordinal);       // thunk-table value for an import
const char* kernelName(int ordinal);

// Argument i (0-based) of a kernel call, for stdcall/cdecl (stack) functions. Fastcall
// functions get the first two in ecx/edx.
inline uint32_t ARG(Ctx* c, int i) { return rd32(c->esp + 4 + 4 * i); }

#define KCAT2(a, b) a##b
#define KCAT(a, b) KCAT2(a, b)
#define KFUNC(name, nargs)                                                         \
    static uint32_t KCAT(k_, name)(Ctx * c);                                       \
    static ::xb::KReg KCAT(kr_, name)(#name, nargs, ::xb::CC::Std, KCAT(k_, name)); \
    static uint32_t KCAT(k_, name)(Ctx * c)
#define KFAST(name, nstack)                                                         \
    static uint32_t KCAT(k_, name)(Ctx * c);                                        \
    static ::xb::KReg KCAT(kr_, name)(#name, nstack, ::xb::CC::Fast, KCAT(k_, name)); \
    static uint32_t KCAT(k_, name)(Ctx * c)
#define KCDECL(name)                                                                \
    static uint32_t KCAT(k_, name)(Ctx * c);                                        \
    static ::xb::KReg KCAT(kr_, name)(#name, 0, ::xb::CC::Cdecl, KCAT(k_, name));    \
    static uint32_t KCAT(k_, name)(Ctx * c)
#define KVAR(name)                                             \
    static uint32_t KCAT(kv_, name)();                         \
    static ::xb::KReg KCAT(kr_, name)(#name, KCAT(kv_, name)); \
    static uint32_t KCAT(kv_, name)()

// ---- NT status codes -----------------------------------------------------------------------
enum : uint32_t {
    ST_SUCCESS = 0, ST_TIMEOUT = 0x102, ST_PENDING = 0x103, ST_USER_APC = 0xC0, ST_ALERTED = 0x101,
    ST_BUFFER_OVERFLOW = 0x80000005, ST_NO_MORE_FILES = 0x80000006,
    ST_UNSUCCESSFUL = 0xC0000001, ST_NOT_IMPLEMENTED = 0xC0000002, ST_INVALID_HANDLE = 0xC0000008,
    ST_INVALID_PARAMETER = 0xC000000D, ST_NO_SUCH_FILE = 0xC000000F, ST_END_OF_FILE = 0xC0000011,
    ST_NO_MEMORY = 0xC0000017, ST_ACCESS_DENIED = 0xC0000022, ST_BUFFER_TOO_SMALL = 0xC0000023,
    ST_OBJECT_TYPE_MISMATCH = 0xC0000024, ST_OBJECT_NAME_INVALID = 0xC0000033,
    ST_OBJECT_NAME_NOT_FOUND = 0xC0000034, ST_OBJECT_NAME_COLLISION = 0xC0000035,
    ST_OBJECT_PATH_NOT_FOUND = 0xC000003A, ST_SEMAPHORE_LIMIT_EXCEEDED = 0xC0000047,
    ST_MUTANT_NOT_OWNED = 0xC0000046, ST_FILE_IS_A_DIRECTORY = 0xC00000BA,
    ST_NOT_A_DIRECTORY = 0xC0000103, ST_DEVICE_NOT_READY = 0xC00000A3,
    ST_DIRECTORY_NOT_EMPTY = 0xC0000101, ST_INVALID_DEVICE_REQUEST = 0xC0000010,
};

uint32_t ntToDos(uint32_t status);

// ---- objects and handles -------------------------------------------------------------------
struct HostFile;
struct Handle {
    enum Kind { None, Dispatcher, File, SymLink, Directory } kind = None;
    uint32_t object = 0;  // guest address (dispatcher objects, ETHREAD)
    HostFile* file = nullptr;
    std::string link;     // symbolic-link target / directory name
};
uint32_t handleNew(const Handle& h);
Handle* handleGet(uint32_t h);
bool handleClose(uint32_t h);
std::string lastWmaOpened();  // host path of the last .wma file the calling guest thread opened (music.cpp)
uint32_t fileObjectFor(Handle* h);  // guest FILE_OBJECT of a file handle (files.cpp)
uint32_t handleDup(uint32_t h);
uint32_t namedObjectFind(const std::string& name);
void namedObjectAdd(const std::string& name, uint32_t handle);

// Dispatcher objects live in guest memory (DISPATCHER_HEADER: Type, Absolute, Size,
// Inserted, SignalState@4, WaitListHead@8). Waits block on g_dispatchCv.
enum : uint8_t { DO_NotifEvent = 0, DO_SyncEvent = 1, DO_Mutant = 2, DO_Semaphore = 5, DO_Thread = 6,
                 DO_NotifTimer = 8, DO_SyncTimer = 9 };
uint32_t waitObjects(Ctx* c, const uint32_t* objs, int n, bool waitAll, bool alertable, uint32_t timeoutPtr);
void signalObject(uint32_t obj);  // after changing SignalState: wake waiters
uint32_t keSetEvent(uint32_t ev);
uint64_t systemTime100ns();      // FILETIME-style system time
uint64_t monoTime100ns();
void deliverApcs(Ctx* c);        // runs pending user APCs of the current thread

// ---- kernel worker: timers, DPCs, interrupts -----------------------------------------------
void workerStart();
void timerSet(uint32_t timer, int64_t dueTime, int32_t periodMs, uint32_t dpc);
bool timerCancel(uint32_t timer);
bool dpcQueue(uint32_t dpc, uint32_t arg1, uint32_t arg2);
bool dpcRemove(uint32_t dpc);
void interruptConnect(uint32_t vector, uint32_t kinterrupt);
void interruptRaise(uint32_t vector);  // runs the connected ISR on the worker (GPU vblank etc.)
void workerAddService(void (*fn)());   // (GIL held) fn runs on the worker, GIL held, every few ms
void workerKick();                     // wakes the worker early

// Guest-callable address for a host function (stdcall, `args` stack words), for vtables of
// HLE objects. The function receives the guest Ctx; its return value goes to eax.
uint32_t hostTrap(KFn fn, int args, const char* name);

// Debugging: log the host backtrace of writes to one guest word (core.cpp).
void debugWatch(uint32_t guestAddr);
void debugWatchRearm();  // the ticker calls it
void debugReadWatchRearm();  // FABLE_RWATCH (core.cpp)
void profilerStart();     // FABLE_PROFILE=[<delay>,]<seconds> (profiler.cpp)

// ---- files ---------------------------------------------------------------------------------
void filesInit(const std::string& gameDir, const std::string& hddDir);
// FABLE_DISABLE=name1,name2: turns individual emulation fixes off (bisecting regressions).
bool featureOff(const char* name);
void perfFlip(std::chrono::steady_clock::time_point now);  // nv2a.cpp: a flip happened
void perfStats(float* fps, float* worstMs);
bool profilerCapture(double delay, double secs, const std::string& path);  // profiler.cpp
void setThreadName(const char* name);  // for profiles and debuggers (15 characters)                   // frames/s and slowest frame (ms), last 0.5 s
uint64_t gameClockNs();  // RDTSC / performance counter clock (FABLE_FLIPTIME: per flip)
std::string resolveObjectName(uint32_t objectAttributes);  // full object path ("\Device\CdRom0\...")
void symlinkCreate(const std::string& link, const std::string& target);
bool symlinkDelete(const std::string& link);
bool symlinkQuery(const std::string& link, std::string& target);

// ---- XBE image -----------------------------------------------------------------------------
extern std::vector<uint8_t> g_xbe;  // the file
uint32_t xbeLoadSection(uint32_t sectionHeader);
uint32_t xbeUnloadSection(uint32_t sectionHeader);

// ---- HLE patches over guest library functions (XAPI input, ...) ---------------------------
void hleInstall();

} // namespace xb
