// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<std::vector<unsigned __int64> >) the counted pointer's per-type deleter: destroy the vector of 64-bit UIDs (free its vector storage) and free the object
// Leaf: ?DeleteData@?$CCountedPointer@V?$vector@_KV?$allocator@_K@std@@@std@@@@SIXPAX@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006174F0, 33 bytes (+ int3 padding), `ret` (static `__fastcall`, data in ecx).
// Identity (proven): CCountedPointer<std::vector<unsigned __int64>>::DeleteData(void*); evidence: its only reference is the count-block store in CCountedPointer<vector<unsigned __int64> >::Reset 0x00618660 (0x006186B0), called by CTCInventoryMap code 0x00606420 at 0x006068A7 with a fresh zeroed 12-byte vector for this+0x200 = Ego_r.pdb CTCInventoryMap::SerialisedTeleporterUIDs, CCountedPointer<vector<unsigned __int64> > (Ego_r +0x254); Ego_r.pdb names ?DeleteData@?$CCountedPointer@V?$vector@_KV?$allocator@_K@std@@@std@@@@SIXPAX@Z.
// Label: the manifest's "DeleteAndFreeData" is behavioural (family J103: five byte-identical deleters; retail has no identical-code
//   folding, so each address is its own CCountedPointer<T>).
// Callees: free 0x00BFEA14 (the STLport malloc allocator's deallocate of the vector block), operator delete 0x00BFE9BC.
// Shape: the landed CCountedPointer<CSpeechFrame>::DeleteData 0x00A4A910: `delete (T*)data` -- the null test, the inline
//   (forced) destructor chain of a POD-element vector (`_Destroy` of trivially destructible elements is empty; `_Vector_base`'s
//   destructor frees a non-null start), then operator delete.
extern "C" void __cdecl free(void*);                       // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

#include "engine/CArray.h"


class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<std::vector<unsigned __int64> >;