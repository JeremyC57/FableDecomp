// CPersistContext::Transfer(CRandomAppearanceMorph& value) -- persist a random-appearance morph by binary stream only: CRC-check the empty key, build the default value, then load or save the morph by the context's binary mode
// Leaf: ??$Transfer@
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00676740, 256 bytes (247 without padding), thiscall `ret 4`.
// Callers: CCreatureDef::Transfer 0x00675E20 (PRandomAppearanceMorph's object, `call` at 0x00675E61) and the two-argument
//   forwarder 0x006768B0 (`push [esp+8] ; call ; ret 8`).
// Callees: CPersistContext::CheckCRC(const char*) 0x00404500 with the pooled empty string 0x0122D70E, CRandomAppearanceMorph
//   ctor 0x0086C4D0 (the default value: GetDefaultVal<T>() returned in place), TransferBinaryIn(CMemoryDataInputStream&)
//   0x0086BFB0, TransferBinaryOut(CDataOutputStream&) const 0x0086B6D0, the default's inline destructor: free 0x00BFEA14
//   (GeneralTextureMorphs, SkeletalMorphs), vector<CVectorMap<unsigned long, CTextureMorph> >::~vector 0x006753B0 (out of
//   line in this TU), __destroy_aux(CArray<CBodyPartMesh>*, .., const __false_type&) 0x00675850 (`__fastcall`; the tag
//   homed in the dead `value` argument slot).
// Label: catalog "CRandomAppearanceMorph_Serialize" is descriptive; Ego_r.pdb names CPersistContext::Transfer<
//   CRandomAppearanceMorph>(const char*, T&[, const T&]) and GetDefaultVal<CRandomAppearanceMorph>(); this one-argument
//   body has no name parameter, so the name here is descriptive.
// Layout: CPersistContext (Ego_r.pdb) Mode +0x18, PLoadStream +0x24, PSaveStream +0x28; CRandomAppearanceMorph as the
//   landed operator= 0x00675B60 (retail 12-byte CArray, 16-byte CVectorMap): BodyParts +0, BodyPartTextureMorphs +0xc,
//   SkeletalMorphs +0x18, GeneralTextureMorphs +0x28, MaxTextureGroupID +0x38.
// Shape: the binary modes are a two-case switch (`sub eax,2 ; je ; dec eax ; je`) whose cases return, so VC7.1 emits the
//   default value's destructor at each of the three exits (cross-jumped from the __destroy_aux call on); the save case
//   comes first in the source (retail lays out default, case 3, case 2). The default value's destructor is inlined at each
//   exit: the lane VC7.1 keeps the implicit (or plain inline) ~CRandomAppearanceMorph out of line (a `call` per exit), so
//   it is declared and then defined `__forceinline` after the explicit specializations of the two out-of-line callees.
#include "fable_compiler.h"
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14

class CDataOutputStream;
class CMemoryDataInputStream;

#include "engine/CArray.h"


#include "engine/CVectorMap.h"

class CRandomAppearanceMorph {                         // retail 0x3c bytes
public:
    class CBodyPartMesh;
    class CTextureMorph {
    public:
        unsigned char _body[0x10];
    };
    class CSkeletalMorph {
    public:
        unsigned char _body[0x20];
    };
    typedef CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<unsigned long, CTextureMorph> > CTextureMorphMap;
    CArray<CArray<CBodyPartMesh> >                                                     BodyParts;              // +0x00
    CArray<CTextureMorphMap>                                                           BodyPartTextureMorphs;  // +0x0c
    CVectorMap<long, CSkeletalMorph, CKeyPairCompareLess<long, CSkeletalMorph> >       SkeletalMorphs;         // +0x18
    CTextureMorphMap                                                                   GeneralTextureMorphs;   // +0x28
    unsigned long                                                                      MaxTextureGroupID;      // +0x38
    CRandomAppearanceMorph();                                                          // 0x0086C4D0
    ~CRandomAppearanceMorph();
    void TransferBinaryIn(CMemoryDataInputStream& stream);                             // 0x0086BFB0
    void TransferBinaryOut(CDataOutputStream& stream) const;                           // 0x0086B6D0
};

namespace std {
template <>
void __fastcall __destroy_aux<CArray<CRandomAppearanceMorph::CBodyPartMesh>*>(CArray<CRandomAppearanceMorph::CBodyPartMesh>* __first,
    CArray<CRandomAppearanceMorph::CBodyPartMesh>* __last, const __false_type&);   // 0x00675850
template <> vector<CRandomAppearanceMorph::CTextureMorphMap, allocator<CRandomAppearanceMorph::CTextureMorphMap> >::~vector();   // 0x006753B0
}

FABLE_FORCEINLINE CRandomAppearanceMorph::~CRandomAppearanceMorph() { }

class CPersistContext {
public:
    unsigned char           _pad00[0x18];
    long                    Mode;                      // +0x18: 2 binary load, 3 binary save
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;               // +0x24
    CDataOutputStream*      PSaveStream;               // +0x28
    protected: void CheckCRC(const char* name); public:                   // 0x00404500
    template <class T> static T GetDefaultVal() { return T(); }
    template <class T> void Transfer(T& value);
};

template <class T>
void CPersistContext::Transfer(T& value)
{
    CheckCRC("");
    T defaultValue = GetDefaultVal<T>();
    switch (Mode) {
    case 3:
        value.TransferBinaryOut(*PSaveStream);
        return;
    case 2:
        value.TransferBinaryIn(*PLoadStream);
        return;
    }
}

template void CPersistContext::Transfer<CRandomAppearanceMorph>(CRandomAppearanceMorph& value);