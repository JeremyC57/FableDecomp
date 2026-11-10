// CHashedItem::Validate() -- validate the item's table member: GetTable() (slot 5) then the table's own Validate (slot 9)
// Leaf: ?Validate@CHashedItem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1F6D0, 12 bytes, thiscall: `mov eax, [ecx] ; call [eax+0x14] ; mov edx, [eax] ; mov ecx, eax ; jmp [edx+0x24]`;
//   split from the 67-byte gap-scan row 0x00C1F6D0 (three functions: slots 3, 6 and 7 of the class, int3 padding between).
// Identity (descriptive; libpk has no PDB or RTTI): slot 3 of CHashedItem's vtable 0x012B3474 (cell 0x012B3480); the class name is chunk 899 / landed ctor ??0CHashedItem@@QAE@K@Z 0x00C1F640;
//   factory type code 0x10102 (CFactory::Create 0x00C1EFC0: key kind 2 = name-hash, table member ctor 0x00C35170 at +8). The family, its
//   slot map and the dispatcher model are in libpk/CItem.h.
// Callee: CItemTable::Validate 0x00C348F0 (table slot 9) on the table member at +8 (ctor 0x00C35170).
// Shape: a plain virtual override calling two virtuals; VC7.1 tail-jumps the second (the chunk 899 Validate rows' shape).
#include "libpk/CItem.h"

void CHashedItem::Validate()
{
    GetTable()->Validate();
}