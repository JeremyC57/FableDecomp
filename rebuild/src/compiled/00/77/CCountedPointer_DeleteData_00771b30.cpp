// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<COpinionSet>) the counted pointer's per-type deleter: destroy the opinion set (free its vector storage) and free the object
// Leaf: ?DeleteData@?$CCountedPointer@VCOpinionSet@@@@SIXPAX@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771B30, 33 bytes (+ int3 padding), `ret` (static `__fastcall`, data in ecx).
// Identity (proven): CCountedPointer<COpinionSet>::DeleteData(void*); evidence: its only reference is the count-block store in CCountedPointer<COpinionSet>::Reset 0x00771B60 (0x00771BB0), called by CTCOpinionSource::OnCreate 0x00771660 on this+0x78 = Ego_r.pdb CTCOpinionSource::POpinionProduced (CCountedPointer<COpinionSet>, Ego_r +0x7C) with a fresh object whose vector holds five zeroed floats = COpinionSet::OpinionValues (CArray<float>); Ego_r.pdb names ?DeleteData@?$CCountedPointer@VCOpinionSet@@@@SIXPAX@Z.
// Label: the manifest's "ObjHolder_FreeAndDelete_00771b30" is behavioural (family J103: five byte-identical deleters; retail has no identical-code
//   folding, so each address is its own CCountedPointer<T>).
// Callees: free 0x00BFEA14 (the STLport malloc allocator's deallocate of the vector block), operator delete 0x00BFE9BC.
// Shape: the landed CCountedPointer<CSpeechFrame>::DeleteData 0x00A4A910: `delete (T*)data` -- the null test, the inline
//   (forced) destructor chain of a POD-element vector (`_Destroy` of trivially destructible elements is empty; `_Vector_base`'s
//   destructor frees a non-null start), then operator delete.
extern "C" void __cdecl free(void*);                       // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

#include "engine/CArray.h"
class COpinionSet {                                        // retail 0x0C bytes (Ego_r.pdb 0x10 with a Dinkumware vector)
public:
    CArray<float> OpinionValues;                           // +0x00
};

class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<COpinionSet>;