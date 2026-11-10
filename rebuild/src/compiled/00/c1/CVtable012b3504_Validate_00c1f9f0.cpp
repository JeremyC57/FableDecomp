// CVtable_012b3504::Validate() -- validate the item's table member: GetTable() (slot 5) then the table's own Validate (slot 9)
// Leaf: ?Validate@CVtable_012b3504@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1F9F0, 12 bytes, thiscall: `mov eax, [ecx] ; call [eax+0x14] ; mov edx, [eax] ; mov ecx, eax ; jmp [edx+0x24]`;
//   split from the 47-byte gap-scan row 0x00C1F9F0 (three functions: slots 3, 6 and 7 of the class, int3 padding between).
// Identity (descriptive; libpk has no PDB or RTTI): slot 3 of CVtable_012b3504's vtable 0x012B3504 (cell 0x012B3510); the class name is the landed ctor CVtable_012b3504::CVtable_012b3504 0x00C1F960;
//   factory type code 0x20201 (CFactory::Create 0x00C1EFC0: key kind 1 = long-id, table member ctor 0x00C34DD0 at +8). The family, its
//   slot map and the dispatcher model are in libpk/CItem.h.
// Callee: CItemTable::Validate 0x00C348F0 (table slot 9) on the table member at +8 (ctor 0x00C34DD0).
// Shape: a plain virtual override calling two virtuals; VC7.1 tail-jumps the second (the chunk 899 Validate rows' shape).
#include "libpk/CItem.h"

void CVtable_012b3504::Validate()
{
    GetTable()->Validate();
}