// Synchronisation objects, waits, critical sections, SRW locks, threads, sleeping and fibers.
#include "w32.hpp"

#include <chrono>
#include <cstring>
#include <map>
#include <pthread.h>
#include <sched.h>
#include <shared_mutex>
#include <sys/mman.h>
#include <thread>
#include <unistd.h>

namespace w32 {
std::condition_variable_any& waitCv();
bool hasApcs(DWORD tid);

namespace {

// ---- objects -----------------------------------------------------------------------------
struct Event : Object {
    bool manual, set;
    Event(bool m, bool s) : manual(m), set(s) {}
    const char* kind() const override { return "event"; }
    bool waitable() const override { return true; }
    bool signaled(DWORD) const override { return set; }
    void acquire(DWORD) override { if (!manual) set = false; }
};
struct Mutex : Object {
    DWORD owner = 0;
    uint32_t count = 0;
    const char* kind() const override { return "mutex"; }
    bool waitable() const override { return true; }
    bool signaled(DWORD tid) const override { return owner == 0 || owner == tid; }
    void acquire(DWORD tid) override { owner = tid; ++count; }
};
struct Semaphore : Object {
    LONG count, max;
    Semaphore(LONG c, LONG m) : count(c), max(m) {}
    const char* kind() const override { return "semaphore"; }
    bool waitable() const override { return true; }
    bool signaled(DWORD) const override { return count > 0; }
    void acquire(DWORD) override { --count; }
};
}  // namespace

struct Thread : Object {
    DWORD tid = 0;
    bool done = false;
    DWORD exitCode = STILL_ACTIVE;
    bool suspended = false;
    int priority = THREAD_PRIORITY_NORMAL;
    const char* kind() const override { return "thread"; }
    bool waitable() const override { return true; }
    bool signaled(DWORD) const override { return done; }
};

namespace {
std::mutex g_namedLock;
std::map<std::u16string, std::weak_ptr<Object>> g_named;

std::u16string key(LPCWSTR name) {
    std::u16string k;
    for (const wchar_t* p = name; p && *p; ++p) k += static_cast<char16_t>(*p);
    return k;
}
template <class T, class Make> HANDLE createNamed(LPCWSTR name, Make make) {
    if (!name || !*name) { setError(ERROR_SUCCESS); return newHandle(make()); }
    std::lock_guard<std::mutex> l(g_namedLock);
    auto& slot = g_named[key(name)];
    if (auto existing = slot.lock()) {
        if (!std::dynamic_pointer_cast<T>(existing)) { setError(ERROR_INVALID_HANDLE); return nullptr; }
        setError(ERROR_ALREADY_EXISTS);
        return newHandle(existing);
    }
    auto o = make();
    slot = o;
    setError(ERROR_SUCCESS);
    return newHandle(o);
}
LPCWSTR wideName(LPCSTR a, wstr& keep) {
    if (!a) return nullptr;
    keep = fromUtf8(acpToUtf8(a).c_str());
    return keep.c_str();
}

// thread bookkeeping
std::atomic<DWORD> g_nextTid{0x100};
thread_local DWORD t_tid;
thread_local std::shared_ptr<Thread> t_self;
}  // namespace

DWORD currentTid() {
    if (!t_tid) t_tid = g_nextTid.fetch_add(4);
    return t_tid;
}

static DWORD waitMany(DWORD n, const HANDLE* hs, BOOL all, DWORD timeout, BOOL alertable) {
    std::vector<std::shared_ptr<Object>> objs(n);
    for (DWORD i = 0; i < n; ++i) {
        if (hs[i] == GetCurrentProcess()) { objs[i] = nullptr; continue; }
        objs[i] = object(hs[i]);
        if (!objs[i] || !objs[i]->waitable()) { setError(ERROR_INVALID_HANDLE); return WAIT_FAILED; }
    }
    const DWORD tid = currentTid();
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout);
    std::unique_lock<std::mutex> lk(waitLock());
    for (;;) {
        if (alertable && hasApcs(tid)) {
            lk.unlock();
            runApcs();
            return WAIT_IO_COMPLETION;
        }
        if (all) {
            bool ok = n > 0;
            for (auto& o : objs) ok = ok && o && o->signaled(tid);
            if (ok) {
                for (auto& o : objs) o->acquire(tid);
                return WAIT_OBJECT_0;
            }
        } else {
            for (DWORD i = 0; i < n; ++i)
                if (objs[i] && objs[i]->signaled(tid)) {
                    objs[i]->acquire(tid);
                    return WAIT_OBJECT_0 + i;
                }
        }
        if (timeout == 0) return WAIT_TIMEOUT;
        if (timeout == INFINITE) waitCv().wait(lk);
        else if (waitCv().wait_until(lk, deadline) == std::cv_status::timeout) timeout = 0;  // final check, then time out
    }
}

// ---- fibers ------------------------------------------------------------------------------
// A fiber is a host stack plus the stack pointer saved by w32_swapctx (fiber_asm.S).
struct Fiber {
    void* sp = nullptr;
    void* stack = nullptr;
    size_t stackSize = 0;
    LPFIBER_START_ROUTINE fn = nullptr;
    void* param = nullptr;
    bool converted = false;  // the thread's own stack
};
thread_local Fiber* t_fiber;
thread_local uint64_t t_teb[16];  // fake NT_TIB: FiberData (+0x20), Self (+0x30)

}  // namespace w32

using namespace w32;

extern "C" {
void w32_swapctx(void** saveSp, void* newSp);
void w32_fiber_trampoline(void);

// Called on a fresh fiber stack by w32_fiber_trampoline.
void w32_fiber_entry(void) {
    Fiber* f = t_fiber;
    f->fn(f->param);
    // Returning from a fiber routine ends the thread on Windows.
    pthread_exit(nullptr);
}

// MinGW's GetCurrentFiber()/NtCurrentTeb() read the TEB through __readgsqword.
unsigned long long w32_readgsqword(unsigned long off) {
    t_teb[0x30 / 8] = reinterpret_cast<uintptr_t>(t_teb);
    t_teb[0x20 / 8] = reinterpret_cast<uintptr_t>(t_fiber ? t_fiber->param : nullptr);
    if (off == 0x20 && t_fiber) return reinterpret_cast<uintptr_t>(t_fiber);  // GetCurrentFiber
    return off < sizeof t_teb ? t_teb[off / 8] : 0;
}

LPVOID WINAPI ConvertThreadToFiber(LPVOID param) {
    if (t_fiber) { setError(ERROR_ALREADY_FIBER); return nullptr; }
    auto* f = new Fiber;
    f->param = param;
    f->converted = true;
    t_fiber = f;
    return f;
}
LPVOID WINAPI ConvertThreadToFiberEx(LPVOID param, DWORD) { return ConvertThreadToFiber(param); }
BOOL WINAPI IsThreadAFiber(void) { return t_fiber != nullptr; }
LPVOID WINAPI CreateFiberEx(SIZE_T commit, SIZE_T reserve, DWORD, LPFIBER_START_ROUTINE fn, LPVOID param) {
    size_t size = reserve > commit ? reserve : commit;
    if (size < (1u << 20)) size = 1u << 20;
    size = (size + 0xFFFF) & ~size_t(0xFFFF);
    void* stack = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE | MAP_STACK, -1, 0);
    if (stack == MAP_FAILED) { setError(ERROR_NOT_ENOUGH_MEMORY); return nullptr; }
    auto* f = new Fiber;
    f->stack = stack;
    f->stackSize = size;
    f->fn = fn;
    f->param = param;
    // Initial frame popped by w32_swapctx: callee-saved registers, FP control, return address.
    auto* top = reinterpret_cast<uint64_t*>((reinterpret_cast<uintptr_t>(stack) + size - 256) & ~uintptr_t(15));
#if defined(__x86_64__)
    // [mxcsr|fpucw][r15][r14][r13][r12][rbx][rbp][ret]
    uint64_t* sp = top - 8;
    std::memset(sp, 0, 8 * 8);
    uint32_t mx;
    uint16_t cw;
    __asm__ volatile("stmxcsr %0" : "=m"(mx));
    __asm__ volatile("fnstcw %0" : "=m"(cw));
    sp[0] = mx | (static_cast<uint64_t>(cw) << 32);
    sp[7] = reinterpret_cast<uintptr_t>(&w32_fiber_trampoline);
#elif defined(__aarch64__)
    // 176-byte frame: x19..x30 (x30 = return address at +88), d8..d15, fpcr at +160
    uint64_t* sp = top - 22;
    std::memset(sp, 0, 22 * 8);
    sp[11] = reinterpret_cast<uintptr_t>(&w32_fiber_trampoline);
    uint64_t fpcr;
    __asm__ volatile("mrs %0, fpcr" : "=r"(fpcr));
    sp[20] = fpcr;
#else
#error "fibers: unsupported architecture"
#endif
    f->sp = sp;
    return f;
}
LPVOID WINAPI CreateFiber(SIZE_T stack, LPFIBER_START_ROUTINE fn, LPVOID param) { return CreateFiberEx(stack, 0, 0, fn, param); }
void WINAPI SwitchToFiber(LPVOID target) {
    auto* to = static_cast<Fiber*>(target);
    Fiber* from = t_fiber;
    if (to == from) return;
    t_fiber = to;
    w32_swapctx(&from->sp, to->sp);
}
void WINAPI DeleteFiber(LPVOID target) {
    auto* f = static_cast<Fiber*>(target);
    if (!f) return;
    if (f == t_fiber) pthread_exit(nullptr);
    if (f->stack) munmap(f->stack, f->stackSize);
    delete f;
}

// ---- events / mutexes / semaphores ---------------------------------------------------------
HANDLE WINAPI CreateEventW(LPSECURITY_ATTRIBUTES, BOOL manual, BOOL initial, LPCWSTR name) {
    return createNamed<Event>(name, [&] { return std::make_shared<Event>(manual, initial); });
}
HANDLE WINAPI CreateEventA(LPSECURITY_ATTRIBUTES sa, BOOL manual, BOOL initial, LPCSTR name) {
    wstr k;
    return CreateEventW(sa, manual, initial, wideName(name, k));
}
BOOL WINAPI SetEvent(HANDLE h) {
    auto e = objectAs<Event>(h);
    if (!e) return fail(ERROR_INVALID_HANDLE);
    std::lock_guard<std::mutex> l(waitLock());
    e->set = true;
    notifyAll();
    return TRUE;
}
BOOL WINAPI ResetEvent(HANDLE h) {
    auto e = objectAs<Event>(h);
    if (!e) return fail(ERROR_INVALID_HANDLE);
    std::lock_guard<std::mutex> l(waitLock());
    e->set = false;
    return TRUE;
}
BOOL WINAPI PulseEvent(HANDLE h) {
    SetEvent(h);
    std::this_thread::yield();
    return ResetEvent(h);
}
HANDLE WINAPI CreateMutexW(LPSECURITY_ATTRIBUTES, BOOL owned, LPCWSTR name) {
    HANDLE h = createNamed<Mutex>(name, [] { return std::make_shared<Mutex>(); });
    if (h && owned && GetLastError() != ERROR_ALREADY_EXISTS) {
        std::lock_guard<std::mutex> l(waitLock());
        objectAs<Mutex>(h)->acquire(currentTid());
    }
    return h;
}
HANDLE WINAPI CreateMutexA(LPSECURITY_ATTRIBUTES sa, BOOL owned, LPCSTR name) {
    wstr k;
    return CreateMutexW(sa, owned, wideName(name, k));
}
HANDLE WINAPI OpenMutexW(DWORD, BOOL, LPCWSTR name) {
    std::lock_guard<std::mutex> l(g_namedLock);
    auto it = name ? g_named.find(key(name)) : g_named.end();
    if (it == g_named.end()) { setError(ERROR_FILE_NOT_FOUND); return nullptr; }
    auto o = it->second.lock();
    if (!o || !std::dynamic_pointer_cast<Mutex>(o)) { setError(ERROR_FILE_NOT_FOUND); return nullptr; }
    return newHandle(o);
}
BOOL WINAPI ReleaseMutex(HANDLE h) {
    auto m = objectAs<Mutex>(h);
    if (!m) return fail(ERROR_INVALID_HANDLE);
    std::lock_guard<std::mutex> l(waitLock());
    if (m->owner != currentTid()) return fail(ERROR_NOT_OWNER);
    if (--m->count == 0) m->owner = 0;
    notifyAll();
    return TRUE;
}
HANDLE WINAPI CreateSemaphoreW(LPSECURITY_ATTRIBUTES, LONG initial, LONG max, LPCWSTR name) {
    return createNamed<Semaphore>(name, [&] { return std::make_shared<Semaphore>(initial, max); });
}
HANDLE WINAPI CreateSemaphoreA(LPSECURITY_ATTRIBUTES sa, LONG initial, LONG max, LPCSTR name) {
    wstr k;
    return CreateSemaphoreW(sa, initial, max, wideName(name, k));
}
BOOL WINAPI ReleaseSemaphore(HANDLE h, LONG n, LPLONG prev) {
    auto s = objectAs<Semaphore>(h);
    if (!s) return fail(ERROR_INVALID_HANDLE);
    std::lock_guard<std::mutex> l(waitLock());
    if (prev) *prev = s->count;
    if (s->count + n > s->max) return fail(ERROR_TOO_MANY_POSTS);
    s->count += n;
    notifyAll();
    return TRUE;
}

DWORD WINAPI WaitForSingleObject(HANDLE h, DWORD ms) { return waitMany(1, &h, FALSE, ms, FALSE); }
DWORD WINAPI WaitForSingleObjectEx(HANDLE h, DWORD ms, BOOL alertable) { return waitMany(1, &h, FALSE, ms, alertable); }
DWORD WINAPI WaitForMultipleObjects(DWORD n, const HANDLE* hs, BOOL all, DWORD ms) { return waitMany(n, hs, all, ms, FALSE); }
DWORD WINAPI WaitForMultipleObjectsEx(DWORD n, const HANDLE* hs, BOOL all, DWORD ms, BOOL alertable) { return waitMany(n, hs, all, ms, alertable); }

BOOL WINAPI CloseHandle(HANDLE h) {
    if (h == GetCurrentProcess() || h == GetCurrentThread()) return TRUE;
    return closeHandle(h) ? TRUE : fail(ERROR_INVALID_HANDLE);
}
BOOL WINAPI DuplicateHandle(HANDLE, HANDLE src, HANDLE, LPHANDLE out, DWORD, BOOL, DWORD options) {
    std::shared_ptr<Object> o = src == GetCurrentThread() ? std::static_pointer_cast<Object>(t_self) : object(src);
    if (!o) return fail(ERROR_INVALID_HANDLE);
    *out = newHandle(o);
    if (options & DUPLICATE_CLOSE_SOURCE) closeHandle(src);
    return TRUE;
}

// ---- critical sections ----------------------------------------------------------------------
// The pthread mutex lives behind DebugInfo; RecursionCount/OwningThread are kept for inspection.
static pthread_mutex_t* csMutex(LPCRITICAL_SECTION cs) { return reinterpret_cast<pthread_mutex_t*>(cs->DebugInfo); }
void WINAPI InitializeCriticalSection(LPCRITICAL_SECTION cs) {
    std::memset(cs, 0, sizeof *cs);
    auto* m = new pthread_mutex_t;
    pthread_mutexattr_t a;
    pthread_mutexattr_init(&a);
    pthread_mutexattr_settype(&a, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(m, &a);
    pthread_mutexattr_destroy(&a);
    cs->DebugInfo = reinterpret_cast<PRTL_CRITICAL_SECTION_DEBUG>(m);
    cs->LockCount = -1;
}
BOOL WINAPI InitializeCriticalSectionAndSpinCount(LPCRITICAL_SECTION cs, DWORD) { InitializeCriticalSection(cs); return TRUE; }
void WINAPI DeleteCriticalSection(LPCRITICAL_SECTION cs) {
    if (auto* m = csMutex(cs)) { pthread_mutex_destroy(m); delete m; }
    cs->DebugInfo = nullptr;
}
void WINAPI EnterCriticalSection(LPCRITICAL_SECTION cs) {
    pthread_mutex_lock(csMutex(cs));
    ++cs->RecursionCount;
    cs->OwningThread = reinterpret_cast<HANDLE>(static_cast<uintptr_t>(currentTid()));
}
BOOL WINAPI TryEnterCriticalSection(LPCRITICAL_SECTION cs) {
    if (pthread_mutex_trylock(csMutex(cs)) != 0) return FALSE;
    ++cs->RecursionCount;
    cs->OwningThread = reinterpret_cast<HANDLE>(static_cast<uintptr_t>(currentTid()));
    return TRUE;
}
void WINAPI LeaveCriticalSection(LPCRITICAL_SECTION cs) {
    if (--cs->RecursionCount == 0) cs->OwningThread = nullptr;
    pthread_mutex_unlock(csMutex(cs));
}

// ---- SRW locks: SRWLOCK_INIT is zero, so the lock object is created on first use ------------
static std::shared_mutex* srw(PSRWLOCK l) {
    auto** slot = reinterpret_cast<std::shared_mutex**>(&l->Ptr);
    std::shared_mutex* m = __atomic_load_n(slot, __ATOMIC_ACQUIRE);
    if (m) return m;
    auto* fresh = new std::shared_mutex;
    std::shared_mutex* expected = nullptr;
    if (__atomic_compare_exchange_n(slot, &expected, fresh, false, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) return fresh;
    delete fresh;
    return expected;
}
void WINAPI InitializeSRWLock(PSRWLOCK l) { l->Ptr = nullptr; }
void WINAPI AcquireSRWLockExclusive(PSRWLOCK l) { srw(l)->lock(); }
void WINAPI ReleaseSRWLockExclusive(PSRWLOCK l) { srw(l)->unlock(); }
void WINAPI AcquireSRWLockShared(PSRWLOCK l) { srw(l)->lock_shared(); }
void WINAPI ReleaseSRWLockShared(PSRWLOCK l) { srw(l)->unlock_shared(); }

// ---- threads ------------------------------------------------------------------------------
struct ThreadStart {
    LPTHREAD_START_ROUTINE fn;
    void* param;
    std::shared_ptr<Thread> self;
    std::mutex gate;
    std::condition_variable cv;
    bool go = false;
};
static void* threadMain(void* p) {
    auto* s = static_cast<ThreadStart*>(p);
    t_self = s->self;
    t_tid = s->self->tid;
    {
        std::unique_lock<std::mutex> l(s->gate);
        s->cv.wait(l, [&] { return s->go; });
    }
    const LPTHREAD_START_ROUTINE fn = s->fn;
    void* param = s->param;
    std::shared_ptr<Thread> self = s->self;
    delete s;
    const DWORD code = fn(param);
    std::lock_guard<std::mutex> l(waitLock());
    self->exitCode = code;
    self->done = true;
    notifyAll();
    return nullptr;
}
static std::mutex g_startLock;
static std::map<Thread*, ThreadStart*> g_suspended;

HANDLE WINAPI CreateThread(LPSECURITY_ATTRIBUTES, SIZE_T stack, LPTHREAD_START_ROUTINE fn, LPVOID param, DWORD flags, LPDWORD tid) {
    auto t = std::make_shared<Thread>();
    t->tid = g_nextTid.fetch_add(4);
    auto* s = new ThreadStart{fn, param, t, {}, {}, false};
    pthread_attr_t a;
    pthread_attr_init(&a);
    size_t size = stack < (8u << 20) ? (8u << 20) : stack;
    pthread_attr_setstacksize(&a, size);
    pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);
    pthread_t th;
    const int r = pthread_create(&th, &a, threadMain, s);
    pthread_attr_destroy(&a);
    if (r != 0) { delete s; setError(ERROR_NOT_ENOUGH_MEMORY); return nullptr; }
    if (tid) *tid = t->tid;
    if (flags & CREATE_SUSPENDED) {
        t->suspended = true;
        std::lock_guard<std::mutex> l(g_startLock);
        g_suspended[t.get()] = s;
    } else {
        std::lock_guard<std::mutex> l(s->gate);
        s->go = true;
        s->cv.notify_all();
    }
    return newHandle(t);
}
DWORD WINAPI ResumeThread(HANDLE h) {
    auto t = objectAs<Thread>(h);
    if (!t) { setError(ERROR_INVALID_HANDLE); return static_cast<DWORD>(-1); }
    ThreadStart* s = nullptr;
    {
        std::lock_guard<std::mutex> l(g_startLock);
        auto it = g_suspended.find(t.get());
        if (it == g_suspended.end()) return 0;
        s = it->second;
        g_suspended.erase(it);
    }
    t->suspended = false;
    std::lock_guard<std::mutex> l(s->gate);
    s->go = true;
    s->cv.notify_all();
    return 1;
}
BOOL WINAPI GetExitCodeThread(HANDLE h, LPDWORD code) {
    auto t = objectAs<Thread>(h);
    if (!t) return fail(ERROR_INVALID_HANDLE);
    std::lock_guard<std::mutex> l(waitLock());
    *code = t->exitCode;
    return TRUE;
}
void WINAPI ExitThread(DWORD code) {
    if (t_self) {
        std::lock_guard<std::mutex> l(waitLock());
        t_self->exitCode = code;
        t_self->done = true;
        notifyAll();
    }
    pthread_exit(nullptr);
}
HANDLE WINAPI GetCurrentThread(void) { return reinterpret_cast<HANDLE>(static_cast<intptr_t>(-2)); }
DWORD WINAPI GetCurrentThreadId(void) { return currentTid(); }
HANDLE WINAPI GetCurrentProcess(void) { return reinterpret_cast<HANDLE>(static_cast<intptr_t>(-1)); }
DWORD WINAPI GetCurrentProcessId(void) { return static_cast<DWORD>(getpid()); }
BOOL WINAPI SetThreadPriority(HANDLE h, int p) {
    if (auto t = objectAs<Thread>(h)) t->priority = p;
    return TRUE;
}
int WINAPI GetThreadPriority(HANDLE h) {
    if (auto t = objectAs<Thread>(h)) return t->priority;
    return THREAD_PRIORITY_NORMAL;
}
BOOL WINAPI SetPriorityClass(HANDLE, DWORD) { return TRUE; }
DWORD WINAPI GetPriorityClass(HANDLE) { return NORMAL_PRIORITY_CLASS; }

// ---- sleeping -----------------------------------------------------------------------------
void WINAPI Sleep(DWORD ms) {
    if (ms == 0) std::this_thread::yield();
    else std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
DWORD WINAPI SleepEx(DWORD ms, BOOL alertable) {
    if (!alertable) { Sleep(ms); return 0; }
    const DWORD tid = currentTid();
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms);
    std::unique_lock<std::mutex> lk(waitLock());
    for (;;) {
        if (hasApcs(tid)) {
            lk.unlock();
            runApcs();
            return WAIT_IO_COMPLETION;
        }
        if (ms == 0) return 0;
        if (ms == INFINITE) waitCv().wait(lk);
        else if (waitCv().wait_until(lk, deadline) == std::cv_status::timeout) return 0;
    }
}
BOOL WINAPI SwitchToThread(void) { sched_yield(); return TRUE; }

// ---- TLS ----------------------------------------------------------------------------------
static pthread_key_t g_tls[1088];
static std::atomic<uint32_t> g_tlsNext{0};
DWORD WINAPI TlsAlloc(void) {
    const uint32_t i = g_tlsNext.fetch_add(1);
    if (i >= 1088 || pthread_key_create(&g_tls[i], nullptr) != 0) return TLS_OUT_OF_INDEXES;
    return i;
}
LPVOID WINAPI TlsGetValue(DWORD i) { setError(ERROR_SUCCESS); return pthread_getspecific(g_tls[i]); }
BOOL WINAPI TlsSetValue(DWORD i, LPVOID v) { return pthread_setspecific(g_tls[i], v) == 0; }
BOOL WINAPI TlsFree(DWORD) { return TRUE; }

}  // extern "C"
