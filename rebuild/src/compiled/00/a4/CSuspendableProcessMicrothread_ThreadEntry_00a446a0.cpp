// CSuspendableProcessMicrothread::ThreadEntry(void* param) -- the fibre body of a suspendable process: until the process is told to stop, mark it running, run its SuspendableProcess (swallowing the CExceptionExit a terminated process throws) and suspend back to the scheduler; then mark it stopped and sleep forever
// Leaf: ?ThreadEntry@CSuspendableProcessMicrothread@@CAXPAX@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A446A0, 158 bytes (to the `ret` at 0x00A4473D), __cdecl, EH frame (handler 0x012109F0; its one
//   catch block takes .?AVCExceptionExit@CASuspendableProcess@@ by reference, catch object [ebp-18h]).
// Identity: the entry the landed constructor 0x00A44740 hands to MicroThread::CreateMicroThread; Ego_r.pdb
//   ?ThreadEntry@CSuspendableProcessMicrothread@@CAXPAX@Z (0x00B9E2A0, private static), between InternalSuspend 0x00A44690 and the
//   constructor as in retail. Callees: slot 4 SuspendableProcess (+0x10), slot 2 SuspendSuspendableProcess (+0x08),
//   MicroThread::MicroThreadSleep 0x009D8650 (?MicroThreadSleep@MicroThread@@YIXXZ).
// Extent: the catalog cut this one function into five rows (0x00A446A0 46 bytes, 0x00A446CE, the catch funclet 0x00A446F4 -- landed as
//   a constant-return shape row --, 0x00A446FA, 0x00A44719); the true extent is 0x00A446A0..0x00A4473D; see SPLITS.tsv.
// Shape: the microthread TU is unoptimised (`#pragma optimize("", off)`, as its landed neighbours). Both loops are guarded by the
//   VC6-era for-scope idiom `if (0) {} else for (...)` (retail `xor ecx,ecx; je body; jmp exit` at 0x00A446C8 and 0x00A44720: a
//   plain `for (;;)` emits no test and `while (true)` emits `mov eax,1; test eax,eax`), spelled out here. The catch parameter is
//   named: the unoptimised frame keeps a second slot for it (`sub esp,8` at 0x00A446B9; unnamed gives one `push ecx`).
#include "fable_compiler.h"
#pragma optimize("", off)
#define FABLE_RETAIL_CASUSPENDABLEPROCESS_EXCEPTIONEXIT
#include "engine/CSuspendableProcessMicrothread.h"

namespace MicroThread {
    void FABLE_FASTCALL MicroThreadSleep();          // 0x009D8650
}

void CSuspendableProcessMicrothread::ThreadEntry(void* param)
{
    CSuspendableProcessMicrothread* process = (CSuspendableProcessMicrothread*)param;
    if (0) {} else for (; !process->StopRunning; ) {
        process->Running = true;
        try {
            process->SuspendableProcess();
        }
        catch (CASuspendableProcess::CExceptionExit& e) {
        }
        if (!process->StopRunning)
            process->SuspendSuspendableProcess();
    }
    process->Running = false;
    if (0) {} else for (;;)
        MicroThread::MicroThreadSleep();
}