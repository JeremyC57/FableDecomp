// CResourceList::MakeFreeSpace(unsigned long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC4F0, oracle length 33 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; push edi ; mov edi,[esp+0Ch] ; push edi ; mov esi,ecx ; call <free space worker> ; mov eax,[esi+24h] ; mov edx,[esi+28h] ; add eax,edi ; cmp edx,eax ; sbb al,al ; pop edi ; inc al ; pop esi ; ret 4
// engine/CResourceList.h (PDB): +0x24 AllocatedMemory, +0x28 MaximumMemory. Evicts resources
// through the worker at 0x009FC210 (same argument) and reports whether `bytes` now fit:
// AllocatedMemory + bytes <= MaximumMemory (sbb/inc materialises the unsigned <=).
#include "engine/CResourceList.h"

struct CResourceList_Methods : CResourceList {
    void EvictForSpace(unsigned long bytes);     // 0x009FC210
    bool MakeFreeSpace(unsigned long bytes);
};

bool CResourceList_Methods::MakeFreeSpace(unsigned long bytes)
{
    EvictForSpace(bytes);
    return this->AllocatedMemory + bytes <= this->MaximumMemory;
}
