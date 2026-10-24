// MicroThread::MicroThreadSleep() -- yield: save the callee-saved registers and the SEH chain head on the current stack, swap esp with the saved micro-thread/main stack pointer, restore the other side's chain head and registers, return on the other stack
// Grade: DIFFER(12v36) -- retail_asm candidate: the whole body is a stack switch (`push ebx/ebp/esi/edi ; push fs:[0] ; mov eax,esp ; mov esp,[g_savedStack] ; mov [g_savedStack],eax ; pop fs:[0] ; pop edi/esi/ebp/ebx ; ret`): it writes esp, which no C++ can express; this reference body only names the effect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D8650, length 36 bytes.
// PDB (FableWin): ?MicroThreadSleep@MicroThread@@YIXXZ -- __fastcall, no arguments (an overload
// ?MicroThreadSleep@MicroThread@@YIXH@Z, 0x009D8680, calls it ecx times).
// Global: the parked stack pointer 0x013CA898, shared with SwitchToMicroThread (0x009D87F0), which points it at the
// thread's restored stack image before calling this function. CreateMicroThread (0x009D8710) builds the initial
// image this function pops: chain head &DummyExceptionBlock (0x0139650C), four zero registers, the entry point as
// the return address, MicroThreadReturn (0x009D8690) as the entry's return address, then the entry's argument.
// Retail-asm: the push/pop frame belongs to the asm (it is the saved-register image of the other stack), so the
// function is effectively frameless; it cannot land as plain C++.
namespace MicroThread
{
    extern void* g_pSavedStack;                     // 0x013CA898
    extern "C" void __cdecl SwapStackPointer(void** saved);   // stands for the retail inline stack switch

    void __fastcall MicroThreadSleep()
    {
        SwapStackPointer(&g_pSavedStack);
    }
}
