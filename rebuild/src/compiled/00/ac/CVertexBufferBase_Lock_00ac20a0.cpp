// CVertexBufferBase::Lock(unsigned long flags, unsigned long offset, unsigned long size) -- lock through the private virtual DoLock (slot 1), the arguments passed on unchanged
// Leaf: ?Lock@CVertexBufferBase@@QAEPAEKKK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC20A0, 5 bytes, thiscall: `mov eax,[ecx] ; jmp [eax+4]`.
// Identity: Ego_r.pdb + FableWin.pdb ?Lock@CVertexBufferBase@@QAEPAEKKK@Z (engine/CVertexBufferBase.h; DoLock is slot 1 of the
//   CVertexBufferWin32 vtable 0x0129BA20, 0x00A63240); the landed callers 0x00A91640 / 0x00B51570 / 0x00B6EDF0 / 0x00B8DCD0 / 0x00BB2540 /
//   0x00BCA950 / 0x00BCC800 / 0x00BEAE50 bind this address under that spelling.
// Port (NF149, q4559): was a generated tail-call shape on a local `struct T` (leaf ?Run@T@@QAEXXZ); now on engine/CVertexBufferBase.h.
// Parameter names as the landed CIndexBuffer::Lock / CVertexBufferWin32::DoLock rows (the DirectX Lock takes arg 2, arg 3, &data, arg 1).
#include "engine/CVertexBufferBase.h"

unsigned char* CVertexBufferBase::Lock(unsigned long flags, unsigned long offset, unsigned long size)
{
    return DoLock(flags, offset, size);
}
