// CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc::CSeaBodyDesc() -- the record's default constructor: an empty name, then the empty array
// Leaf: ??0CSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@QAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00515620, 23 bytes, thiscall, plain `ret`.
// Identity (probable): unreferenced (inlined at its uses); Ego_r.pdb/FableWin.pdb ??0CSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@QAE@XZ is the only {CCharString, CArray<...>} record with a default constructor symbol in the CWorldMap static-map code this sits in (0x00515620, among CWorldMap's STLport instantiations; the manifest's propagated labels name CSeaBodyDesc in this TU); the other such records (CStringParserComment, C3DSkeletalMorphTarget 0x00A44E60) live in the engine.
// Callees: CCharString::CCharString() 0x0099E4B0.
// Shape (family M109, representative 0x00515620): CCharString's default constructor out of line, then the CArray (an STLport
//   vector, the landed `CArray<T> : public std::vector<T>` model) zeroes its three pointers at +0x4 (one `xor eax,eax`).
class CCharString {
public:
    CCharString();                                        // 0x0099E4B0
    void* PStringData;
};
#include "engine/CArray.h"
class CStaticMapGenerationVisibiilityInfo { public:
    struct CSeaBodyDesc {                                 // Ego_r.pdb, 20 bytes (Dinkumware CArray)
        CSeaBodyDesc() { }
        CCharString Identifier;                           // +0x0
        CArray<long> Maps;                                // +0x4
    };
};
// the implicit-in-retail constructor, forced out of line for this object (not part of the retail body)
CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc* ConstructOne(void* memory);
CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc* ConstructOne(void* memory) { return new (memory) CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc; }