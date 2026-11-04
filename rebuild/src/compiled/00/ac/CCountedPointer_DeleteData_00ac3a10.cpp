// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CArray<C3DTriangle2> >) the counted pointer's per-type deleter: destroy the triangle array (free its vector storage) and free the object
// Leaf: ?DeleteData@?$CCountedPointer@V?$CArray@VC3DTriangle2@@@@@@SIXPAX@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC3A10, 33 bytes (+ int3 padding), `ret` (static `__fastcall`, data in ecx).
// Identity (proven): CCountedPointer<CArray<C3DTriangle2>>::DeleteData(void*); evidence: its only reference is the count-block store in CCountedPointer<CArray<C3DTriangle2> >::Reset 0x00AC3A40 (0x00AC3A90), called by C3DMeshPartitionBuilderElement::Build 0x00AC36D0 (both PDB name sets: ?Build@C3DMeshPartitionBuilderElement@@QAEX... PAV?$CCountedPointer@V?$CArray@VC3DTriangle2@@@@@@@Z) on its out pointer with the fresh 12-byte CArray<C3DTriangle2> it filled through BuildOutput 0x00AC2A80; both PDB name sets name ?DeleteData@?$CCountedPointer@V?$CArray@VC3DTriangle2@@@@@@SIXPAX@Z.
// Label: the manifest's "Object_FreeBufferAndDelete" is behavioural (family J103: five byte-identical deleters; retail has no identical-code
//   folding, so each address is its own CCountedPointer<T>).
// Callees: free 0x00BFEA14 (the STLport malloc allocator's deallocate of the vector block), operator delete 0x00BFE9BC.
// Shape: the landed CCountedPointer<CSpeechFrame>::DeleteData 0x00A4A910: `delete (T*)data` -- the null test, the inline
//   (forced) destructor chain of a POD-element vector (`_Destroy` of trivially destructible elements is empty; `_Vector_base`'s
//   destructor frees a non-null start), then operator delete.
extern "C" void __cdecl free(void*);                       // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

#include "engine/CArray.h"
class C3DTriangle2 {                                      // 6 bytes (Ego_r.pdb)
public:
    unsigned short VertexIndex[3];                         // +0x00
};

class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<CArray<C3DTriangle2> >;