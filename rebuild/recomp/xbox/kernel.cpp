// xboxkrnl.exe exports used by Fable: memory, synchronisation, threads, strings, time,
// hardware queries, crypto. Files and symbolic links are in files.cpp.
#include "settings.hpp"
#include "xhost.hpp"

#include <algorithm>
#include <chrono>
#include <map>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>
#include <ctime>
#include <thread>

namespace xb {

// ============================================================================================
// kernel variables
// ============================================================================================
static uint32_t g_tickCount;  // guest address of KeTickCount

KVAR(KeTickCount) { return g_tickCount = poolAllocZero(4); }
KVAR(XboxHardwareInfo) {
    const uint32_t a = poolAllocZero(8);
    wr32(a, 0x00000020);  // flags: retail unit
    wr8(a + 4, 0xD4);     // GPU revision (NV2A A3)
    wr8(a + 5, 0xD4);     // MCPX revision
    return a;
}
KVAR(XboxKrnlVersion) {
    const uint32_t a = poolAllocZero(8);
    wr16(a, 1); wr16(a + 2, 0); wr16(a + 4, 5838); wr16(a + 6, 1);
    return a;
}
KVAR(LaunchDataPage) { return poolAllocZero(4); }  // PLAUNCH_DATA_PAGE: none (cold boot)
KVAR(XeImageFileName) { return newAnsiString("\\Device\\CdRom0\\default.xbe"); }
KVAR(HalDiskCachePartitionCount) { const uint32_t a = poolAllocZero(4); wr32(a, 3); return a; }
KVAR(HalDiskModelNumber) { return newAnsiString("FableRecomp HDD"); }
KVAR(HalDiskSerialNumber) { return newAnsiString("0000000000000"); }
KVAR(HalBootSMCVideoMode) { return poolAllocZero(4); }
static uint32_t objectType(const char* tag) {
    const uint32_t a = poolAllocZero(0x20);
    std::memcpy(gp(a + 0x1C), tag, 4);
    return a;
}
KVAR(ExEventObjectType) { return objectType("Even"); }
KVAR(PsThreadObjectType) { return objectType("Thre"); }
KVAR(IoFileObjectType) { return objectType("File"); }
KVAR(IdexChannelObject) { return poolAllocZero(0x40); }
KVAR(XboxHDKey) { return poolAllocZero(16); }
KVAR(XboxSignatureKey) { return poolAllocZero(16); }
KVAR(XboxAlternateSignatureKeys) { return poolAllocZero(16 * 16); }
KVAR(XePublicKeyData) { return poolAllocZero(284); }
KVAR(XboxLANKey) { return poolAllocZero(16); }

void kernelTick() {
    if (g_tickCount) wr32(g_tickCount, static_cast<uint32_t>(monoTime100ns() / 10000));
}

// ============================================================================================
// memory
// ============================================================================================
static uint32_t contig(uint32_t size, uint32_t lo, uint32_t hi, uint32_t align) {
    const uint32_t p = physAlloc(size, std::max<uint32_t>(align, 0x1000), lo, hi, true);
    if (!p) {
        XLOG(0, "MmAllocateContiguousMemory(0x%X): out of memory (%u KB free)", size, physFreeBytes() / 1024);
        return 0;
    }
    std::memset(gp(p), 0, size);
    return kContigBase + p;
}
KFUNC(MmAllocateContiguousMemory, 1) { return contig(ARG(c, 0), 0, kPhysSize - 1, 0x1000); }
KFUNC(MmAllocateContiguousMemoryEx, 5) { return contig(ARG(c, 0), ARG(c, 1), ARG(c, 2), ARG(c, 3)); }
KFUNC(MmFreeContiguousMemory, 1) { physFree(ARG(c, 0)); return 0; }
KFUNC(MmGetPhysicalAddress, 1) { return physOf(ARG(c, 0)); }
KFUNC(MmQueryAllocationSize, 1) { return physAllocSize(ARG(c, 0)); }
KFUNC(MmPersistContiguousMemory, 3) { return 0; }
KFUNC(MmSetAddressProtect, 3) { return 0; }
KFUNC(MmQueryAddressProtect, 1) { return 0x04; }  // PAGE_READWRITE
KFUNC(MmLockUnlockBufferPages, 3) { return 0; }
KFUNC(MmLockUnlockPhysicalPage, 2) { return 0; }
KFUNC(MmQueryStatistics, 1) {
    const uint32_t s = ARG(c, 0);
    if (!s || rd32(s) != 0x24) return ST_INVALID_PARAMETER;
    wr32(s + 4, kPhysSize / 0x1000);           // TotalPhysicalPages
    wr32(s + 8, physFreeBytes() / 0x1000);     // AvailablePages
    for (uint32_t o = 12; o < 0x24; o += 4) wr32(s + o, 0);
    return ST_SUCCESS;
}
KFUNC(MmClaimGpuInstanceMemory, 2) {
    // Returns the end of the GPU instance memory; the GPU code works downwards from here.
    if (ARG(c, 1)) wr32(ARG(c, 1), 0);  // NumberOfPaddingBytes
    return kContigBase + kPhysSize;
}

KFUNC(NtAllocateVirtualMemory, 5) {
    const uint32_t pBase = ARG(c, 0), pSize = ARG(c, 2), type = ARG(c, 3);
    const uint32_t base = rd32(pBase) & ~0xFFFu;
    const uint32_t size = ((rd32(pBase) & 0xFFFu) + rd32(pSize) + 0xFFF) & ~0xFFFu;
    if (!size) return ST_INVALID_PARAMETER;
    if (base) {
        const auto [rb, rs] = vaRegion(base);
        if (rb) {  // committing (or re-reserving) pages inside an existing reservation
            if (base + size > rb + rs) return ST_INVALID_PARAMETER;
            wr32(pBase, base);
            wr32(pSize, size);
            return ST_SUCCESS;
        }
        if (!(type & 0x2000)) return ST_INVALID_PARAMETER;  // commit without a reservation
    }
    const uint32_t a = vaReserve(base, size, (type & 0x100000) != 0);  // MEM_TOP_DOWN
    if (!a) {
        XLOG(1, "NtAllocateVirtualMemory(base 0x%08X, 0x%X): no address space", base, size);
        return ST_NO_MEMORY;
    }
    wr32(pBase, a);
    wr32(pSize, size);
    return ST_SUCCESS;
}
KFUNC(NtFreeVirtualMemory, 3) {
    const uint32_t pBase = ARG(c, 0), pSize = ARG(c, 1), type = ARG(c, 2);
    const uint32_t base = rd32(pBase) & ~0xFFFu;
    if (type & 0x8000) {  // MEM_RELEASE
        const auto [rb, rs] = vaRegion(base);
        if (!rb || rb != base) return ST_INVALID_PARAMETER;
        wr32(pSize, rs);
        vaRelease(base);
        return ST_SUCCESS;
    }
    // MEM_DECOMMIT
    uint32_t size = (rd32(pSize) + 0xFFF) & ~0xFFFu;
    const auto [rb, rs] = vaRegion(base);
    if (!rb) return ST_INVALID_PARAMETER;
    if (!size) size = rb + rs - base;
    vaDecommit(base, size);
    return ST_SUCCESS;
}
KFUNC(NtQueryVirtualMemory, 2) {
    const uint32_t a = ARG(c, 0) & ~0xFFFu, mbi = ARG(c, 1);
    const auto [rb, rs] = vaRegion(a);
    wr32(mbi + 0, a);
    wr32(mbi + 4, rb ? rb : a);
    wr32(mbi + 8, 0x04);
    wr32(mbi + 12, rb ? rb + rs - a : 0x1000);
    wr32(mbi + 16, rb ? 0x1000 : 0x10000);  // MEM_COMMIT / MEM_FREE
    wr32(mbi + 20, rb ? 0x04 : 0x01);
    wr32(mbi + 24, rb ? 0x20000 : 0);       // MEM_PRIVATE
    return ST_SUCCESS;
}

KFUNC(ExAllocatePoolWithTag, 2) { return poolAlloc(ARG(c, 0)); }
KFUNC(ExFreePool, 1) { poolFree(ARG(c, 0)); return 0; }
KFUNC(ExQueryPoolBlockSize, 1) { return poolSize(ARG(c, 0)); }

// ============================================================================================
// IRQL, interrupts, DPCs, timers
// ============================================================================================
static uint32_t irqlAddr() { return curThread()->pcr + 0x24; }
KFAST(KfRaiseIrql, 0) {
    const uint32_t old = rd8(irqlAddr());
    wr8(irqlAddr(), static_cast<uint8_t>(c->ecx));
    return old;
}
KFAST(KfLowerIrql, 0) { wr8(irqlAddr(), static_cast<uint8_t>(c->ecx)); return 0; }
KFUNC(KeRaiseIrqlToDpcLevel, 0) {
    const uint32_t old = rd8(irqlAddr());
    wr8(irqlAddr(), 2);
    return old;
}
KFUNC(HalGetInterruptVector, 2) {
    const uint32_t level = ARG(c, 0);
    if (ARG(c, 1)) wr8(ARG(c, 1), static_cast<uint8_t>(26 - level));
    return 0x30 + level;
}
KFUNC(KeInitializeInterrupt, 7) {
    const uint32_t ki = ARG(c, 0);
    wr32(ki + 0x00, ARG(c, 1));  // ServiceRoutine
    wr32(ki + 0x04, ARG(c, 2));  // ServiceContext
    wr32(ki + 0x08, ARG(c, 3) - 0x30);
    wr32(ki + 0x0C, ARG(c, 4));
    wr8(ki + 0x10, 0);
    wr8(ki + 0x11, static_cast<uint8_t>(ARG(c, 6)));
    wr32(ki + 0x12, ARG(c, 5));
    return 0;
}
KFUNC(KeConnectInterrupt, 1) {
    const uint32_t ki = ARG(c, 0);
    if (rd8(ki + 0x10)) return 0;
    wr8(ki + 0x10, 1);
    interruptConnect(rd32(ki + 0x08), ki);
    XLOG(1, "interrupt %u connected (ISR 0x%08X)", rd32(ki + 0x08), rd32(ki));
    return 1;
}
KFUNC(KeDisconnectInterrupt, 1) { wr8(ARG(c, 0) + 0x10, 0); return 1; }
KFUNC(KeSynchronizeExecution, 3) { return guestCall(ARG(c, 1), {ARG(c, 2)}); }

KFUNC(KeInitializeDpc, 3) {
    const uint32_t d = ARG(c, 0);
    wr16(d, 0x13);
    wr8(d + 2, 0);
    wr32(d + 0x0C, ARG(c, 1));
    wr32(d + 0x10, ARG(c, 2));
    return 0;
}
KFUNC(KeInsertQueueDpc, 3) { return dpcQueue(ARG(c, 0), ARG(c, 1), ARG(c, 2)); }
KFUNC(KeRemoveQueueDpc, 1) { return dpcRemove(ARG(c, 0)); }

KFUNC(KeInitializeTimerEx, 2) {
    const uint32_t t = ARG(c, 0);
    std::memset(gp(t), 0, 0x28);
    wr8(t, static_cast<uint8_t>(DO_NotifTimer + ARG(c, 1)));
    wr8(t + 2, 0x28 / 4);
    wr32(t + 8, t + 8);
    wr32(t + 12, t + 8);
    return 0;
}
KFUNC(KeSetTimer, 4) {
    const uint32_t t = ARG(c, 0);
    const bool was = rd8(t + 3) != 0;
    timerSet(t, static_cast<int64_t>(static_cast<uint64_t>(ARG(c, 1)) | (static_cast<uint64_t>(ARG(c, 2)) << 32)), 0, ARG(c, 3));
    return was;
}
KFUNC(KeSetTimerEx, 5) {
    const uint32_t t = ARG(c, 0);
    const bool was = rd8(t + 3) != 0;
    timerSet(t, static_cast<int64_t>(static_cast<uint64_t>(ARG(c, 1)) | (static_cast<uint64_t>(ARG(c, 2)) << 32)),
             static_cast<int32_t>(ARG(c, 3)), ARG(c, 4));
    return was;
}
KFUNC(KeCancelTimer, 1) { return timerCancel(ARG(c, 0)); }

// ============================================================================================
// events, semaphores, mutants, waits
// ============================================================================================
KFUNC(KeSetEvent, 3) { return keSetEvent(ARG(c, 0)); }

static uint32_t newDispatcher(uint8_t type, uint32_t size, int32_t signal) {
    const uint32_t o = poolAllocZero(size);
    wr8(o, type);
    wr8(o + 2, static_cast<uint8_t>(size / 4));
    wr32(o + 4, static_cast<uint32_t>(signal));
    wr32(o + 8, o + 8);
    wr32(o + 12, o + 8);
    return o;
}
static uint32_t objectOf(uint32_t h) {
    if (h == 0xFFFFFFFEu) return curThread()->ethread;  // NtCurrentThread()
    Handle* x = handleGet(h);
    return x && x->kind == Handle::Dispatcher ? x->object : 0;
}
// Named objects: ObjectAttributes with a name opens the existing object.
static uint32_t createNamed(uint32_t pHandle, uint32_t objAttr, uint32_t object, bool& existed) {
    existed = false;
    std::string name;
    if (objAttr && rd32(objAttr + 4)) name = readAnsiString(rd32(objAttr + 4));
    if (!name.empty()) {
        if (uint32_t h = namedObjectFind(name)) {
            wr32(pHandle, handleDup(h));
            existed = true;
            return 0x40000000;  // STATUS_OBJECT_NAME_EXISTS
        }
    }
    Handle h;
    h.kind = Handle::Dispatcher;
    h.object = object;
    const uint32_t v = handleNew(h);
    if (!name.empty()) namedObjectAdd(name, v);
    wr32(pHandle, v);
    return ST_SUCCESS;
}
KFUNC(NtCreateEvent, 4) {
    bool existed;
    return createNamed(ARG(c, 0), ARG(c, 1), newDispatcher(static_cast<uint8_t>(ARG(c, 2)), 0x10, ARG(c, 3) & 0xFF), existed);
}
KFUNC(NtSetEvent, 2) {
    const uint32_t o = objectOf(ARG(c, 0));
    if (!o) return ST_INVALID_HANDLE;
    const uint32_t old = keSetEvent(o);
    if (ARG(c, 1)) wr32(ARG(c, 1), old);
    return ST_SUCCESS;
}
KFUNC(NtClearEvent, 1) {
    const uint32_t o = objectOf(ARG(c, 0));
    if (!o) return ST_INVALID_HANDLE;
    wr32(o + 4, 0);
    return ST_SUCCESS;
}
KFUNC(NtCreateSemaphore, 4) {
    const uint32_t o = newDispatcher(DO_Semaphore, 0x14, static_cast<int32_t>(ARG(c, 2)));
    wr32(o + 0x10, ARG(c, 3));
    bool existed;
    return createNamed(ARG(c, 0), ARG(c, 1), o, existed);
}
KFUNC(NtReleaseSemaphore, 3) {
    const uint32_t o = objectOf(ARG(c, 0));
    if (!o) return ST_INVALID_HANDLE;
    const int32_t prev = static_cast<int32_t>(rd32(o + 4)), add = static_cast<int32_t>(ARG(c, 1));
    if (prev + add > static_cast<int32_t>(rd32(o + 0x10))) return ST_SEMAPHORE_LIMIT_EXCEEDED;
    wr32(o + 4, static_cast<uint32_t>(prev + add));
    if (ARG(c, 2)) wr32(ARG(c, 2), static_cast<uint32_t>(prev));
    g_dispatchCv.notify_all();
    return ST_SUCCESS;
}
KFUNC(NtCreateMutant, 3) {
    const bool owned = ARG(c, 2) & 0xFF;
    const uint32_t o = newDispatcher(DO_Mutant, 0x20, owned ? 0 : 1);
    if (owned) wr32(o + 0x18, curThread()->ethread);
    bool existed;
    return createNamed(ARG(c, 0), ARG(c, 1), o, existed);
}
KFUNC(NtReleaseMutant, 2) {
    const uint32_t o = objectOf(ARG(c, 0));
    if (!o) return ST_INVALID_HANDLE;
    if (rd32(o + 0x18) != curThread()->ethread) return ST_MUTANT_NOT_OWNED;
    const int32_t prev = static_cast<int32_t>(rd32(o + 4));
    wr32(o + 4, static_cast<uint32_t>(prev + 1));
    if (prev + 1 > 0) wr32(o + 0x18, 0);
    if (ARG(c, 1)) wr32(ARG(c, 1), static_cast<uint32_t>(prev));
    g_dispatchCv.notify_all();
    return ST_SUCCESS;
}

static uint32_t waitHandle(Ctx* c, uint32_t h, bool alertable, uint32_t timeout) {
    const uint32_t o = objectOf(h);
    if (!o) {
        Handle* x = handleGet(h);
        if (x && x->kind == Handle::File) return ST_SUCCESS;  // file handles: I/O is synchronous
        return ST_INVALID_HANDLE;
    }
    return waitObjects(c, &o, 1, false, alertable, timeout);
}
// FABLE_WAIT_LOG=1 (debugging): where guest threads block, by call site, every 5 s.
// First game-code return address on the stack above the XAPI wrappers (heuristic: a .text
// address preceded by a call instruction).
uint32_t stackCaller(Ctx* c) {
    for (uint32_t a = c->esp + 4; a < c->esp + 0x200; a += 4) {
        const uint32_t v = rd32(a);
        if (v < 0x13005 || v >= 0x612000 || (v >= 0x1EB000 && v < 0x1EC000)) continue;
        if (rd8(v - 5) == 0xE8 || rd8(v - 6) == 0xFF || rd8(v - 2) == 0xFF || rd8(v - 3) == 0xFF) return v;
    }
    return 0;
}

struct WaitProbe {
    const char* name;
    uint32_t r1, r2, tid;
    std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
    WaitProbe(Ctx* c, const char* n) : name(n), r1(rd32(c->esp)), r2(stackCaller(c)), tid(static_cast<uint32_t>(reinterpret_cast<uintptr_t>(curThread()))) {}
    ~WaitProbe() {
        static const bool on = getenv("FABLE_WAIT_LOG") != nullptr;
        if (!on) return;
        static std::mutex m;
        static std::map<std::tuple<std::string, uint32_t, uint32_t, uint32_t>, std::pair<double, uint64_t>> h;
        static auto last = std::chrono::steady_clock::now();
        const auto now = std::chrono::steady_clock::now();
        std::lock_guard<std::mutex> l(m);
        auto& e = h[{name, r1, r2, tid}];
        e.first += std::chrono::duration<double, std::milli>(now - t0).count();
        ++e.second;
        if (now - last < std::chrono::seconds(5)) return;
        last = now;
        std::vector<std::pair<double, std::string>> v;
        for (auto& [k, x] : h) {
            char b[160];
            std::snprintf(b, sizeof b, "%s ret %08X/%08X thread %08X: %.0f ms in %llu calls", std::get<0>(k).c_str(), std::get<1>(k), std::get<2>(k), std::get<3>(k), x.first,
                          static_cast<unsigned long long>(x.second));
            v.push_back({x.first, b});
        }
        std::sort(v.rbegin(), v.rend());
        for (size_t i = 0; i < v.size() && i < 8; ++i) XLOG(0, "wait: %s", v[i].second.c_str());
        h.clear();
    }
};

KFUNC(NtWaitForSingleObject, 3) { WaitProbe p(c, "NtWaitForSingleObject"); return waitHandle(c, ARG(c, 0), ARG(c, 1) & 0xFF, ARG(c, 2)); }
KFUNC(NtWaitForSingleObjectEx, 4) { WaitProbe p(c, "NtWaitForSingleObjectEx"); return waitHandle(c, ARG(c, 0), ARG(c, 2) & 0xFF, ARG(c, 3)); }
KFUNC(KeWaitForSingleObject, 5) {
    WaitProbe p(c, "KeWaitForSingleObject");
    const uint32_t o = ARG(c, 0);
    return waitObjects(c, &o, 1, false, ARG(c, 3) & 0xFF, ARG(c, 4));
}
KFUNC(KeWaitForMultipleObjects, 8) {
    WaitProbe p(c, "KeWaitForMultipleObjects");
    const uint32_t n = ARG(c, 0), arr = ARG(c, 1);
    std::vector<uint32_t> objs(n);
    for (uint32_t i = 0; i < n; ++i) objs[i] = rd32(arr + 4 * i);
    return waitObjects(c, objs.data(), static_cast<int>(n), ARG(c, 2) == 0, ARG(c, 5) & 0xFF, ARG(c, 6));
}
KFUNC(KeDelayExecutionThread, 3) {
    WaitProbe p(c, "KeDelayExecutionThread");
    const bool alertable = ARG(c, 1) & 0xFF;
    const int64_t v = static_cast<int64_t>(rd64(ARG(c, 2)));
    XThread* t = curThread();
    if (alertable && !t->userApcs.empty()) {
        deliverApcs(c);
        return ST_USER_APC;
    }
    uint64_t ns100 = v < 0 ? static_cast<uint64_t>(-v) : 0;
    if (v > 0) {
        const uint64_t sys = systemTime100ns();
        ns100 = static_cast<uint64_t>(v) > sys ? static_cast<uint64_t>(v) - sys : 0;
    }
    GilRelease r;
    if (ns100) std::this_thread::sleep_for(std::chrono::nanoseconds(ns100 * 100));
    else std::this_thread::yield();
    return ST_SUCCESS;
}
KFUNC(NtYieldExecution, 0) {
    GilRelease r;
    std::this_thread::yield();
    return ST_SUCCESS;
}

// Critical sections embed a synchronization event (RTL_CRITICAL_SECTION: Event[0x10],
// LockCount@0x10, RecursionCount@0x14, OwningThread@0x18).
KFUNC(RtlInitializeCriticalSection, 1) {
    const uint32_t cs = ARG(c, 0);
    std::memset(gp(cs), 0, 0x1C);
    wr8(cs, DO_SyncEvent);
    wr8(cs + 2, 4);
    wr32(cs + 8, cs + 8);
    wr32(cs + 12, cs + 8);
    wr32(cs + 0x10, 0xFFFFFFFFu);
    return 0;
}
KFUNC(RtlEnterCriticalSection, 1) {
    const uint32_t cs = ARG(c, 0), me = curThread()->ethread;
    const int32_t lc = static_cast<int32_t>(rd32(cs + 0x10)) + 1;
    wr32(cs + 0x10, static_cast<uint32_t>(lc));
    if (lc == 0) {
        wr32(cs + 0x18, me);
        wr32(cs + 0x14, 1);
    } else if (rd32(cs + 0x18) == me) {
        wr32(cs + 0x14, rd32(cs + 0x14) + 1);
    } else {
        waitObjects(c, &cs, 1, false, false, 0);
        wr32(cs + 0x18, me);
        wr32(cs + 0x14, 1);
    }
    return 0;
}
KFUNC(RtlTryEnterCriticalSection, 1) {
    const uint32_t cs = ARG(c, 0), me = curThread()->ethread;
    if (static_cast<int32_t>(rd32(cs + 0x10)) == -1) {
        wr32(cs + 0x10, 0);
        wr32(cs + 0x18, me);
        wr32(cs + 0x14, 1);
        return 1;
    }
    if (rd32(cs + 0x18) == me) {
        wr32(cs + 0x10, rd32(cs + 0x10) + 1);
        wr32(cs + 0x14, rd32(cs + 0x14) + 1);
        return 1;
    }
    return 0;
}
KFUNC(RtlLeaveCriticalSection, 1) {
    const uint32_t cs = ARG(c, 0);
    const uint32_t rec = rd32(cs + 0x14) - 1;
    wr32(cs + 0x14, rec);
    const int32_t lc = static_cast<int32_t>(rd32(cs + 0x10)) - 1;
    wr32(cs + 0x10, static_cast<uint32_t>(lc));
    if (rec == 0) {
        wr32(cs + 0x18, 0);
        if (lc >= 0) keSetEvent(cs);
    }
    return 0;
}

// ============================================================================================
// threads and objects
// ============================================================================================
KFUNC(PsCreateSystemThreadEx, 10) {
    const uint32_t pHandle = ARG(c, 0), ext = ARG(c, 1), stack = ARG(c, 2), tls = ARG(c, 3), pId = ARG(c, 4);
    XThread* t = newThread(stack, tls, ext);
    Handle h;
    h.kind = Handle::Dispatcher;
    h.object = t->ethread;
    t->handle = handleNew(h);
    if (pHandle) wr32(pHandle, t->handle);
    if (pId) wr32(pId, t->id);
    startThread(t, ARG(c, 5), ARG(c, 6), ARG(c, 9), ARG(c, 7) & 0xFF);
    return ST_SUCCESS;
}
KFUNC(PsTerminateSystemThread, 1) { exitCurrentThread(ARG(c, 0)); }
KFUNC(NtResumeThread, 2) {
    const uint32_t o = objectOf(ARG(c, 0));
    XThread* t = o ? threadByEthread(o) : nullptr;
    if (!t) return ST_INVALID_HANDLE;
    if (ARG(c, 1)) wr32(ARG(c, 1), static_cast<uint32_t>(t->suspend));
    if (t->suspend > 0) --t->suspend;
    g_dispatchCv.notify_all();
    return ST_SUCCESS;
}
KFUNC(KeSetBasePriorityThread, 2) {
    const uint32_t old = static_cast<int8_t>(rd8(ARG(c, 0) + 0x70));
    wr8(ARG(c, 0) + 0x70, static_cast<uint8_t>(ARG(c, 1)));
    return old;
}
KFUNC(ObReferenceObjectByHandle, 3) {
    const uint32_t h = ARG(c, 0), out = ARG(c, 2);
    uint32_t o = objectOf(h);
    if (!o) {
        Handle* x = handleGet(h);
        if (!x) return ST_INVALID_HANDLE;
        o = x->kind == Handle::File ? fileObjectFor(x) : x->object;
    }
    wr32(out, o);
    return ST_SUCCESS;
}
KFAST(ObfDereferenceObject, 0) { return 0; }
KFUNC(ObReferenceObjectByName, 5) { return ST_OBJECT_NAME_NOT_FOUND; }
KFUNC(NtDuplicateObject, 3) {
    const uint32_t h = ARG(c, 0) == 0xFFFFFFFEu ? curThread()->handle : ARG(c, 0);
    const uint32_t d = handleDup(h);
    if (!d) return ST_INVALID_HANDLE;
    wr32(ARG(c, 1), d);
    if (ARG(c, 2) & 1) handleClose(h);  // DUPLICATE_CLOSE_SOURCE
    return ST_SUCCESS;
}

// ============================================================================================
// strings and runtime
// ============================================================================================
KFUNC(RtlInitAnsiString, 2) {
    const uint32_t s = ARG(c, 0), src = ARG(c, 1);
    const uint32_t n = src ? static_cast<uint32_t>(strlen(gstr(src))) : 0;
    wr16(s, static_cast<uint16_t>(n));
    wr16(s + 2, static_cast<uint16_t>(src ? n + 1 : 0));
    wr32(s + 4, src);
    return 0;
}
KFUNC(RtlInitUnicodeString, 2) {
    const uint32_t s = ARG(c, 0), src = ARG(c, 1);
    uint32_t n = 0;
    if (src)
        while (rd16(src + 2 * n)) ++n;
    wr16(s, static_cast<uint16_t>(2 * n));
    wr16(s + 2, static_cast<uint16_t>(src ? 2 * n + 2 : 0));
    wr32(s + 4, src);
    return 0;
}
KFUNC(RtlEqualString, 3) {
    std::string a = readAnsiString(ARG(c, 0)), b = readAnsiString(ARG(c, 1));
    if (a.size() != b.size()) return 0;
    if (ARG(c, 2) & 0xFF) return strncasecmp(a.data(), b.data(), a.size()) == 0;
    return a == b;
}
KFUNC(RtlFreeAnsiString, 1) {
    const uint32_t s = ARG(c, 0);
    if (rd32(s + 4)) poolFree(rd32(s + 4));
    wr32(s + 4, 0);
    wr16(s, 0);
    wr16(s + 2, 0);
    return 0;
}
KFUNC(RtlUnicodeStringToAnsiString, 3) {
    const uint32_t dst = ARG(c, 0), src = ARG(c, 1);
    const uint32_t n = rd16(src) / 2, buf = rd32(src + 4);
    if (ARG(c, 2) & 0xFF) {
        wr32(dst + 4, poolAlloc(n + 1));
        wr16(dst + 2, static_cast<uint16_t>(n + 1));
    } else if (rd16(dst + 2) < n + 1) {
        return ST_BUFFER_OVERFLOW;
    }
    const uint32_t out = rd32(dst + 4);
    for (uint32_t i = 0; i < n; ++i) {
        const uint16_t ch = rd16(buf + 2 * i);
        wr8(out + i, ch < 0x100 ? static_cast<uint8_t>(ch) : '?');
    }
    wr8(out + n, 0);
    wr16(dst, static_cast<uint16_t>(n));
    return ST_SUCCESS;
}
KFUNC(RtlAnsiStringToUnicodeString, 3) {
    const uint32_t dst = ARG(c, 0), src = ARG(c, 1);
    const uint32_t n = rd16(src), buf = rd32(src + 4);
    if (ARG(c, 2) & 0xFF) {
        wr32(dst + 4, poolAlloc(2 * n + 2));
        wr16(dst + 2, static_cast<uint16_t>(2 * n + 2));
    } else if (rd16(dst + 2) < 2 * n + 2) {
        return ST_BUFFER_OVERFLOW;
    }
    const uint32_t out = rd32(dst + 4);
    for (uint32_t i = 0; i < n; ++i) wr16(out + 2 * i, rd8(buf + i));
    wr16(out + 2 * n, 0);
    wr16(dst, static_cast<uint16_t>(2 * n));
    return ST_SUCCESS;
}
KFUNC(RtlCompareMemoryUlong, 3) {
    const uint32_t src = ARG(c, 0), len = ARG(c, 1) & ~3u, pat = ARG(c, 2);
    uint32_t i = 0;
    while (i < len && rd32(src + i) == pat) i += 4;
    return i;
}
uint32_t ntToDos(uint32_t status) {
    switch (status) {
    case ST_SUCCESS: return 0;
    case ST_OBJECT_NAME_NOT_FOUND: case ST_NO_SUCH_FILE: return 2;  // ERROR_FILE_NOT_FOUND
    case ST_OBJECT_PATH_NOT_FOUND: return 3;
    case ST_ACCESS_DENIED: return 5;
    case ST_INVALID_HANDLE: return 6;
    case ST_NO_MEMORY: return 8;
    case ST_END_OF_FILE: return 38;  // ERROR_HANDLE_EOF
    case ST_NOT_IMPLEMENTED: return 50;
    case ST_OBJECT_NAME_COLLISION: return 183;  // ERROR_ALREADY_EXISTS
    case ST_INVALID_PARAMETER: return 87;
    case ST_BUFFER_OVERFLOW: return 234;
    case ST_NO_MORE_FILES: return 18;
    case ST_PENDING: return 997;
    case ST_TIMEOUT: return 1460;
    case ST_DEVICE_NOT_READY: return 21;
    case ST_DIRECTORY_NOT_EMPTY: return 145;
    default: return 317;  // ERROR_MR_MID_NOT_FOUND
    }
}
KFUNC(RtlNtStatusToDosError, 1) { return ntToDos(ARG(c, 0)); }
// TIME_FIELDS: Year, Month, Day, Hour, Minute, Second, Milliseconds, Weekday (USHORTs)
KFUNC(RtlTimeFieldsToTime, 2) {
    const uint32_t f = ARG(c, 0);
    struct tm t{};
    t.tm_year = rd16(f) - 1900;
    t.tm_mon = rd16(f + 2) - 1;
    t.tm_mday = rd16(f + 4);
    t.tm_hour = rd16(f + 6);
    t.tm_min = rd16(f + 8);
    t.tm_sec = rd16(f + 10);
    const int64_t secs = timegm(&t);
    wr64(ARG(c, 1), static_cast<uint64_t>(secs * 10000000 + rd16(f + 12) * 10000) + 116444736000000000ull);
    return 1;
}
KFUNC(RtlTimeToTimeFields, 2) {
    const uint64_t v = rd64(ARG(c, 0));
    const int64_t unix100 = static_cast<int64_t>(v) - 116444736000000000ll;
    time_t secs = static_cast<time_t>(unix100 / 10000000);
    struct tm t{};
    gmtime_r(&secs, &t);
    const uint32_t f = ARG(c, 1);
    wr16(f, static_cast<uint16_t>(t.tm_year + 1900));
    wr16(f + 2, static_cast<uint16_t>(t.tm_mon + 1));
    wr16(f + 4, static_cast<uint16_t>(t.tm_mday));
    wr16(f + 6, static_cast<uint16_t>(t.tm_hour));
    wr16(f + 8, static_cast<uint16_t>(t.tm_min));
    wr16(f + 10, static_cast<uint16_t>(t.tm_sec));
    wr16(f + 12, static_cast<uint16_t>((unix100 / 10000) % 1000));
    wr16(f + 14, static_cast<uint16_t>(t.tm_wday));
    return 0;
}
KFUNC(RtlRaiseException, 1) {
    const uint32_t rec = ARG(c, 0);
    die("RtlRaiseException code 0x%08X at 0x%08X (SEH dispatch not implemented yet)", rd32(rec), rd32(rec + 12));
}
KFUNC(RtlUnwind, 4) { die("RtlUnwind (SEH unwinding not implemented yet)"); }
KFUNC(KeBugCheck, 1) { die("KeBugCheck(0x%08X)", ARG(c, 0)); }
KCDECL(DbgPrint) {
    XLOG(1, "DbgPrint: %s", gstr(ARG(c, 0)));
    return 0;
}
KFUNC(DbgBreakPoint, 0) { XLOG(1, "DbgBreakPoint"); return 0; }

// ============================================================================================
// time
// ============================================================================================
KFUNC(KeQuerySystemTime, 1) { wr64(ARG(c, 0), systemTime100ns()); return 0; }
KFUNC(NtSetSystemTime, 2) { return ST_SUCCESS; }
constexpr uint64_t kPerfFreq = 3375000;  // the ACPI timer
KFUNC(KeQueryPerformanceCounter, 0) {
    const uint64_t v = gameClockNs() / 100 * kPerfFreq / 10000000;
    c->edx = static_cast<uint32_t>(v >> 32);
    return static_cast<uint32_t>(v);
}
KFUNC(KeQueryPerformanceFrequency, 0) {
    c->edx = 0;
    return static_cast<uint32_t>(kPerfFreq);
}
KFUNC(KeStallExecutionProcessor, 1) {
    const uint32_t us = ARG(c, 0);
    if (us > 50) {
        GilRelease r;
        std::this_thread::sleep_for(std::chrono::microseconds(us));
    }
    return 0;
}
KFUNC(KeSaveFloatingPointState, 1) { return ST_SUCCESS; }
KFUNC(KeRestoreFloatingPointState, 1) { return ST_SUCCESS; }

// ============================================================================================
// hardware, firmware, settings
// ============================================================================================
KFUNC(ExQueryNonVolatileSetting, 5) {
    const uint32_t idx = ARG(c, 0), pType = ARG(c, 1), val = ARG(c, 2), len = ARG(c, 3), pRes = ARG(c, 4);
    uint32_t v = 0;
    switch (idx) {
    case 7: v = 1; break;                 // XC_LANGUAGE: English
    case 8: v = settings().widescreen ? 0x00010000u : 0u; break;  // XC_VIDEO flags: WIDESCREEN (aspect = 16:9)
    case 9: v = 0; break;                 // XC_AUDIO: stereo
    case 0x103: v = 0x00400100; break;    // XC_FACTORY_AV_REGION: NTSC-M
    case 0x104: v = 1; break;             // XC_FACTORY_GAME_REGION: North America
    default: v = 0; break;
    }
    if (pType) wr32(pType, 4);  // REG_DWORD
    if (val && len >= 4) wr32(val, v);
    else if (val && len) std::memset(gp(val), 0, len);
    if (pRes) wr32(pRes, 4);
    return ST_SUCCESS;
}
KFUNC(HalReadSMBusValue, 4) {
    const uint32_t addr = ARG(c, 0), cmd = ARG(c, 1), out = ARG(c, 3);
    uint32_t v = 0;
    if (addr == 0x21 && cmd == 0x04) v = 0x06;  // SMC: AV pack = standard composite
    if (out) wr32(out, v);
    return ST_SUCCESS;
}
KFUNC(HalReadWritePCISpace, 6) {
    const uint32_t bus = ARG(c, 0), slot = ARG(c, 1), reg = ARG(c, 2), buf = ARG(c, 3), len = ARG(c, 4), write = ARG(c, 5) & 0xFF;
    if (write) return 0;
    std::memset(gp(buf), 0, len);
    if (bus == 1 && slot == 0 && reg == 0 && len >= 4) wr32(buf, 0x02A010DE);  // NV2A
    if (bus == 1 && slot == 0 && reg == 8 && len >= 1) wr8(buf, 0xA1);         // revision
    return 0;
}
KFUNC(AvGetSavedDataAddress, 0) { return 0; }
KFUNC(AvSetSavedDataAddress, 1) { return 0; }
KFUNC(AvSendTVEncoderOption, 4) {
    const uint32_t opt = ARG(c, 1), res = ARG(c, 3);
    if (res) wr32(res, opt == 6 ? 0x00400106 : 0);  // AV_QUERY_AV_CAPABILITIES: standard pack, NTSC
    return 0;
}
uint32_t g_avFramebuffer, g_avPitch, g_avFormat;  // the scanout the game last set
KFUNC(AvSetDisplayMode, 6) {
    g_avFramebuffer = ARG(c, 5);
    g_avPitch = ARG(c, 4);
    g_avFormat = ARG(c, 3);
    XLOG(1, "AvSetDisplayMode(mode 0x%X, format 0x%X, pitch %u, fb 0x%08X)", ARG(c, 2), ARG(c, 3), ARG(c, 4), ARG(c, 5));
    return ST_SUCCESS;
}
KFUNC(HalRegisterShutdownNotification, 2) { return 0; }
KFUNC(HalIsResetOrShutdownPending, 0) { return 0; }
KFUNC(HalReturnToFirmware, 1) {
    XLOG(0, "HalReturnToFirmware(%u): the game asked to leave", ARG(c, 0));
    fflush(stderr);
    _exit(0);
}
KFUNC(HalInitiateShutdown, 0) {
    XLOG(0, "HalInitiateShutdown");
    _exit(0);
}
KFUNC(PhyGetLinkState, 1) { return 0; }  // no network link
KFUNC(PhyInitialize, 2) { return 0xC0000001u; }

// I/O manager device plumbing used by XAPI's drivers (memory units, USB). The HLE input path
// keeps those drivers from starting; these report failure if anything still asks.
KFUNC(IoCreateDevice, 6) { return ST_UNSUCCESSFUL; }
KFAST(IofCallDriver, 0) { return ST_INVALID_DEVICE_REQUEST; }
KFAST(IofCompleteRequest, 0) { return 0; }
KFUNC(IoBuildSynchronousFsdRequest, 7) { return 0; }
KFUNC(IoSynchronousFsdRequest, 5) { return ST_INVALID_DEVICE_REQUEST; }

} // namespace xb
