// CVtable_012b3504::Detach(CItemDispatcher* dispatcher) -- remove the item from the dispatcher's own id-keyed list (+0): its Remove (list slot 8)
// Leaf: ?Detach@CVtable_012b3504@@UAEXPAUCItemDispatcher@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1FA10, 15 bytes, thiscall, `ret 4` in the callee: `mov eax, ecx ; mov ecx, [esp+4] ; mov edx, [ecx] ; mov [esp+4], eax ; jmp [edx+0x20]`;
//   a new function start exposed by splitting the gap-scan row 0x00C1F9F0.
// Identity (descriptive; libpk has no PDB or RTTI): slot 7 of CVtable_012b3504's vtable 0x012B3504 (cell 0x012B3520); the class name is the landed ctor CVtable_012b3504::CVtable_012b3504 0x00C1F960;
//   factory type code 0x20201 (CFactory::Create 0x00C1EFC0: key kind 1 = long-id, table member ctor 0x00C34DD0 at +8). The family, its
//   slot map and the dispatcher model are in libpk/CItem.h.
// Callee: PKCTRList<CItem>::Remove (list slot 8, +0x20) on the dispatcher's own id-keyed list (+0).
// Shape: `dispatcher->Remove(this)` on the dispatcher's primary list: VC7.1 moves `this` into the argument slot it no longer needs (`mov [esp+4],eax`) and tail-jumps the list's virtual.
#include "libpk/CItem.h"

void CVtable_012b3504::Detach(CItemDispatcher* dispatcher)
{
    dispatcher->Remove(this);
}