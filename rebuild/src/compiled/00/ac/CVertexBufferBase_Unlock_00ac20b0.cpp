// CVertexBufferBase::Unlock() -- unlock through the private virtual DoUnlock (slot 2)
// Leaf: ?Unlock@CVertexBufferBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC20B0, 5 bytes, thiscall: `mov eax,[ecx] ; jmp [eax+8]`.
// Identity: Ego_r.pdb + FableWin.pdb ?Unlock@CVertexBufferBase@@QAEXXZ (engine/CVertexBufferBase.h; DoUnlock is slot 2 of the
//   CVertexBufferWin32 vtable 0x0129BA20, 0x00A63120); the landed callers 0x00A91640 / 0x00B51570 / 0x00B6EDF0 / 0x00B8DCD0 / 0x00BB2540 /
//   0x00BCA950 / 0x00BCC800 / 0x00BEAE50 bind this address under that spelling.
// Port (NF149, q4560): was a generated tail-call shape on a local `struct T` (leaf ?Run@T@@QAEXXZ); now on engine/CVertexBufferBase.h.
#include "engine/CVertexBufferBase.h"

void CVertexBufferBase::Unlock()
{
    DoUnlock();
}
