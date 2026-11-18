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
// Portable twin (FABLE_PORTABLE; fable_portable::MicroThreadSleep): NOT identical semantics -- owner decision. ISO C++ cannot
// switch stacks, so the twin uses the host's context switch and keeps the protocol, not the bytes:
//   * POSIX (ucontext): the parked side's ucontext_t is a local of the twin, i.e. it lives on the parked side's own stack where
//     retail pushes its 24-byte frame, and the token is its address -- the same "save on my stack, publish my stack address,
//     resume the published one" protocol. swapcontext saves / restores every callee-saved register (and the signal mask);
//     C++ exception handling is table-driven on these hosts, so each side's handlers belong to its own stack by construction
//     (the analogue of the fs:[0] swap). Because the token points into the parked side's stack, SwitchToMicroThread's stack-image
//     copying (RestoreStack / PreserveStack, same addresses) keeps working: the dual-path test runs exactly that (scenario C, g++ and
//     clang++ -O0..-O3). This needs `self` to be the lowest live object of the parked side, hence noinline: true on x86-64 SysV
//     GCC / Clang, not guaranteed by C++ nor on ABIs that keep the frame record below the locals (e.g. AArch64).
//   * Win32 (fibres; there is no ucontext and no C++ route to esp): the token is the parked side's fibre handle and the running
//     fibre is tracked in fable_portable::MicroThreadRunningFiber(); SwitchToFiber swaps registers, stack bounds and the SEH
//     chain head (x86 TEB ExceptionList). The fibres have stacks of their own, so the stack-image copying does not apply.
//   Not shared on any host: the token's representation (esp vs ucontext_t* vs fibre handle), the 24-byte saved-frame layout and
//   absolute stack addresses -- so CreateMicroThread's fabricated first frame (and, on Win32, SwitchToMicroThread's image
//   copying) need portable branches of their own before a portable build can create a micro-thread. No FPU state is involved
//   on the asm path (x87 control word / SSE state are not saved; the POSIX twin saves the FP environment, a superset).
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
#if defined(_MSC_VER)
extern "C" __declspec(dllimport) void __stdcall SwitchToFiber(void* fiber);   // kernel32 (declared by hand: no <windows.h>)
#else
#include <ucontext.h>
#endif
namespace fable_portable
{
#if defined(_MSC_VER)
    // The fibre that is running now (retail needs no such variable: the running side is wherever esp points). The code that
    // creates the fibres (ConvertThreadToFiber for the scheduler) sets it once.
    inline void*& MicroThreadRunningFiber()
    {
        static void* fiber = 0;
        return fiber;
    }

    inline void MicroThreadSleep(unsigned char*& parked)
    {
        void* resume = parked;
        parked = (unsigned char*)MicroThreadRunningFiber();
        MicroThreadRunningFiber() = resume;
        SwitchToFiber(resume);
    }
#else
    // Never inlined: `self` must be the lowest live object of the parked side's stack, so that a stack image copied from the
    // token upwards (SwitchToMicroThread's PreserveStack / RestoreStack) holds everything the side needs when it resumes.
    // Inlined into a caller, the caller's own locals can sit below `self` (Clang -O1 put Body's argument there: lost on copy).
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
