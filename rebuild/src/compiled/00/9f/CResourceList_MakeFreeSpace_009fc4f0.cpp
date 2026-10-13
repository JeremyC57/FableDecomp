// CResourceList::MakeFreeSpace(unsigned long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC4F0, oracle length 33 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; push edi ; mov edi,[esp+0Ch] ; push edi ; mov esi,ecx ; call <free space worker> ; mov eax,[esi+24h] ; mov edx,[esi+28h] ; add eax,edi ; cmp edx,eax ; sbb al,al ; pop edi ; inc al ; pop esi ; ret 4
// engine/CResourceList.h (PDB): +0x24 AllocatedMemory, +0x28 MaximumMemory. Evicts resources
// through the worker at 0x009FC210 (same argument) and reports whether `bytes` now fit:
// AllocatedMemory + bytes <= MaximumMemory (sbb/inc materialises the unsigned <=).
//
// Lane feedback: bool result needs the `?:` form (byte-wide `sbb al,al ; inc al`). RELOCATION_MATCH.
#include "engine/CResourceList.h"

struct CResourceList_Methods : CResourceList {
    void EvictForSpace(unsigned long bytes);     // 0x009FC210 (name from use; manifest label untrusted)
    bool MakeFreeSpace(unsigned long bytes);
};

bool CResourceList_Methods::MakeFreeSpace(unsigned long bytes)
{
    EvictForSpace(bytes);
    // `sbb al,al ; inc al` (byte-wide) only comes from the `?:` form; `return a <= b;`
    // computes the flag in eax (`sbb eax,eax ; inc eax`) and differs by one byte.
    return (this->AllocatedMemory + bytes <= this->MaximumMemory) ? true : false;
}