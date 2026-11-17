// CCountedPointer<CVertexBufferWin32>::DeleteData(void* data) -- the per-type deleter stored in the count block: delete the vertex buffer
//   through its virtual destructor (`test ecx,ecx ; je ; mov eax,[ecx] ; push 1 ; call [eax]`).
// Leaf: ?DeleteData@?$CCountedPointer@VCVertexBufferWin32@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8E690, 11 bytes.
// Identity (NF56, round 7, queue q10491): FableWin.pdb ?DeleteData@?$CCountedPointer@VCVertexBufferWin32@@@@SIXPAX@Z (the per-T deleter of
//   engine/CCountedPointer.h); its only reference is the count-block store of the landed CCountedPointer<CVertexBufferWin32>::Reset 0x00B8EC10
//   (absolute reference 0x00B8EC60); CVertexBufferWin32 is polymorphic (RTTI : CVertexBufferBase, slot 0 = the scalar deleting destructor).
//   Was the gapscan stand-in extern "C" DeleteIfSet over a local Obj. The landed DeleteData rows explicitly instantiate the template member.
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#include "engine/CVertexBufferWin32.h"
#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CVertexBufferWin32>::DeleteData(void*);
