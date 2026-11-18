// MicroThread::MicroThreadSleep() -- yield to the other side: push ebx/ebp/esi/edi and the SEH head fs:[0] on the current stack, swap esp with MicroThread::SavedStackPointer, pop the other stack's SEH head and registers, return on the other stack
// Grade: DIFFER(12v36) -- not C++-expressible (retail_asm candidate, OWNER DECISION): the body writes esp (0x009D865D `mov esp,[0x013CA898]`, 0x009D8663 `mov [0x013CA898],eax`); a naked asm helper macro reproduces all 36 bytes (RELOCATION_MATCH, probe/f1.cpp + probe/asmhdr/engine/MicroThreadStackSwitch.h) but has no semantically identical FABLE_PORTABLE twin and needs a naked definition in a shared header (no precedent). This file is the plain-C++ statement of the effect only.
// Leaf: ?MicroThreadSleep@MicroThread@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D8650, 36 bytes, __fastcall, no arguments, frameless (the pushes ARE the saved image).
// Identity (proven, three sources): Ego_d.pdb lib_microthread.obj order DeleteMicroThread / MicroThreadSleep / MicroThreadReturn /
//   SwitchToMicroThread (0x009D87F0); FableWin.pdb ?MicroThreadSleep@MicroThread@@YIXXZ 0x02F6CA00 then ?MicroThreadSleep@MicroThread@@YIXH@Z
//   0x02F6CA30 then ?MicroThreadReturn@MicroThread@@YIXXZ 0x02F6CA60 (Ego_r the same, 0x00B2B980 / 0x00B2B9B0 / 0x00B2B9C0) = retail
//   0x009D8650 / 0x009D8680 / 0x009D8690; the landed SwitchToMicroThread 0x009D87F0 calls it. Ego_d /Od length 36 = retail 36: naked
//   in the original source too. Global: reloc_bindings ?SavedStackPointer@MicroThread@@3PAEA (0x013CA898; descriptive landed name).
namespace MicroThread
{
    extern unsigned char* SavedStackPointer;                    // 0x013CA898
    extern "C" void __cdecl SwapStackPointer(unsigned char** saved);   // stands for the retail stack switch (no C++ equivalent)

    void __fastcall MicroThreadSleep()
    {
        SwapStackPointer(&SavedStackPointer);
    }
}
