// CSuspendableProcessMicrothread::InternalSuspend() -- yields the running microthread (unoptimised TU)
// Retail: Fable.exe 0x00A44690, 10 bytes: `push ebp ; mov ebp,esp ; call MicroThread::MicroThreadSleep ; pop ebp ; ret`.
// Identity: the catalog label (_Cons_val<... CParticleMorphs ...>) is a propagated wrong name. Its callers sit in
// 0x00A44840, CSuspendableProcessMicrothread::SuspendSuspendableProcess (bool; termination check, suspend, check
// again), and the function sits between InternalSwitchToProcess 0x00A44660 and ThreadEntry 0x00A446A0. Ego_r.pdb:
// ?InternalSuspend@CSuspendableProcessMicrothread@@KIXXZ (protected static __fastcall, no arguments). The callee
// 0x009D8650 is the no-argument overload ?MicroThreadSleep@MicroThread@@YIXXZ (nothing is passed).
// The microthread TU is compiled without optimisation (ebp frames, `movzx`/`test` bool tests): #pragma optimize("", off).
// Leaf: ?InternalSuspend@CSuspendableProcessMicrothread@@KIXXZ
#pragma optimize("", off)

namespace MicroThread {
    void __fastcall MicroThreadSleep();          // 0x009D8650
}

class CSuspendableProcessMicrothread {
protected:
    static void __fastcall InternalSuspend();
};

void __fastcall CSuspendableProcessMicrothread::InternalSuspend()
{
    MicroThread::MicroThreadSleep();
}