// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CWoundMorphs>) the counted pointer's per-type deleter: destroy the wound morph table (free its vector storage) and free the object
// Leaf: ?DeleteData@?$CCountedPointer@VCWoundMorphs@@@@SIXPAX@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00674E50, 33 bytes (+ int3 padding), `ret` (static `__fastcall`, data in ecx).
// Identity (proven): CCountedPointer<CWoundMorphs>::DeleteData(void*); evidence: its only reference is the count-block store in CCountedPointer<CWoundMorphs>::Reset 0x00675060 (0x006750B0), called by CCreatureDef::CCreatureDef 0x006765A0 at 0x00676687 on this+0x28 = Ego_r.pdb CCreatureDef::PWoundMorphs (CCountedPointer<CWoundMorphs>) with a fresh 0x10-byte object (zeroed vector, Dirty +0x0D cleared = CWoundMorphs::Morphs, a CVectorMap); Ego_r.pdb names ?DeleteData@?$CCountedPointer@VCWoundMorphs@@@@SIXPAX@Z.
// Label: the manifest's "Object_FreeInnerAndDelete_00674e50" is behavioural (family J103: five byte-identical deleters; retail has no identical-code
//   folding, so each address is its own CCountedPointer<T>).
// Callees: free 0x00BFEA14 (the STLport malloc allocator's deallocate of the vector block), operator delete 0x00BFE9BC.
// Shape: the landed CCountedPointer<CSpeechFrame>::DeleteData 0x00A4A910: `delete (T*)data` -- the null test, the inline
//   (forced) destructor chain of a POD-element vector (`_Destroy` of trivially destructible elements is empty; `_Vector_base`'s
//   destructor frees a non-null start), then operator delete.
extern "C" void __cdecl free(void*);                       // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

#include "engine/CVectorMap.h"
enum EBodyLocation { };                                    // Ego_r.pdb (enumerators not needed)
class CWoundMorphs {                                       // retail 0x10 bytes (Ego_r.pdb 0x14 with a Dinkumware vector)
public:
    class CEntry {                                         // 40 bytes (Ego_r.pdb), trivially destructible
    public:
        EBodyLocation Location;                            // +0x00
        long ReplacingTextureIndex;                        // +0x04
        long ReplacingBumpMapIndex;                        // +0x08
        long WoundTextureIndex;                            // +0x0C
        long WoundBumpMapIndex;                            // +0x10
        long ScarTextureIndex;                             // +0x14
        long ScarBumpMapIndex;                             // +0x18
        long FadeToScarDuration;                           // +0x1C
        long TotalDuration;                                // +0x20
        float PermanentScarStrength;                       // +0x24
    };
    CVectorMap<EBodyLocation, CEntry, CKeyPairCompareLess<EBodyLocation, CEntry> > Morphs;   // +0x00
};

#include "engine/CCountedPointer.h"

template class CCountedPointer<CWoundMorphs>;