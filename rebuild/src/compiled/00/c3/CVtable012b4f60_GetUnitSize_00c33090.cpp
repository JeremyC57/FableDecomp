// CVtable_012b4f60::GetUnitSize() -- the wrapped file's read unit: forwards to the file object's GetUnitSize (+4, slot 2)
// Leaf: ?GetUnitSize@CVtable_012b4f60@@UAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C33090, 8 bytes, thiscall (no arguments), a tail call: `mov ecx, [ecx+4] ; mov eax, [ecx] ; jmp [eax+8]`;
//   a new function start exposed by splitting the gap-scan row 0x00C33070.
// Identity (descriptive; libpk has no PDB or RTTI): slot 2 of the file handle vtable 0x012B4F60 (cell 0x012B4F68), stored only
//   by the landed ctor 0x00C330A0; the slot names are the sibling default-file handle CVtable_012b4f4c's (0x00C32F40 GetSize,
//   0x00C32F60 Read, 0x00C32F30 GetUnitSize). Class model and evidence: libpk/CVtable_012b4f60.h.
// Shape: a forwarder whose arguments sit where the callee wants them; VC7.1 tail-jumps the wrapped object's virtual.
#include "libpk/CVtable_012b4f60.h"

unsigned long CVtable_012b4f60::GetUnitSize()
{
    return m_pFile->GetUnitSize();
}