// MicroThread::SetStack(unsigned char*, unsigned long) -- install the shared micro-thread stack: plant a 0xCDCDCDCD guard word below its top and record top, base and size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D86B0, length 27 bytes.
// PDB (FableWin): ?SetStack@MicroThread@@YIXPAEK@Z -- a namespace-scope __fastcall (stack in ecx, size in edx).
// Globals (MicroThread module statics): the working stack top 0x013CA8A0 (the guard word's address, the point
// SwitchToMicroThread copies a thread's saved stack image back under), the caller-supplied end 0x013CA8A4 and the
// size 0x013CA89C. Shape: the guard is written through the computed pointer BEFORE the globals are stored (a store
// through a pointer may alias the globals, so VC7.1 keeps the source order).
namespace MicroThread
{
    extern unsigned char* g_pStackTop;       // 0x013CA8A0
    extern unsigned char* g_pStackEnd;       // 0x013CA8A4
    extern unsigned long  g_stackSize;       // 0x013CA89C

    void __fastcall SetStack(unsigned char* stack, unsigned long size)
    {
        unsigned char* top = stack - 4;
        *(unsigned long*)top = 0xCDCDCDCD;
        g_pStackTop = top;
        g_pStackEnd = stack;
        g_stackSize = size;
    }
}