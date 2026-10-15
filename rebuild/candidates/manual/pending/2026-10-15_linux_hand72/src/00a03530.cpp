// CScopedCritical::~CScopedCritical() -- leaves the owning pool's critical section when the lock was taken and the pool is multi-threaded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A03530, length 29 bytes.
// Leaf: ??1CScopedCritical
// Retail bytes: mov al,[ecx+4] ; test al,al ; je E ; mov eax,[ecx] ; mov cl,[eax+24h] ; test cl,cl ; je E ; add eax,44Ch ; push eax ;
//               call [LeaveCriticalSection] ; E: ret
// Callee: KERNEL32 LeaveCriticalSection (import 0x0143FE10).
// Layout (PDB): CScopedCritical = {CHandleBasedMemoryPool* Owner, bool Success}. CHandleBasedMemoryPool::MultiThreaded is at
// +0x24 as in engine/CHandleBasedMemoryPool.h, but its CriticalSection sits at +0x44c in retail (the header's +0x450 counts
// the PDB build's 16-byte CArray; retail CArray is 12 bytes), so the pool is modelled here with the retail offsets.
#include "engine/CHandleBasedMemoryPool.h"

struct RTL_CRITICAL_SECTION_Fable {            // Win32 RTL_CRITICAL_SECTION, 24 bytes
    unsigned char _body[0x18];
};
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(RTL_CRITICAL_SECTION_Fable* section);

struct CHandleBasedMemoryPoolRetail {
    unsigned char              _pad00[0x24];   // +0x00 vptr, buffers, sizes, counters, MemoryFlags
    bool                       MultiThreaded;  // +0x24
    unsigned char              _pad25[0x44c - 0x25];
    RTL_CRITICAL_SECTION_Fable CriticalSection;   // +0x44c
};
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, MultiThreaded) == offsetof(CHandleBasedMemoryPoolRetail, MultiThreaded));

struct CScopedCritical {
    CHandleBasedMemoryPoolRetail* Owner;       // +0x00
    bool                          Success;     // +0x04
    ~CScopedCritical();
};

CScopedCritical::~CScopedCritical()
{
    if (Success && Owner->MultiThreaded)
        LeaveCriticalSection(&Owner->CriticalSection);
}
