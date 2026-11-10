// CVtable_012b4f60::GetSize(unsigned long* size) -- the size of the wrapped file: forwards to the file object's GetSize (+4, slot 0)
// Leaf: ?GetSize@CVtable_012b4f60@@UAE_NPAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C33070, 7 bytes, thiscall (`ret 4` in the callee), a tail call: `mov ecx, [ecx+4] ; mov eax, [ecx] ; jmp [eax]`;
//   split from the 40-byte gap-scan row 0x00C33070 (three forwarders 16 bytes apart, int3 padding between).
// Identity (descriptive; libpk has no PDB or RTTI): slot 0 of the file handle vtable 0x012B4F60 (cell 0x012B4F60), stored only
//   by the landed ctor 0x00C330A0; the slot names are the sibling default-file handle CVtable_012b4f4c's (0x00C32F40 GetSize,
//   0x00C32F60 Read, 0x00C32F30 GetUnitSize). Class model and evidence: libpk/CVtable_012b4f60.h.
// Shape: a forwarder whose arguments sit where the callee wants them; VC7.1 tail-jumps the wrapped object's virtual.
#include "libpk/CVtable_012b4f60.h"

bool CVtable_012b4f60::GetSize(unsigned long* size)
{
    return m_pFile->GetSize(size);
}