// test_MicroThreadSleep.cpp -- dual-path test of engine/MicroThreadSleep.h (the micro-thread stack switch, row 0x009D8650).
// A switch has no golden value: the test drives two micro-threads with a scheduler and checks what callers rely on -- the
// interleaving (each call resumes the parked side right after ITS call), both sides' locals surviving the switches (register /
// frame restore), a C++ exception thrown after a yield inside a try being caught by THAT side's handler, on the thread and on the
// scheduler (per-side handler chains: retail parks and restores the fs:[0] chain head), and the parked-side token protocol of
// the global. Scenarios:
//   S  each micro-thread on a stack region of its own; the scheduler keeps each thread's parked token;
//   C  the retail module design (asm path only): one shared stack, each thread's stack image copied under StackTop before the
//      switch and back after it (SwitchToMicroThread 0x009D87F0's RestoreStack / PreserveStack, same addresses every time);
//   P  the portable module design (twins): each thread's image holds its own context and stack, made by CreateMicroThread
//      0x009D8710's portable branch (MicroThreadInit) and switched by SwitchToMicroThread's portable RestoreStack / PreserveStack
//      (MicroThreadResumeToken / MicroThreadParkToken): no stack is copied and no frame layout is relied on.
// First entry of a thread (CreateMicroThread's job, per path): asm path = CreateMicroThread 0x009D8710's first frame
// { &DummyExceptionBlock, 0, 0, 0, 0, entry, MicroThreadReturn, argument }; twins, scenario S = a makecontext'd ucontext_t /
// CreateFiber made here; scenario P = the header's MicroThreadInit, exactly as CreateMicroThread's portable branch calls it.
// Build modes (tools/decomp_pipeline/portable_twins_check.py): vc71-asm = asm path, S + C; vc71-dual = asm path S + C and the Win32
// twin S + P; vc71-portable = Win32 twin S + P; gxx-portable = POSIX twin S + P. Every run is compared with the same expected
// trace, so the paths are compared with each other through it.
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "engine/MicroThreadSleep.h"
#include "fable_test_mode.h"

namespace MicroThread
{
    unsigned char* PGlobalStackPointer;
    FABLE_MICROTHREAD_SLEEP_DEFINITION
}

#if defined(FABLE_PORTABLE) && !defined(_MSC_VER)
#define TEST_ROW_IS_UCONTEXT 1      // the row's definition is the POSIX twin
#elif defined(FABLE_PORTABLE)
#define TEST_ROW_IS_FIBRE 1         // the row's definition is the Win32 twin
#else
#define TEST_ROW_IS_ASM 1           // the row's definition is the retail asm path
#endif
#if defined(_MSC_VER) && (defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST))
#define TEST_HAVE_FIBRES 1
extern "C" __declspec(dllimport) void* __stdcall ConvertThreadToFiber(void* parameter);
extern "C" __declspec(dllimport) void* __stdcall CreateFiber(unsigned long stackSize, void (__stdcall *start)(void*), void* parameter);
extern "C" __declspec(dllimport) void __stdcall DeleteFiber(void* fiber);
#endif

enum { kThreads = 2, kRegion = 0x8000, kImageMax = 0x6000, kTrace = 64 };
enum Kind { kAsmFrame, kFibre, kUcontext, kPortableModule };

static void (*g_sleep)();
static int g_trace[kTrace];
static int g_count;
static int g_done[kThreads];
static unsigned long g_sum[kThreads];
static int g_bad;
static unsigned char* g_lo[kThreads];
static unsigned char* g_hi[kThreads];

static void Trace(int v)
{
    if (g_count < kTrace)
        g_trace[g_count++] = v;
}

static void SleepRow() { MicroThread::MicroThreadSleep(); }
#if defined(FABLE_DUAL_PATH_TEST) && !defined(FABLE_PORTABLE)
static void SleepTwin() { fable_portable::MicroThreadSleep(MicroThread::PGlobalStackPointer); }
#endif

static void Body(int id)
{
    unsigned long sum = 17 + id;
    Trace(100 + id);
    for (int k = 0; k < 3; ++k) {
        Trace(200 + id * 10 + k);
        try {
            g_sleep();                                  // yield inside this side's try
            sum = sum * 31 + k;
            if ((k + id) & 1)
                throw 1000 + id * 10 + k;
            Trace(300 + id * 10 + k);
        }
        catch (int v) {
            Trace(400 + id * 10 + k);
            if (v != 1000 + id * 10 + k)
                ++g_bad;
        }
    }
    g_sum[id] = sum;
    g_done[id] = 1;
    Trace(500 + id);
    for (;;)
        g_sleep();
}

static int InRange(const void* p, int t)
{
    return (const unsigned char*)p >= g_lo[t] && (const unsigned char*)p < g_hi[t];
}

// ---- first entry, per path ----
#ifdef TEST_ROW_IS_ASM
struct SInitialStackFrame                               // CreateMicroThread 0x009D8710's image (32-bit words)
{
    void* ExceptionChain;
    unsigned long Edi, Esi, Ebp, Ebx;
    void (__cdecl *Entry)(void*);
    void (__cdecl *Return)();
    void* Argument;
};
static struct { void* Next; void* Handler; } g_dummyExceptionBlock = { (void*)-1, 0 };   // 0x0139650C
static void __cdecl AsmEntry(void* argument) { Body((int)(size_t)argument); }
static void __cdecl ReturnTrap() { ++g_bad; for (;;) g_sleep(); }                       // MicroThreadReturn's slot
static void FirstFrame(SInitialStackFrame* f, int id)
{
    f->ExceptionChain = &g_dummyExceptionBlock;
    f->Edi = f->Esi = f->Ebp = f->Ebx = 0;
    f->Entry = AsmEntry;
    f->Return = ReturnTrap;
    f->Argument = (void*)(size_t)id;
}
#endif
#ifdef TEST_HAVE_FIBRES
static void __stdcall FibreEntry(void* argument) { Body((int)(size_t)argument); }
#endif
#ifndef _MSC_VER
static ucontext_t g_firstContext[kThreads];             // scenario S first contexts (static: no stack image involved)
static void UcEntry(int id) { Body(id); }
static void MakeFirstContext(ucontext_t* c, unsigned char* stackLow, size_t stackSize, int id)   // getcontext returns twice:
{                                                                                              // kept out of Run's frame
    getcontext(c);
    c->uc_stack.ss_sp = stackLow;
    c->uc_stack.ss_size = stackSize;
    c->uc_link = 0;
    makecontext(c, (void (*)())UcEntry, 1, id);
}
#endif

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#define TEST_HAVE_PORTABLE_MODULE 1
static unsigned char g_portableImage[kThreads][sizeof(fable_portable::MicroThreadContext) + 15 + fable_portable::MicroThreadStackBytes];
static void PortableEntry(void* argument) { Body((int)(size_t)argument); }
static void FABLE_FASTCALL PortableReturnTrap() { ++g_bad; for (;;) g_sleep(); }   // MicroThreadReturn's slot
#endif

static unsigned char* g_stackTop;
static unsigned char g_image[kThreads][kImageMax];
static size_t g_imageSize[kThreads];

static void SwitchTo(int t, int shared, Kind kind, unsigned char** token)
{
#ifdef TEST_HAVE_PORTABLE_MODULE
    if (kind == kPortableModule) {                      // SwitchToMicroThread's portable RestoreStack / PreserveStack
        MicroThread::PGlobalStackPointer = fable_portable::MicroThreadResumeToken(g_portableImage[t]);
        g_sleep();
        fable_portable::MicroThreadParkToken(g_portableImage[t], MicroThread::PGlobalStackPointer);
#ifndef _MSC_VER
        if (!InRange(MicroThread::PGlobalStackPointer, t))   // POSIX: the token is the thread's `self`, on its own stack
            ++g_bad;
#endif
        return;
    }
#endif
    if (shared) {
        MicroThread::PGlobalStackPointer = g_stackTop - g_imageSize[t];                        // RestoreStack
        memmove(MicroThread::PGlobalStackPointer, g_image[t], g_imageSize[t]);
    }
    else
        MicroThread::PGlobalStackPointer = token[t];
    g_sleep();
    unsigned char* parked = MicroThread::PGlobalStackPointer;
    if (kind != kFibre && !InRange(parked, t))           // the token is on the parked thread's own stack
        ++g_bad;
#ifdef TEST_ROW_IS_ASM
    if (kind == kAsmFrame) {                             // { chain, edi, esi, ebp, ebx, return }: chain and ebp on its stack
        unsigned long* f = (unsigned long*)parked;
        if (!InRange((void*)f[0], t) || !InRange((void*)f[3], t) || f[5] == 0)
            ++g_bad;
    }
#endif
    if (shared) {
        g_imageSize[t] = (size_t)(g_stackTop - parked);                                        // PreserveStack
        if (g_imageSize[t] > kImageMax)
            g_imageSize[t] = kImageMax, ++g_bad;
        memmove(g_image[t], parked, g_imageSize[t]);
    }
    else
        token[t] = parked;
}

static int Run(const char* name, Kind kind, int shared, unsigned char* stack, size_t size)
{
    static const int kExpected[24] = {
        100, 200, 600, 101, 210, 601, 300, 201, 710, 410, 211, 711,
        401, 202, 620, 311, 212, 621, 302, 500, 730, 412, 501, 731 };
    unsigned char* token[kThreads];
    unsigned long schedulerSum = 5;
    g_count = 0;
    g_bad = 0;
    g_stackTop = stack + size - 16;
    for (int t = 0; t < kThreads; ++t) {
        g_done[t] = 0;
        g_sum[t] = 0;
        g_lo[t] = shared ? stack : stack + t * (size / kThreads);
        g_hi[t] = shared ? g_stackTop : g_lo[t] + size / kThreads;
        token[t] = 0;
#ifdef TEST_HAVE_PORTABLE_MODULE
        if (kind == kPortableModule) {                  // CreateMicroThread's portable branch
            memset(g_portableImage[t], 0, sizeof(g_portableImage[t]));
            fable_portable::MicroThreadInit(g_portableImage[t], PortableEntry, (void*)(size_t)t, PortableReturnTrap);
            g_lo[t] = g_portableImage[t];
            g_hi[t] = g_portableImage[t] + sizeof(g_portableImage[t]);
        }
#endif
#ifdef TEST_ROW_IS_ASM
        if (kind == kAsmFrame) {
            SInitialStackFrame* f = shared ? (SInitialStackFrame*)g_image[t] : (SInitialStackFrame*)(g_hi[t] - 16) - 1;
            FirstFrame(f, t);
            g_imageSize[t] = sizeof(SInitialStackFrame);
            token[t] = (unsigned char*)f;
        }
#endif
#ifdef TEST_HAVE_FIBRES
        if (kind == kFibre)
            token[t] = (unsigned char*)CreateFiber(0x10000, FibreEntry, (void*)(size_t)t);
#endif
#ifndef _MSC_VER
        if (kind == kUcontext && !shared) {
            MakeFirstContext(&g_firstContext[t], g_lo[t], (size_t)(g_hi[t] - g_lo[t]) - 64, t);
            token[t] = (unsigned char*)&g_firstContext[t];
        }
#endif
    }
    for (int round = 0; round < 8 && !(g_done[0] && g_done[1]); ++round) {
        for (int t = 0; t < kThreads; ++t) {
            if (g_done[t])
                continue;
            try {
                SwitchTo(t, shared, kind, token);
                schedulerSum = schedulerSum * 7 + round * 2 + t;
                if (round & 1)
                    throw -round;
                Trace(600 + round * 10 + t);
            }
            catch (int v) {
                Trace(700 + round * 10 + t);
                if (v != -round)
                    ++g_bad;
            }
        }
    }
#ifdef TEST_HAVE_FIBRES
    if (kind == kFibre)
        for (int t = 0; t < kThreads; ++t)
            DeleteFiber(token[t]);
#endif
#ifdef TEST_HAVE_PORTABLE_MODULE
    if (kind == kPortableModule)                        // ~MicroThreadDetails's portable branch
        for (int t = 0; t < kThreads; ++t)
            fable_portable::MicroThreadRelease(g_portableImage[t]);
#endif
    int bad = g_bad + (g_count != 24);
    for (int i = 0; i < 24 && i < g_count; ++i)
        if (g_trace[i] != kExpected[i])
            ++bad;
    for (int t = 0; t < kThreads; ++t)
        if (g_sum[t] != (((17ul + t) * 31 + 0) * 31 + 1) * 31 + 2)
            ++bad;
    unsigned long want = 5;
    for (int r = 0; r < 4; ++r)
        for (int t = 0; t < kThreads; ++t)
            want = want * 7 + r * 2 + t;
    if (schedulerSum != want)
        ++bad;
    printf("  %-34s %s (%d trace points, %d bad)\n", name, bad ? "FAIL" : "ok", g_count, bad);
    return bad;
}

int main()
{
    unsigned char stack[kThreads * kRegion];            // on the real stack (Win32 SEH accepts handler frames only within it)
    memset(stack, 0, sizeof(stack));
    int bad = 0;
#ifdef TEST_ROW_IS_ASM
    g_sleep = SleepRow;
    bad += Run("asm path, S (separate stacks)", kAsmFrame, 0, stack, sizeof(stack));
    bad += Run("asm path, C (shared stack + copy)", kAsmFrame, 1, stack, sizeof(stack));
#endif
#if defined(TEST_HAVE_FIBRES)
#ifdef TEST_ROW_IS_FIBRE
    g_sleep = SleepRow;
#else
    g_sleep = SleepTwin;
#endif
    fable_portable::MicroThreadRunningFiber() = ConvertThreadToFiber(0);
    bad += Run("Win32 twin, S (fibres)", kFibre, 0, stack, sizeof(stack));
    bad += Run("Win32 twin, P (portable module)", kPortableModule, 0, stack, sizeof(stack));
#endif
#ifdef TEST_ROW_IS_UCONTEXT
    g_sleep = SleepRow;
    bad += Run("POSIX twin, S (ucontext)", kUcontext, 0, stack, sizeof(stack));
    bad += Run("POSIX twin, P (portable module)", kPortableModule, 0, stack, sizeof(stack));
#endif
    printf("test_MicroThreadSleep %s: %s\n", kFableTestMode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
