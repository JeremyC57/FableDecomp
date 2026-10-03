// KERNEL32 imports.
#include "host.hpp"

#include <atomic>
#include <map>
#include <mutex>
#include <vector>

namespace host {

// Handles that are really host pointers (find handles, resource handles) -> small guest tokens.
static std::vector<void*> g_ptrHandles;
static std::mutex g_ptrLock;
uint32_t ptrHandleToGuest(void* p) {
    if (!p) return 0;
    std::lock_guard<std::mutex> l(g_ptrLock);
    for (size_t i = 0; i < g_ptrHandles.size(); ++i)
        if (g_ptrHandles[i] == p) return 0xFF900000u + 0x10u * static_cast<uint32_t>(i + 1);
    g_ptrHandles.push_back(p);
    return 0xFF900000u + 0x10u * static_cast<uint32_t>(g_ptrHandles.size());
}
void* ptrHandleFromGuest(uint32_t g) {
    std::lock_guard<std::mutex> l(g_ptrLock);
    const uint32_t i = (g - 0xFF900000u) / 0x10u;
    if (g < 0xFF900000u || g >= 0xFFA00000u || i == 0 || i > g_ptrHandles.size()) return nullptr;
    return g_ptrHandles[i - 1];
}

namespace {
constexpr const char* K = "kernel32.dll";

SECURITY_ATTRIBUTES* sa(uint32_t g, SECURITY_ATTRIBUTES& tmp) {
    if (!g) return nullptr;
    tmp.nLength = sizeof tmp;
    tmp.lpSecurityDescriptor = nullptr;
    tmp.bInheritHandle = rd32(g + 8);
    return &tmp;
}

// ---------------------------------------------------------------------------
// straight forwards (scalar / pointer-free struct arguments)
// ---------------------------------------------------------------------------
FWD_STD(K, GetTimeFormatW);
FWD_STD(K, VirtualFree);
FWD_STD(K, SystemTimeToFileTime);
FWD_STD(K, FileTimeToSystemTime);
FWD_STD(K, RemoveDirectoryW);
FWD_STD(K, GetLastError);
FWD_STD(K, GetFileSizeEx);
FWD_STD(K, GetFileTime);
FWD_STD(K, SetFileTime);
FWD_STD(K, GetCurrentProcess);
FWD_STD(K, SetPriorityClass);
FWD_STD(K, GetCurrentDirectoryA);
FWD_STD(K, lstrcpyA);
FWD_STD(K, lstrcatA);
FWD_STD(K, lstrlenA);
FWD_STD(K, WideCharToMultiByte);
FWD_STD(K, FlushFileBuffers);
FWD_STD(K, GetFileInformationByHandle);
FWD_STD(K, GetDiskFreeSpaceExW);
FWD_STD(K, SetFileAttributesW);
FWD_STD(K, GetDateFormatW);
FWD_STD(K, GetCurrentThread);
FWD_STD(K, SetThreadPriority);
FWD_STD(K, GetThreadPriority);
FWD_STD(K, SleepEx);
FWD_STD(K, SetEvent);
FWD_STD(K, ResetEvent);
FWD_STD(K, SetFilePointer);
FWD_STD(K, SetEndOfFile);
FWD_STD(K, GetFileSize);
FWD_STD(K, GetTickCount);
FWD_STD(K, QueryPerformanceCounter);
FWD_STD(K, QueryPerformanceFrequency);
FWD_STD(K, WaitForSingleObjectEx);
FWD_STD(K, WaitForSingleObject);
FWD_STD(K, GetSystemTime);
FWD_STD(K, Sleep);
FWD_STD(K, OpenMutexW);
FWD_STD(K, CloseHandle);
FWD_STD(K, DeleteFileW);
FWD_STD(K, lstrlenW);
FWD_STD(K, GetUserDefaultLCID);
FWD_STD(K, FileTimeToLocalFileTime);
FWD_STD(K, GetSystemDefaultLangID);
FWD_STD(K, GetCPInfoExA);
FWD_STD(K, GetVersionExA);
FWD_STD(K, GetVersionExW);
FWD_STD(K, GetVersion);
FWD_STD(K, MultiByteToWideChar);
FWD_STD(K, GetCurrentThreadId);
FWD_STD(K, GetSystemTimeAsFileTime);
FWD_STD(K, GetCurrentProcessId);
FWD_STD(K, ReleaseSemaphore);
FWD_STD(K, GetSystemDirectoryA);
FWD_STD(K, lstrcmpW);
FWD_STD(K, lstrcpyW);
FWD_STD(K, MulDiv);
FWD_STD(K, lstrcatW);
FWD_STD(K, GetLocaleInfoA);
FWD_STD(K, IsDBCSLeadByteEx);
FWD_STD(K, CompareStringA);
FWD_STD(K, lstrcmpiW);
FWD_STD(K, lstrcpynW);

// ---------------------------------------------------------------------------
// interlocked (guest addresses are host addresses)
// ---------------------------------------------------------------------------
IMPORT(K, InterlockedIncrement) { retStd(c, __atomic_add_fetch(argp<uint32_t>(c, 0), 1u, __ATOMIC_SEQ_CST), 1); }
IMPORT(K, InterlockedDecrement) { retStd(c, __atomic_sub_fetch(argp<uint32_t>(c, 0), 1u, __ATOMIC_SEQ_CST), 1); }
IMPORT(K, InterlockedExchange) { retStd(c, __atomic_exchange_n(argp<uint32_t>(c, 0), arg(c, 1), __ATOMIC_SEQ_CST), 2); }

// ---------------------------------------------------------------------------
// critical sections: guest CRITICAL_SECTION (24 bytes) carries a host pointer
// in its first 8 bytes (DebugInfo/LockCount); RecursionCount/OwningThread are
// kept up to date for guest code that inspects them.
// ---------------------------------------------------------------------------
CRITICAL_SECTION* csGet(uint32_t g) {
    auto* slot = gp<uint64_t>(g);
    uint64_t v = __atomic_load_n(slot, __ATOMIC_ACQUIRE);
    if (v) return reinterpret_cast<CRITICAL_SECTION*>(v);
    auto* cs = new CRITICAL_SECTION;
    InitializeCriticalSection(cs);
    uint64_t expected = 0;
    if (__atomic_compare_exchange_n(slot, &expected, reinterpret_cast<uint64_t>(cs), false, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) return cs;
    DeleteCriticalSection(cs);
    delete cs;
    return reinterpret_cast<CRITICAL_SECTION*>(expected);
}
void csEntered(uint32_t g) {
    wr32(g + 8, rd32(g + 8) + 1);
    wr32(g + 12, GetCurrentThreadId());
}
IMPORT(K, InitializeCriticalSection) {
    const uint32_t g = arg(c, 0);
    std::memset(gp(g), 0, 24);
    csGet(g);
    retStd(c, 0, 1);
}
IMPORT(K, DeleteCriticalSection) {
    const uint32_t g = arg(c, 0);
    auto* cs = reinterpret_cast<CRITICAL_SECTION*>(rd64(g));
    if (cs) { DeleteCriticalSection(cs); delete cs; }
    std::memset(gp(g), 0, 24);
    retStd(c, 0, 1);
}
IMPORT(K, EnterCriticalSection) {
    const uint32_t g = arg(c, 0);
    EnterCriticalSection(csGet(g));
    csEntered(g);
    retStd(c, 0, 1);
}
IMPORT(K, TryEnterCriticalSection) {
    const uint32_t g = arg(c, 0);
    const BOOL ok = TryEnterCriticalSection(csGet(g));
    if (ok) csEntered(g);
    retStd(c, ok, 1);
}
IMPORT(K, LeaveCriticalSection) {
    const uint32_t g = arg(c, 0);
    const uint32_t rc = rd32(g + 8) - 1;
    wr32(g + 8, rc);
    if (!rc) wr32(g + 12, 0);
    LeaveCriticalSection(csGet(g));
    retStd(c, 0, 1);
}

// ---------------------------------------------------------------------------
// memory
// ---------------------------------------------------------------------------
constexpr uint32_t kProcessHeap = 0x00140000;
IMPORT(K, GetProcessHeap) { retStd(c, kProcessHeap, 0); }
IMPORT(K, HeapAlloc) {
    const uint32_t flags = arg(c, 1), n = arg(c, 2);
    retStd(c, (flags & HEAP_ZERO_MEMORY) ? gcalloc(1, n) : gmalloc(n), 3);
}
IMPORT(K, HeapFree) { gfree(arg(c, 2)); retStd(c, 1, 3); }
IMPORT(K, HeapSize) { retStd(c, gmsize(arg(c, 2)), 3); }
IMPORT(K, GlobalAlloc) {
    const uint32_t flags = arg(c, 0), n = arg(c, 1);
    retStd(c, (flags & GMEM_ZEROINIT) ? gcalloc(1, n) : gmalloc(n), 2);
}
IMPORT(K, GlobalFree) { gfree(arg(c, 0)); retStd(c, 0, 1); }
IMPORT(K, LocalFree) { gfree(arg(c, 0)); retStd(c, 0, 1); }
IMPORT(K, VirtualAlloc) {
    const uint32_t r = lowVirtualAlloc(arg(c, 0), arg(c, 1), arg(c, 2) & ~MEM_TOP_DOWN, arg(c, 3));
    HLOG(1, "VirtualAlloc(0x%08X, 0x%X, 0x%X, 0x%X) -> 0x%08X", arg(c, 0), arg(c, 1), arg(c, 2), arg(c, 3), r);
    retStd(c, r, 4);
}
IMPORT(K, VirtualQuery) {
    MEMORY_BASIC_INFORMATION m{};
    if (!VirtualQuery(gp(arg(c, 0)), &m, sizeof m) || arg(c, 2) < 28) { retStd(c, 0, 3); return; }
    const uint32_t o = arg(c, 1);
    auto clamp = [](uintptr_t v) { return static_cast<uint32_t>(v > 0xFFFFFFFFu ? 0xFFFFFFFFu : v); };
    wr32(o + 0, clamp(reinterpret_cast<uintptr_t>(m.BaseAddress)));
    wr32(o + 4, clamp(reinterpret_cast<uintptr_t>(m.AllocationBase)));
    wr32(o + 8, m.AllocationProtect);
    wr32(o + 12, clamp(m.RegionSize));
    wr32(o + 16, m.State);
    wr32(o + 20, m.Protect);
    wr32(o + 24, m.Type);
    retStd(c, 28, 3);
}
IMPORT(K, GlobalMemoryStatus) {
    MEMORYSTATUSEX m{sizeof m};
    GlobalMemoryStatusEx(&m);
    const uint32_t o = arg(c, 0);
    auto cl = [](uint64_t v, uint32_t mx) { return static_cast<uint32_t>(v > mx ? mx : v); };
    wr32(o + 0, 32);
    wr32(o + 4, m.dwMemoryLoad);
    wr32(o + 8, cl(m.ullTotalPhys, 0x7FFF0000u));
    wr32(o + 12, cl(m.ullAvailPhys, 0x60000000u));
    wr32(o + 16, cl(m.ullTotalPageFile, 0x7FFF0000u));
    wr32(o + 20, cl(m.ullAvailPageFile, 0x60000000u));
    wr32(o + 24, 0x7FFE0000u);
    wr32(o + 28, 0x70000000u);
    retStd(c, 0, 1);
}
IMPORT(K, GetSystemInfo) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    const uint32_t o = arg(c, 0);
    std::memset(gp(o), 0, 36);
    wr16(o + 0, PROCESSOR_ARCHITECTURE_INTEL);
    wr32(o + 4, 0x1000);
    wr32(o + 8, 0x00010000);
    wr32(o + 12, 0x7FFEFFFF);
    wr32(o + 16, static_cast<uint32_t>(si.dwActiveProcessorMask));
    wr32(o + 20, si.dwNumberOfProcessors);
    wr32(o + 24, PROCESSOR_INTEL_PENTIUM);
    wr32(o + 28, 0x10000);
    wr16(o + 32, 15);
    wr16(o + 34, 0x0209);
    retStd(c, 0, 1);
}

// ---------------------------------------------------------------------------
// files and async I/O
// ---------------------------------------------------------------------------
IMPORT(K, GetFileAttributesW) {
    const DWORD r = GetFileAttributesW(argp<wchar_t>(c, 0));
    HLOG(1, "GetFileAttributesW(%s) -> 0x%lX", narrow(argp<wchar_t>(c, 0)).c_str(), r);
    retStd(c, r, 1);
}
IMPORT(K, GetFileAttributesExW) {
    const BOOL r = GetFileAttributesExW(argp<wchar_t>(c, 0), static_cast<GET_FILEEX_INFO_LEVELS>(arg(c, 1)), argp(c, 2));
    HLOG(1, "GetFileAttributesExW(%s) -> %d", narrow(argp<wchar_t>(c, 0)).c_str(), r);
    retStd(c, r, 3);
}
IMPORT(K, CreateFileA) {
    SECURITY_ATTRIBUTES t;
    HANDLE h = CreateFileA(argp(c, 0), arg(c, 1), arg(c, 2), sa(arg(c, 3), t), arg(c, 4), arg(c, 5), hh(arg(c, 6)));
    HLOG(1, "CreateFileA(%s, 0x%X) -> %s", argp(c, 0), arg(c, 1), h == INVALID_HANDLE_VALUE ? "fail" : "ok");
    retStd(c, gh(h), 7);
}
IMPORT(K, CreateFileW) {
    SECURITY_ATTRIBUTES t;
    HANDLE h = CreateFileW(argp<wchar_t>(c, 0), arg(c, 1), arg(c, 2), sa(arg(c, 3), t), arg(c, 4), arg(c, 5), hh(arg(c, 6)));
    HLOG(1, "CreateFileW(%s, 0x%X, flags 0x%X) -> %s", narrow(argp<wchar_t>(c, 0)).c_str(), arg(c, 1), arg(c, 5),
         h == INVALID_HANDLE_VALUE ? "fail" : "ok");
    retStd(c, gh(h), 7);
}

// Guest OVERLAPPED: Internal, InternalHigh, Offset, OffsetHigh, hEvent (20 bytes).
void ovIn(uint32_t g, OVERLAPPED& o) {
    std::memset(&o, 0, sizeof o);
    o.Offset = rd32(g + 8);
    o.OffsetHigh = rd32(g + 12);
    o.hEvent = hh(rd32(g + 16));
}
void ovOut(uint32_t g, const OVERLAPPED& o) {
    wr32(g + 0, static_cast<uint32_t>(o.Internal));
    wr32(g + 4, static_cast<uint32_t>(o.InternalHigh));
}
void syncIo(Ctx* c, bool write) {
    const HANDLE h = hh(arg(c, 0));
    const uint32_t ovg = arg(c, 4);
    DWORD done = 0;
    BOOL ok;
    if (!ovg) {
        ok = write ? WriteFile(h, argp(c, 1), arg(c, 2), &done, nullptr) : ReadFile(h, argp(c, 1), arg(c, 2), &done, nullptr);
    } else {
        OVERLAPPED o;
        ovIn(ovg, o);
        ok = write ? WriteFile(h, argp(c, 1), arg(c, 2), &done, &o) : ReadFile(h, argp(c, 1), arg(c, 2), &done, &o);
        if (!ok && GetLastError() == ERROR_IO_PENDING) ok = GetOverlappedResult(h, &o, &done, TRUE);
        ovOut(ovg, o);
    }
    if (arg(c, 3)) wr32(arg(c, 3), done);
    retStd(c, ok, 5);
}
IMPORT(K, ReadFile) { syncIo(c, false); }
IMPORT(K, WriteFile) { syncIo(c, true); }

struct AsyncOp {
    OVERLAPPED ov;
    uint32_t guestOv;
    uint32_t routine;
};
VOID CALLBACK asyncDone(DWORD err, DWORD bytes, LPOVERLAPPED ov) {
    auto* op = reinterpret_cast<AsyncOp*>(ov);
    ovOut(op->guestOv, op->ov);
    const uint32_t routine = op->routine, g = op->guestOv;
    delete op;
    if (routine) guestCall(routine, {err, bytes, g});
}
void asyncIo(Ctx* c, bool write) {
    auto* op = new AsyncOp;
    op->guestOv = arg(c, 3);
    op->routine = arg(c, 4);
    ovIn(op->guestOv, op->ov);
    const BOOL ok = write ? WriteFileEx(hh(arg(c, 0)), argp(c, 1), arg(c, 2), &op->ov, asyncDone)
                          : ReadFileEx(hh(arg(c, 0)), argp(c, 1), arg(c, 2), &op->ov, asyncDone);
    if (!ok) delete op;
    retStd(c, ok, 5);
}
IMPORT(K, ReadFileEx) { asyncIo(c, false); }
IMPORT(K, WriteFileEx) { asyncIo(c, true); }

IMPORT(K, DeviceIoControl) {
    if (arg(c, 7)) log("DeviceIoControl with OVERLAPPED: running synchronously");
    DWORD ret = 0;
    const BOOL ok = DeviceIoControl(hh(arg(c, 0)), arg(c, 1), argp(c, 2), arg(c, 3), argp(c, 4), arg(c, 5), &ret, nullptr);
    if (arg(c, 6)) wr32(arg(c, 6), ret);
    retStd(c, ok, 8);
}
IMPORT(K, FindFirstFileA) {
    HANDLE h = FindFirstFileA(argp(c, 0), argp<WIN32_FIND_DATAA>(c, 1));
    retStd(c, h == INVALID_HANDLE_VALUE ? 0xFFFFFFFFu : ptrHandleToGuest(h), 2);
}
IMPORT(K, FindClose) {
    HANDLE h = ptrHandleFromGuest(arg(c, 0));
    retStd(c, h ? FindClose(h) : FALSE, 1);
}

// ---------------------------------------------------------------------------
// synchronisation objects
// ---------------------------------------------------------------------------
IMPORT(K, CreateEventA) {
    SECURITY_ATTRIBUTES t;
    retStd(c, gh(CreateEventA(sa(arg(c, 0), t), arg(c, 1), arg(c, 2), argp(c, 3))), 4);
}
IMPORT(K, CreateEventW) {
    SECURITY_ATTRIBUTES t;
    retStd(c, gh(CreateEventW(sa(arg(c, 0), t), arg(c, 1), arg(c, 2), argp<wchar_t>(c, 3))), 4);
}
IMPORT(K, CreateMutexW) {
    SECURITY_ATTRIBUTES t;
    retStd(c, gh(CreateMutexW(sa(arg(c, 0), t), arg(c, 1), argp<wchar_t>(c, 2))), 3);
}
IMPORT(K, CreateSemaphoreA) {
    SECURITY_ATTRIBUTES t;
    retStd(c, gh(CreateSemaphoreA(sa(arg(c, 0), t), arg(c, 1), arg(c, 2), argp(c, 3))), 4);
}
IMPORT(K, CreateSemaphoreW) {
    SECURITY_ATTRIBUTES t;
    retStd(c, gh(CreateSemaphoreW(sa(arg(c, 0), t), arg(c, 1), arg(c, 2), argp<wchar_t>(c, 3))), 4);
}
IMPORT(K, WaitForMultipleObjects) {
    const uint32_t n = arg(c, 0);
    HANDLE hs[MAXIMUM_WAIT_OBJECTS];
    for (uint32_t i = 0; i < n && i < MAXIMUM_WAIT_OBJECTS; ++i) hs[i] = hh(rd32(arg(c, 1) + 4 * i));
    retStd(c, WaitForMultipleObjects(n, hs, arg(c, 2), arg(c, 3)), 4);
}
IMPORT(K, DuplicateHandle) {
    HANDLE out = nullptr;
    const BOOL ok = DuplicateHandle(hh(arg(c, 0)), hh(arg(c, 1)), hh(arg(c, 2)), &out, arg(c, 4), arg(c, 5), arg(c, 6));
    if (arg(c, 3)) wr32(arg(c, 3), gh(out));
    retStd(c, ok, 7);
}
IMPORT(K, CreateThread) {
    DWORD tid = 0;
    HANDLE h = startGuestThread(arg(c, 2), arg(c, 3), arg(c, 1), arg(c, 4), &tid);
    if (arg(c, 5)) wr32(arg(c, 5), tid);
    HLOG(1, "CreateThread(0x%08X, param 0x%08X) -> tid %lu", arg(c, 2), arg(c, 3), tid);
    retStd(c, gh(h), 6);
}
IMPORT(K, ConvertThreadToFiber) {
    const uint32_t fiber = gcalloc(1, 0x40);
    wr32(fiber, arg(c, 0));
    wr32(currentThread()->teb + 0x10, fiber);
    HLOG(1, "ConvertThreadToFiber(0x%08X) -> 0x%08X", arg(c, 0), fiber);
    retStd(c, fiber, 1);
}

// ---------------------------------------------------------------------------
// process / modules / resources
// ---------------------------------------------------------------------------
IMPORT(K, ExitProcess) {
    log("guest ExitProcess(%u)", arg(c, 0));
    ExitProcess(arg(c, 0));
}
IMPORT(K, DebugBreak) {
    log("guest DebugBreak() from 0x%08X", rd32(c->esp));
    retStd(c, 0, 0);
}
IMPORT(K, OutputDebugStringA) {
    log("[guest] %s", argp(c, 0));
    retStd(c, 0, 1);
}
IMPORT(K, OutputDebugStringW) {
    log("[guest] %s", narrow(argp<wchar_t>(c, 0)).c_str());
    retStd(c, 0, 1);
}
IMPORT(K, GetCommandLineW) {
    static uint32_t s = 0;
    if (!s) {
        wstring cmd = L"\"" + g_exePath + L"\"";
        int n = 0;
        LPWSTR* w = CommandLineToArgvW(GetCommandLineW(), &n);
        for (int i = 1; i < n; ++i) {
            if (_wcsicmp(w[i], L"--game") == 0 && i + 1 < n) { ++i; continue; }
            cmd += L" ";
            cmd += w[i];
        }
        LocalFree(w);
        s = gwcsdup(cmd.c_str());
    }
    retStd(c, s, 0);
}
IMPORT(K, GetStartupInfoA) {
    const uint32_t o = arg(c, 0);
    std::memset(gp(o), 0, 68);
    wr32(o, 68);
    retStd(c, 0, 1);
}
IMPORT(K, GetModuleHandleA) {
    const char* n = arg(c, 0) ? argp(c, 0) : nullptr;
    uint32_t r = kImageBase;
    if (n && _stricmp(n, "fable.exe") != 0) r = isKnownDll(n) ? moduleToGuest(nullptr, n) : 0;
    HLOG(1, "GetModuleHandleA(%s) -> 0x%08X", n ? n : "NULL", r);
    retStd(c, r, 1);
}
IMPORT(K, GetModuleHandleW) {
    const std::string n = arg(c, 0) ? narrow(argp<wchar_t>(c, 0)) : "";
    uint32_t r = kImageBase;
    if (arg(c, 0) && _stricmp(n.c_str(), "fable.exe") != 0) r = isKnownDll(n.c_str()) ? moduleToGuest(nullptr, n.c_str()) : 0;
    HLOG(1, "GetModuleHandleW(%s) -> 0x%08X", arg(c, 0) ? n.c_str() : "NULL", r);
    retStd(c, r, 1);
}
IMPORT(K, GetModuleFileNameW) {
    const uint32_t m = arg(c, 0), n = arg(c, 2);
    wstring path;
    if (!m || m == kImageBase) path = g_exePath;
    else if (HMODULE h = moduleFromGuest(m)) { wchar_t b[MAX_PATH]; GetModuleFileNameW(h, b, MAX_PATH); path = b; }
    else if (const char* nm = moduleName(m)) path = g_gameDir + widen(nm);
    if (!n) { retStd(c, 0, 3); return; }
    const uint32_t len = static_cast<uint32_t>(path.size()) < n ? static_cast<uint32_t>(path.size()) : n - 1;
    std::memcpy(argp(c, 1), path.c_str(), len * 2);
    wr16(arg(c, 1) + len * 2, 0);
    retStd(c, len, 3);
}

uint32_t loadLibrary(const std::string& name) {
    std::string base = name;
    if (auto p = base.find_last_of("\\/"); p != std::string::npos) base = base.substr(p + 1);
    if (isKnownDll(base.c_str())) {
        const uint32_t g = moduleToGuest(nullptr, base.c_str());
        HLOG(1, "LoadLibrary(%s) -> host implementation 0x%08X", name.c_str(), g);
        return g;
    }
    // A helper DLL we recompiled: map it and run its DllMain.
    if (const uint32_t g = loadGuestDll(base)) return g;
    // Resource-only use: load from the game folder as a data file.
    wstring path = widen(name.c_str());
    if (path.find(L':') == wstring::npos) path = g_gameDir + widen(base.c_str());
    HMODULE h = LoadLibraryExW(path.c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE);
    if (h) {
        const uint32_t g = moduleToGuest(h, base.c_str());
        HLOG(1, "LoadLibrary(%s) -> data file 0x%08X", name.c_str(), g);
        return g;
    }
    log("LoadLibrary(%s) -> not available", name.c_str());
    return 0;
}
IMPORT(K, LoadLibraryA) { retStd(c, loadLibrary(argp(c, 0)), 1); }
IMPORT(K, LoadLibraryW) { retStd(c, loadLibrary(narrow(argp<wchar_t>(c, 0))), 1); }
IMPORT(K, FreeLibrary) { retStd(c, 1, 1); }
IMPORT(K, GetProcAddress) {
    const uint32_t m = arg(c, 0), n = arg(c, 1);
    const char* mod = moduleName(m);
    std::string name = n < 0x10000 ? "#" + std::to_string(n) : std::string(gp<char>(n));
    uint32_t r = isGuestDll(m) ? guestDllExport(m, name) : mod ? resolveImport(mod, name.c_str()) : 0;
    HLOG(1, "GetProcAddress(%s, %s) -> 0x%08X", mod ? mod : "?", name.c_str(), r);
    if (!r) SetLastError(ERROR_PROC_NOT_FOUND);
    retStd(c, r, 2);
}

// Resources: host handles -> tokens; locked data copied into guest memory once.
struct ResRec { HMODULE mod; HRSRC res; uint32_t data; };
std::vector<ResRec> g_res;
std::mutex g_resLock;
LPCSTR resId(uint32_t g) { return g < 0x10000 ? MAKEINTRESOURCEA(g) : gp<char>(g); }
IMPORT(K, FindResourceExA) {
    HMODULE m = moduleFromGuest(arg(c, 0));
    HRSRC r = m ? FindResourceExA(m, resId(arg(c, 1)), resId(arg(c, 2)), static_cast<WORD>(arg(c, 3))) : nullptr;
    if (!r) { retStd(c, 0, 4); return; }
    std::lock_guard<std::mutex> l(g_resLock);
    g_res.push_back({m, r, 0});
    retStd(c, 0xFF800000u + 0x10u * static_cast<uint32_t>(g_res.size()), 4);
}
IMPORT(K, LoadResource) { retStd(c, arg(c, 1), 2); }  // HGLOBAL token == HRSRC token
IMPORT(K, LockResource) {
    const uint32_t t = arg(c, 0);
    std::lock_guard<std::mutex> l(g_resLock);
    const uint32_t i = (t - 0xFF800000u) / 0x10u;
    if (t < 0xFF800000u || i == 0 || i > g_res.size()) { retStd(c, 0, 1); return; }
    ResRec& r = g_res[i - 1];
    if (!r.data) {
        HGLOBAL hg = LoadResource(r.mod, r.res);
        const DWORD sz = SizeofResource(r.mod, r.res);
        if (hg) r.data = gmemdup(LockResource(hg), sz);
    }
    retStd(c, r.data, 1);
}
IMPORT(K, EnumResourceNamesA) {
    HMODULE m = moduleFromGuest(arg(c, 0));
    std::vector<uint32_t> names;
    EnumResourceNamesA(
        m, resId(arg(c, 1)),
        [](HMODULE, LPCSTR, LPSTR name, LONG_PTR lp) -> BOOL {
            auto* v = reinterpret_cast<std::vector<uint32_t>*>(lp);
            v->push_back(IS_INTRESOURCE(name) ? static_cast<uint32_t>(reinterpret_cast<uintptr_t>(name)) : gstrdup(name));
            return TRUE;
        },
        reinterpret_cast<LONG_PTR>(&names));
    const uint32_t mod = arg(c, 0), type = arg(c, 1), fn = arg(c, 2), lp = arg(c, 3);
    for (uint32_t n : names)
        if (!guestCall(fn, {mod, type, n, lp})) break;
    retStd(c, names.empty() ? 0 : 1, 4);
}

IMPORT(K, FormatMessageW) {
    const uint32_t flags = arg(c, 0);
    const void* src = (flags & FORMAT_MESSAGE_FROM_STRING) ? static_cast<const void*>(argp(c, 1)) : nullptr;
    if (flags & FORMAT_MESSAGE_FROM_HMODULE) src = moduleFromGuest(arg(c, 1));
    const DWORD f = (flags & ~FORMAT_MESSAGE_ARGUMENT_ARRAY) | FORMAT_MESSAGE_IGNORE_INSERTS;
    if (flags & FORMAT_MESSAGE_ALLOCATE_BUFFER) {
        wchar_t* buf = nullptr;
        const DWORD n = FormatMessageW(f, src, arg(c, 2), arg(c, 3), reinterpret_cast<LPWSTR>(&buf), arg(c, 5), nullptr);
        wr32(arg(c, 4), n ? gmemdup(buf, (n + 1) * 2) : 0);
        if (buf) LocalFree(buf);
        retStd(c, n, 7);
    } else {
        retStd(c, FormatMessageW(f, src, arg(c, 2), arg(c, 3), argp<wchar_t>(c, 4), arg(c, 5), nullptr), 7);
    }
}


// ---------------------------------------------------------------------------
// used by statically linked CRTs in recompiled guest DLLs
// ---------------------------------------------------------------------------
FWD_STD(K, GetACP);
FWD_STD(K, GetOEMCP);
FWD_STD(K, GetCPInfo);
FWD_STD(K, GetDiskFreeSpaceExA);
FWD_STD(K, GetFileType);
FWD_STD(K, GetPriorityClass);
FWD_STD(K, GetStdHandle);
FWD_STD(K, SetStdHandle);
FWD_STD(K, GetStringTypeA);
FWD_STD(K, GetStringTypeW);
FWD_STD(K, LCMapStringA);
FWD_STD(K, LCMapStringW);
FWD_STD(K, SetLastError);
FWD_STD(K, TerminateProcess);
FWD_STD(K, lstrcmpiA);
IMPORT(K, SetHandleCount) { retStd(c, arg(c, 0), 1); }
IMPORT(K, IsBadReadPtr) { retStd(c, arg(c, 0) ? 0 : 1, 2); }
IMPORT(K, IsBadWritePtr) { retStd(c, arg(c, 0) ? 0 : 1, 2); }
IMPORT(K, IsBadCodePtr) { retStd(c, arg(c, 0) ? 0 : 1, 1); }
IMPORT(K, SetUnhandledExceptionFilter) { retStd(c, 0, 1); }
IMPORT(K, UnhandledExceptionFilter) { retStd(c, EXCEPTION_CONTINUE_SEARCH, 1); }
IMPORT(K, RtlUnwind) { die("RtlUnwind (SEH unwinding) is not supported yet, called from 0x%08X", rd32(c->esp)); }
IMPORT(K, RaiseException) { die("RaiseException(0x%08X) from 0x%08X: SEH dispatch is not supported yet", arg(c, 0), rd32(c->esp)); }
IMPORT(K, VirtualProtect) {
    // Guest memory is plain read/write data (code never executes from it).
    if (arg(c, 3)) wr32(arg(c, 3), PAGE_EXECUTE_READWRITE);
    retStd(c, 1, 4);
}
IMPORT(K, GetCommandLineA) {
    static uint32_t s = 0;
    if (!s) s = gstrdup(("\"" + narrow(g_exePath.c_str()) + "\"").c_str());
    retStd(c, s, 0);
}
IMPORT(K, GetModuleFileNameA) {
    const uint32_t m = arg(c, 0), n = arg(c, 2);
    std::string path;
    if (!m || m == kImageBase) path = narrow(g_exePath.c_str());
    else if (const char* nm = moduleName(m)) path = narrow(g_gameDir.c_str()) + nm;
    if (!n) { retStd(c, 0, 3); return; }
    const uint32_t len = static_cast<uint32_t>(path.size()) < n ? static_cast<uint32_t>(path.size()) : n - 1;
    std::memcpy(argp(c, 1), path.c_str(), len);
    wr8(arg(c, 1) + len, 0);
    retStd(c, len, 3);
}
IMPORT(K, GetEnvironmentStrings) {
    LPCH e = GetEnvironmentStringsA();
    size_t n = 0;
    while (e[n] || e[n + 1]) ++n;
    const uint32_t g = gmemdup(e, static_cast<uint32_t>(n + 2));
    FreeEnvironmentStringsA(e);
    retStd(c, g, 0);
}
IMPORT(K, GetEnvironmentStringsW) {
    LPWCH e = GetEnvironmentStringsW();
    size_t n = 0;
    while (e[n] || e[n + 1]) ++n;
    const uint32_t g = gmemdup(e, static_cast<uint32_t>((n + 2) * 2));
    FreeEnvironmentStringsW(e);
    retStd(c, g, 0);
}
IMPORT(K, FreeEnvironmentStringsA) { gfree(arg(c, 0)); retStd(c, 1, 1); }
IMPORT(K, FreeEnvironmentStringsW) { gfree(arg(c, 0)); retStd(c, 1, 1); }
IMPORT(K, HeapCreate) { retStd(c, kProcessHeap + 0x10000u * (1 + (arg(c, 0) & 0xF)), 3); }
IMPORT(K, HeapDestroy) { retStd(c, 1, 1); }
IMPORT(K, HeapReAlloc) {
    const uint32_t flags = arg(c, 1), p = arg(c, 2), n = arg(c, 3);
    const uint32_t old = gmsize(p);
    if ((flags & HEAP_REALLOC_IN_PLACE_ONLY) && n > old) { retStd(c, 0, 4); return; }
    const uint32_t q = grealloc(p, n);
    if (q && (flags & HEAP_ZERO_MEMORY) && n > old) std::memset(gp(q + old), 0, n - old);
    retStd(c, q, 4);
}

// TLS: slots live in each guest TEB (teb+0xE10), like the real thing.
std::mutex g_tlsLock;
uint64_t g_tlsUsed = 0;
IMPORT(K, TlsAlloc) {
    std::lock_guard<std::mutex> l(g_tlsLock);
    for (uint32_t i = 0; i < 64; ++i)
        if (!(g_tlsUsed >> i & 1)) { g_tlsUsed |= 1ull << i; retStd(c, i, 0); return; }
    retStd(c, TLS_OUT_OF_INDEXES, 0);
}
IMPORT(K, TlsFree) {
    std::lock_guard<std::mutex> l(g_tlsLock);
    if (arg(c, 0) < 64) g_tlsUsed &= ~(1ull << arg(c, 0));
    retStd(c, 1, 1);
}
IMPORT(K, TlsGetValue) { SetLastError(0); retStd(c, arg(c, 0) < 64 ? rd32(currentThread()->teb + 0xE10 + 4 * arg(c, 0)) : 0, 1); }
IMPORT(K, TlsSetValue) {
    if (arg(c, 0) >= 64) { retStd(c, 0, 2); return; }
    wr32(currentThread()->teb + 0xE10 + 4 * arg(c, 0), arg(c, 1));
    retStd(c, 1, 2);
}
}  // namespace
}  // namespace host
