// CVtable_012b34bc::Validate() -- validate the item's table member: GetTable() (slot 5) then the table's own Validate (slot 9)
// Leaf: ?Validate@CVtable_012b34bc@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1F870, 12 bytes, thiscall: `mov eax, [ecx] ; call [eax+0x14] ; mov edx, [eax] ; mov ecx, eax ; jmp [edx+0x24]`;
//   split from the 47-byte gap-scan row 0x00C1F870 (three functions: slots 3, 6 and 7 of the class, int3 padding between).
// Identity (descriptive; libpk has no PDB or RTTI): slot 3 of CVtable_012b34bc's vtable 0x012B34BC (cell 0x012B34C8); the class name is the landed ctor CVtable_012b34bc::CVtable_012b34bc 0x00C1F7E0;
//   factory type code 0x20101 (CFactory::Create 0x00C1EFC0: key kind 1 = long-id, table member ctor 0x00C34AA0 at +8). The family, its
//   slot map and the dispatcher model are in libpk/CItem.h.
// Callee: CItemTable::Validate 0x00C348F0 (table slot 9) on the table member at +8 (ctor 0x00C34AA0).
// Shape: a plain virtual override calling two virtuals; VC7.1 tail-jumps the second (the chunk 899 Validate rows' shape).
#include "libpk/CItem.h"

void CVtable_012b34bc::Validate()
{
    GetTable()->Validate();
}