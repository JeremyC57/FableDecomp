// CKeyedItem::Attach(CItemDispatcher* dispatcher) -- add the item to the dispatcher's own id-keyed list (+0): its Add (list slot 9)
// Leaf: ?Attach@CKeyedItem@@UAEXPAUCItemDispatcher@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1F7C0, 15 bytes, thiscall, `ret 4` in the callee: `mov eax, ecx ; mov ecx, [esp+4] ; mov edx, [ecx] ; mov [esp+4], eax ; jmp [edx+0x24]`;
//   a new function start exposed by splitting the gap-scan row 0x00C1F7B0.
// Identity (descriptive; libpk has no PDB or RTTI): slot 6 of CKeyedItem's vtable 0x012B3498 (cell 0x012B34B0); the class name is the landed caller binding ??0CKeyedItem@@QAE@J@Z (CItemAction 0x00C1EE60) of ctor 0x00C1F720;
//   factory type code 0x10101 (CFactory::Create 0x00C1EFC0: key kind 1 = long-id, table member ctor 0x00C35170 at +8). The family, its
//   slot map and the dispatcher model are in libpk/CItem.h.
// Callee: PKCTRList<CItem>::Add (list slot 9, +0x24) on the dispatcher's own id-keyed list (+0).
// Shape: `dispatcher->Add(this)` on the dispatcher's primary list: VC7.1 moves `this` into the argument slot it no longer needs (`mov [esp+4],eax`) and tail-jumps the list's virtual.
#include "libpk/CItem.h"

void CKeyedItem::Attach(CItemDispatcher* dispatcher)
{
    dispatcher->Add(this);
}