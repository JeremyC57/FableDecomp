// MicroThread::MicroThreadSleep() -- the micro-thread stack switch: park the running side (ebx ebp esi edi and the SEH chain head
// fs:[0] pushed on its own stack), publish its stack pointer in MicroThread's parked-stack global and resume the side parked
// there (load its esp, pop its chain head and registers, return on its stack). The scheduler (SwitchToMicroThread 0x009D87F0) and
// the micro-threads (CSuspendableProcessMicrothread::InternalSuspend 0x00A44690, ThreadEntry 0x00A446A0, MicroThreadReturn
// 0x009D8690) all call it: one call enters the thread, the next call from the thread returns to the scheduler.
// Identity: ?MicroThreadSleep@MicroThread@@YIXXZ (__fastcall, no arguments) -- Ego_d.pdb lib_microthread.obj order DeleteMicroThread /
//   MicroThreadSleep / MicroThreadReturn / SwitchToMicroThread; FableWin.pdb 0x02F6CA00 and Ego_r.pdb 0x00B2B980 (each followed by
//   the MicroThreadSleep(int) overload and MicroThreadReturn = retail 0x009D8680 / 0x009D8690); reloc_bindings (caller 0x00A44690).
// Global 0x013CA898: `PGlobalStackPointer` -- Ego_d.pdb S_LDATA32 in lib_microthread.obj (type 0x0420 unsigned char*) and the
//   Fable Anniversary Xbox 360 PDB S_LDATA32 of the same name; Ego_d's TU statics PActiveScript / PGlobalStackPointer /
//   ThreadStackSize / PScriptStackTop / PScriptStackTopWithChecksum sit 4 bytes apart in the order of retail 0x013CA894..0x013CA8A4.
//   A file static in the original (LDATA32: the record carries no namespace; assumed MicroThread like DummyExceptionBlock).
//
// Retail-asm-helper: FABLE_MICROTHREAD_SLEEP_DEFINITION
// Retail-asm-function: 0x009D8650, all 36 bytes hand asm with no compiler prologue: VC7.1 never saves ebp around an inline-asm
//   block (probes nn1-nn4: 28 / 42 / 40 / 42 bytes, compiler pushes ebx esi edi first), retail pushes ebp second; Ego_d.pdb /Od
//   length 36 = retail (a debug C++ body has an ebp frame and /RTC fill). Read by the proposed purity.py change only.
// Retail-asm: 0x009D8650 `push ebx` / 0x009D8651 `push ebp` / 0x009D8652 `push esi` / 0x009D8653 `push edi`, 0x009D8654
//   `mov eax, dword ptr fs:[0]` + 0x009D865A `push eax`, 0x009D865B `mov eax, esp`, 0x009D865D `mov esp, dword ptr [0x013CA898]`,
//   0x009D8663 `mov dword ptr [0x013CA898], eax`, 0x009D8668 `pop eax` + 0x009D8669 `mov dword ptr fs:[0], eax`, 0x009D866F..72
//   `pop edi` / `pop esi` / `pop ebp` / `pop ebx`, 0x009D8673 `ret` (36 bytes, no compiler prologue).
//   Why asm: the body loads esp from memory (0x009D865D); no VC7.1 C++ spelling or intrinsic writes esp (rule-4 table: 32 C++ and
//   12 intrinsic spellings graded, all DIFFER: the asm1 hand-back README.md). __readfsdword / __writefsdword DO exist in VC7.1 RTM and
//   emit the 64 A1 / 64 A3 forms, but the chain head travels through push eax / pop eax on the switched stacks, which no intrinsic
//   does. Why a naked definition: VC7.1 never saves ebp around an inline-asm block and puts its own push ebx / esi / edi first
//   (probes: 28 / 40 / 42 bytes), so retail's push ebx / push ebp / push esi / push edi frame exists only as hand asm in a function
//   without a compiler prologue; Ego_d.pdb's /Od body is 36 bytes too (a debug C++ body has a frame and /RTC fill), so the
//   original source defined it the same way.
//
// Contract (what callers rely on): after MicroThreadSleep returns on side B, PGlobalStackPointer holds side A's resume token and
// side B runs with its own callee-saved registers and its own exception-handler chain exactly as it left them. Retail token =
// A's saved esp, pointing at the 24-byte frame { chain head, edi, esi, ebp, ebx, return address } on A's stack; that layout is
// shared with CreateMicroThread 0x009D8710 (which fabricates a first frame { &DummyExceptionBlock, 0, 0, 0, 0, entry,
// MicroThreadReturn, argument }) and SwitchToMicroThread 0x009D87F0 (RestoreStack points the global at StackTop - image size,
// PreserveStack copies [global, StackTop) back into the thread's image).
//
// Portable twin (FABLE_PORTABLE; owner 2026-11-18: "as long as it does not affect matching or portability"). ISO C++ cannot switch
// stacks, so the portable build gives every micro-thread a context and a stack of its own and switches with the host's context
// API; nothing is copied between stacks and nothing depends on frame layout or one ABI:
//   * The thread's `Stack` vector (MicroThreadDetails +8) holds a MicroThreadContext record and, on POSIX, the thread's own stack
//     (CreateMicroThread 0x009D8710's portable branch: MicroThreadImageBytes / MicroThreadInit). The vector is never resized after
//     that (SwitchToMicroThread 0x009D87F0's portable RestoreStack / PreserveStack only move the token), so the record stays put.
//   * The token in PGlobalStackPointer is the parked side's context: a ucontext_t* (POSIX; the parked side's `self`, on its own
//     stack, which never moves) or a fibre handle (Win32). RestoreStack publishes the thread's token (MicroThreadResumeToken),
//     PreserveStack stores the token the thread parked with (MicroThreadParkToken): retail's protocol, without the image copy.
//   * First entry: retail's fabricated frame returns into entry(argument), whose return address is MicroThreadReturn 0x009D8690;
//     the twin's start routine calls entry(argument), then MicroThreadReturn (which marks the thread dead and parks it forever).
//   * Each side keeps its callee-saved registers and FP environment (swapcontext / SwitchToFiber), its locals (its own stack) and its
//     exception handlers (table-driven unwinding per stack on POSIX hosts; the TEB ExceptionList swap of SwitchToFiber on Win32 x86),
//     as retail's fs:[0] swap does. ~MicroThreadDetails 0x009D88D0's portable branch releases a Win32 fibre (MicroThreadRelease).
//   * Hosts: POSIX ucontext (getcontext / makecontext / swapcontext: glibc, the BSDs, macOS with _XOPEN_SOURCE) on any ABI, and Win32
//     fibres. A host with neither (e.g. musl) needs one more branch here; no code outside this header changes.
//   * Stack size: MicroThreadStackBytes (256 KB) per thread on POSIX (retail runs every thread on the one stack SetStack 0x009D86B0
//     installs, so it has no per-thread size); Win32 fibres take the same reserve.
// Test: rebuild/tests/portable/test_MicroThreadSleep.cpp (asm path S + C, Win32 twin S, POSIX twin S + C; every build mode of
//   tools/decomp_pipeline/portable_twins_check.py).
// Use: invoke FABLE_MICROTHREAD_SLEEP_DEFINITION once, inside namespace MicroThread, on a line of its own (lib_microthread.obj's
// row 0x009D8650). Callers include this header for the declaration only.
#ifndef FABLE_ENGINE_MICROTHREADSLEEP_H
#define FABLE_ENGINE_MICROTHREADSLEEP_H
#ifndef FABLE_FASTCALL                              // rebuild_abi.h's spelling (identical tokens); not included: its 32-bit pointer
#if defined(_MSC_VER)                               // check rejects the 64-bit host build of the portable test
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
#endif

namespace MicroThread
{
    extern unsigned char* PGlobalStackPointer;      // 0x013CA898 (the parked side's resume token)
    void FABLE_FASTCALL MicroThreadSleep();         // 0x009D8650
}

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#include <stdlib.h>
#if defined(_MSC_VER)
extern "C" __declspec(dllimport) void __stdcall SwitchToFiber(void* fiber);   // kernel32 (declared by hand: no <windows.h>)
extern "C" __declspec(dllimport) void* __stdcall ConvertThreadToFiber(void* parameter);
extern "C" __declspec(dllimport) void* __stdcall CreateFiber(unsigned long stackSize, void (__stdcall *start)(void*), void* parameter);
extern "C" __declspec(dllimport) void __stdcall DeleteFiber(void* fiber);
#else
#include <ucontext.h>
#endif
namespace fable_portable
{
    enum { MicroThreadStackBytes = 0x40000 };

    // One micro-thread's context: the first 16-byte-aligned bytes of its Stack image.
    struct MicroThreadContext
    {
        void (*Entry)(void*);                       // CreateMicroThread's entry and argument
        void* Argument;
        void (FABLE_FASTCALL *Return)();            // MicroThreadReturn 0x009D8690
        unsigned char* Resume;                      // the token this thread resumes from (PGlobalStackPointer when it runs next)
#if defined(_MSC_VER)
        void* Fiber;
#else
        int Started;
        ucontext_t Start;                           // the first context (makecontext'd onto the stack that follows)
#endif
    };

    inline MicroThreadContext* MicroThreadRecord(unsigned char* image)
    {
        return (MicroThreadContext*)(((size_t)image + 15) & ~(size_t)15);
    }

    // Bytes CreateMicroThread's portable branch inserts into a new thread's Stack (record + alignment, + the stack on POSIX).
    inline unsigned int MicroThreadImageBytes()
    {
#if defined(_MSC_VER)
        return (unsigned int)(sizeof(MicroThreadContext) + 15);
#else
        return (unsigned int)(sizeof(MicroThreadContext) + 15 + MicroThreadStackBytes);
#endif
    }

    // A zero image to insert from (static storage: no stack or heap cost at the call site).
    inline const unsigned char* MicroThreadZeroImage()
    {
        static unsigned char zeros[sizeof(MicroThreadContext) + 15 + MicroThreadStackBytes];
        return zeros;
    }

    inline void MicroThreadRun(MicroThreadContext* c)
    {
        c->Entry(c->Argument);
        c->Return();                                // never comes back: the thread is dead and parked for good
        abort();
    }

#if defined(_MSC_VER)
    // The fibre that is running now (retail needs no such variable: the running side is wherever esp points). Set on the first
    // switch from the thread that runs the scheduler.
    inline void*& MicroThreadRunningFiber()
    {
        static void* fiber = 0;
        return fiber;
    }

    inline void __stdcall MicroThreadFiberStart(void* context)
    {
        MicroThreadRun((MicroThreadContext*)context);
    }

    inline void MicroThreadInit(unsigned char* image, void (*entry)(void*), void* argument, void (FABLE_FASTCALL *ret)())
    {
        MicroThreadContext* c = MicroThreadRecord(image);
        c->Entry = entry;
        c->Argument = argument;
        c->Return = ret;
        c->Fiber = CreateFiber(MicroThreadStackBytes, MicroThreadFiberStart, c);
        c->Resume = (unsigned char*)c->Fiber;
    }

    inline void MicroThreadRelease(unsigned char* image)
    {
        MicroThreadContext* c = MicroThreadRecord(image);
        if (c->Fiber)
            DeleteFiber(c->Fiber);
        c->Fiber = 0;
    }

    inline void MicroThreadSleep(unsigned char*& parked)
    {
        if (!MicroThreadRunningFiber())
            MicroThreadRunningFiber() = ConvertThreadToFiber(0);
        void* resume = parked;
        parked = (unsigned char*)MicroThreadRunningFiber();
        MicroThreadRunningFiber() = resume;
        SwitchToFiber(resume);
    }
#else
    // The thread being entered for the first time (makecontext passes only ints: no pointer argument on 64-bit hosts).
    inline MicroThreadContext*& MicroThreadStarting()
    {
        static MicroThreadContext* starting = 0;
        return starting;
    }

    inline void MicroThreadContextStart()
    {
        MicroThreadRun(MicroThreadStarting());
    }

    inline void MicroThreadInit(unsigned char* image, void (*entry)(void*), void* argument, void (FABLE_FASTCALL *ret)())
    {
        MicroThreadContext* c = MicroThreadRecord(image);
        c->Entry = entry;
        c->Argument = argument;
        c->Return = ret;
        c->Started = 0;
        getcontext(&c->Start);
        c->Start.uc_stack.ss_sp = (unsigned char*)(c + 1);
        c->Start.uc_stack.ss_size = MicroThreadStackBytes;
        c->Start.uc_link = 0;
        makecontext(&c->Start, MicroThreadContextStart, 0);
        c->Resume = (unsigned char*)&c->Start;
    }

    inline void MicroThreadRelease(unsigned char*) { }   // the stack is the image itself: freed with the vector

    // Never inlined, so the parked side's `self` lives in this call's own frame on that side's stack.
#if defined(__GNUC__)
    __attribute__((noinline))
#endif
    inline void MicroThreadSleep(unsigned char*& parked)
    {
        ucontext_t self;                            // on this side's own stack, like retail's pushed frame
        ucontext_t* resume = (ucontext_t*)parked;
        parked = (unsigned char*)&self;
        swapcontext(&self, resume);
    }
#endif

    // SwitchToMicroThread's portable RestoreStack: the token the thread resumes from.
    inline unsigned char* MicroThreadResumeToken(unsigned char* image)
    {
        MicroThreadContext* c = MicroThreadRecord(image);
#if !defined(_MSC_VER)
        if (!c->Started) {
            c->Started = 1;
            MicroThreadStarting() = c;
        }
#endif
        return c->Resume;
    }

    // SwitchToMicroThread's portable PreserveStack: keep the token the thread parked with.
    inline void MicroThreadParkToken(unsigned char* image, unsigned char* token)
    {
        MicroThreadRecord(image)->Resume = token;
    }
}
#endif

#ifdef FABLE_PORTABLE
#define FABLE_MICROTHREAD_SLEEP_DEFINITION \
    void FABLE_FASTCALL MicroThreadSleep() { ::fable_portable::MicroThreadSleep(PGlobalStackPointer); }
#else
#define FABLE_MICROTHREAD_SLEEP_DEFINITION \
    __declspec(naked) void __fastcall MicroThreadSleep() { \
        __asm push ebx \
        __asm push ebp \
        __asm push esi \
        __asm push edi \
        __asm mov eax, dword ptr fs:[0] \
        __asm push eax \
        __asm mov eax, esp \
        __asm mov esp, PGlobalStackPointer \
        __asm mov PGlobalStackPointer, eax \
        __asm pop eax \
        __asm mov dword ptr fs:[0], eax \
        __asm pop edi \
        __asm pop esi \
        __asm pop ebp \
        __asm pop ebx \
        __asm ret \
    }
#endif

#endif // FABLE_ENGINE_MICROTHREADSLEEP_H
