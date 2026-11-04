// CConfigOptionsDefaultsDef::CConfigOptionsDefaultsDef() -- default constructor of a CParentDefClassBase-derived def: CResource base out of line, then the inlined def bases (PDefManager 0, the three flag bits cleared, InstantiationName -1, an empty SubDefInfoMap), then this class's vtable
// Leaf: ??0CConfigOptionsDefaultsDef@@QAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00434240, 51 bytes (+ int3 padding to 64), thiscall, plain `ret`, returns this.
// Identity (proven): CConfigOptionsDefaultsDef::CConfigOptionsDefaultsDef() (FableWin.pdb name set: ??0CConfigOptionsDefaultsDef@@QAE@XZ); evidence: RTTI of the stored vtable 0x0123125C is .?AVCConfigOptionsDefaultsDef@@ (same def base chain); its size slot 20 (0x00434280) returns 0x74 = Ego_r.pdb CConfigOptionsDefaultsDef 0x78 less 4
// Label: the manifest's "CBlurredAttitudesDef" is propagated (family J048: five byte-similar def constructors; the stored vtable names each class; CBlurredAttitudesDef's own constructor would also build its Attitudes vector).
// Callees: CResource::CResource() 0x009FBEC0 (??0CResource@@QAE@XZ, catalog; it stores the CResource vtable and IVRefCount 1).
// Layout: Ego_r.pdb CDefClassBase (0x25, packed: PDefManager +0x1C, GlobalIndex +0x20, Setup/Template/DefaultValsApplied bits +0x24) and
//   CParentDefClassBase (InstantiationName +0x25, SubDefInfoMap +0x29: retail CVectorMap 0x10 = CArray Data, Compare +0xC, Dirty +0xD);
//   the derived def at natural alignment, so its first member lands at 0x3C (Ego_r +0x40 less the 4 bytes of the larger debug STLport vector).
//   Members per Ego_r.pdb CConfigOptionsDefaultsDef (retail offset = Ego_r offset - 4); retail sizeof 0x74 (vtable slot 20).
// Shape: no intermediate vtable stores survive (each inlined base constructor's store is dead before the next); the members are zeroed from
//   one register and the most-derived vtable is stored last.
// Model: local -- engine/CResource.h, CDefClassBase.h and CParentDefClassBase.h flatten the vptr (data-only), so the polymorphic bases are written out here.
class CResourceList;
class CResource {                                        // Ego_r.pdb 0x1C (CIVCountedPointeeBase: vptr, IVRefCount)
public:
    CResource();                                         // 0x009FBEC0
    virtual ~CResource();                                // 0x009FC550
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    virtual ~CDefPointeeBase() { }
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    CDefClassBase() : PDefManager(0)
    {
        Setup = false;
        Template = false;
        DefaultValsApplied = false;
    }
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
class CDefString {
public:
    CDefString() : TablePos(-1) { }
    long TablePos;
};
#include "engine/CArray.h"
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    CVectorMap() : Dirty(false) { }
    CArray<std::pair<K, V> > Data;                       // +0x00
    TCompare                 Compare;                    // +0x0C
    bool                     Dirty;                      // +0x0D
};
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {       // vtable 0x01230CBC
public:
    CDefString InstantiationName;                        // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
class CConfigOptionsDefaultsDef : public CParentDefClassBase {               // vtable 0x0123125C
public:
    CConfigOptionsDefaultsDef();
    unsigned long  Antialiasing;                            // +0x3C (Ego_r +0x40)
    long           ResolutionWidth;                         // +0x40 (Ego_r +0x44)
    long           ResolutionHeight;                        // +0x44 (Ego_r +0x48)
    long           BitDepth;                                // +0x48 (Ego_r +0x4C)
    long           MinResolutionWidth;                      // +0x4C (Ego_r +0x50)
    long           MinResolutionHeight;                     // +0x50 (Ego_r +0x54)
    float          TextureDetail;                           // +0x54 (Ego_r +0x58)
    float          MaxTextureDetail;                        // +0x58 (Ego_r +0x5C)
    float          ShadowDetail;                            // +0x5C (Ego_r +0x60)
    float          MaxShadowDetail;                         // +0x60 (Ego_r +0x64)
    float          MeshDetail;                              // +0x64 (Ego_r +0x68)
    float          MaxMeshDetail;                           // +0x68 (Ego_r +0x6C)
    float          EffectsDetail;                           // +0x6C (Ego_r +0x70)
    float          MaxEffectsDetail;                        // +0x70 (Ego_r +0x74)
};
CConfigOptionsDefaultsDef::CConfigOptionsDefaultsDef()
{
}
