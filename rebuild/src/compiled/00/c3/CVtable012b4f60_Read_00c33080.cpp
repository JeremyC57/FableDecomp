// CVtable_012b4f60::Read(unsigned long count, long unused, void* a, void* b, void* c, void* d) -- queue a read on the wrapped file: forwards all six arguments to the file object's Read (+4, slot 1)
// Leaf: ?Read@CVtable_012b4f60@@UAE_NKJPAX000@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C33080, 8 bytes, thiscall (`ret 18h` in the callee), a tail call: `mov ecx, [ecx+4] ; mov eax, [ecx] ; jmp [eax+4]`;
//   a new function start exposed by splitting the gap-scan row 0x00C33070.
// Identity (descriptive; libpk has no PDB or RTTI): slot 1 of the file handle vtable 0x012B4F60 (cell 0x012B4F64), stored only
//   by the landed ctor 0x00C330A0; the slot names are the sibling default-file handle CVtable_012b4f4c's (0x00C32F40 GetSize,
//   0x00C32F60 Read, 0x00C32F30 GetUnitSize). Class model and evidence: libpk/CVtable_012b4f60.h.
// Shape: a forwarder whose arguments sit where the callee wants them; VC7.1 tail-jumps the wrapped object's virtual.
#include "libpk/CVtable_012b4f60.h"

bool CVtable_012b4f60::Read(unsigned long count, long unused, void* a, void* b, void* c, void* d)
{
    return m_pFile->Read(count, unused, a, b, c, d);
}