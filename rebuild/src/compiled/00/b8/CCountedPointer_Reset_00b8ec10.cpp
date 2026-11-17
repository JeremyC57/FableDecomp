// CCountedPointer<CVertexBufferWin32>::Reset(CVertexBufferWin32*) -- drop the shared count block (destroying the object on last reference), then adopt the new object
// Leaf: ?Reset@?$CCountedPointer@VCVertexBufferWin32@@@@QAEXPAVCVertexBufferWin32@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8EC10, oracle length 103 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+4] ; test eax,eax ; je A ; dec dword ptr [eax] ; mov eax,[esi+4] ; cmp dword ptr [eax],0 ; jne A ; mov ecx,[eax+8] ; call [eax+4] ; mov eax,[esi+4] ; push eax ; call operator delete ; add esp,4 ; A: mov eax,[esp+8] ; test eax,eax ; mov dword ptr [esi+4],0 ; mov [esi],eax ; je Z ; push 0Ch ; call operator new ; add esp,4 ; test eax,eax ; je N ; mov ecx,[esi] ; mov dword ptr [eax],1 ; mov dword ptr [eax+4],offset Destroy ; mov [eax+8],ecx ; mov [esi+4],eax ; pop esi ; ret 4 ; N: xor eax,eax ; mov [esi+4],eax ; Z: pop esi ; ret 4
// Intrusive-count smart pointer {object, count block}: the 12-byte count block holds the
// reference count, the per-type destroy function (0x00B8E690, called with the object in ecx)
// and the object. Reset() decrements the old block (re-reading it after the decrement: the count
// is touched through a reference), destroys + frees it on zero, then adopts the new object by
// allocating a fresh block (operator new @ 0x00BFEA1A, null-checked; delete @ 0x00BFE9BC). The
// instantiation is named from the manifest label when unique, else from the destroy function.
// One of 74 bodies of this shape.
// Identity (NF53): FableWin.pdb ?Reset@?$CCountedPointer@VCVertexBufferWin32@@@@QAEXPAVCVertexBufferWin32@@@Z and its deleter
//   ?DeleteData@?$CCountedPointer@VCVertexBufferWin32@@@@SIXPAX@Z 0x00B8E690 (`if (p) p->~T()` through slot 0: CVertexBufferWin32 is
//   polymorphic), the vertex-buffer TU next to the map<ulong, CCountedPointer<CVertexBufferWin32> > helpers 0x00B8DA80 / 0x00B8F470.
//   The Reset family shape of engine/CCountedPointer.h (0x008AB2B0 and 197 more), explicitly instantiated.
#include "engine/CCountedPointer.h"
class CVertexBufferWin32;
template void CCountedPointer<CVertexBufferWin32>::Reset(CVertexBufferWin32*);
