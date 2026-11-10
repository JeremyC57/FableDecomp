// CHashedItem::Attach(CItemDispatcher* dispatcher) -- add the item to the dispatcher's name-hash index (+0x10): its Add (list slot 9)
// Leaf: ?Attach@CHashedItem@@UAEXPAUCItemDispatcher@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1F6E0, 19 bytes, thiscall, `ret 4` in the callee: `mov eax, [esp+4] ; mov edx, [eax+0x10] ; add eax, 0x10 ; mov [esp+4], ecx ; mov ecx, eax ; jmp [edx+0x24]`;
//   a new function start exposed by splitting the gap-scan row 0x00C1F6D0.
// Identity (descriptive; libpk has no PDB or RTTI): slot 6 of CHashedItem's vtable 0x012B3474 (cell 0x012B348C) (inherited by CNamedItem 0x012B3600, cell 0x012B3618); the class name is chunk 899 / landed ctor ??0CHashedItem@@QAE@K@Z 0x00C1F640;
//   factory type code 0x10102 (CFactory::Create 0x00C1EFC0: key kind 2 = name-hash, table member ctor 0x00C35170 at +8). The family, its
//   slot map and the dispatcher model are in libpk/CItem.h.
// Callee: PKCTRList<CItem>::Add (list slot 9, +0x24) on the dispatcher's name-hash index (+0x10).
// Shape: `dispatcher->m_names.Add(this)`: VC7.1 calls a member object's virtual through its vtable (no devirtualisation), reuses the dead argument slot for `this` and tail-jumps.
#include "libpk/CItem.h"

void CHashedItem::Attach(CItemDispatcher* dispatcher)
{
    dispatcher->m_names.Add(this);
}