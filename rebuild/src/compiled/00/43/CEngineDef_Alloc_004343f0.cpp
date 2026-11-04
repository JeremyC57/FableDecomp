// CEngineDef::Alloc() -- the def class's static allocator: `new CEngineDef` (inlined default constructor), or null when the allocation fails
// Leaf: ?Alloc@CEngineDef@@SIPAVCDefClassBase@@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe @ 0x004343F0, 76 bytes, static __fastcall (no arguments), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?Alloc@CEngineDef@@SIPAVCDefClassBase@@XZ; the object it builds stores vtable 0x0123118C
// (RTTI .?AVCEngineDef@@, bases CParentDefClassBase / CDefClassBase / CDefPointeeBase / CResource), and the
// `operator new(0x80)` size matches the class (its size slot 20 returns 0x80). Callers: CStartupDefinitionManager::InitAndCompilePlatformSpecific (0x00433B94) and CGameDefinitionManager::InitAndCompile (0x0044D2AE) register it.
// Callees: operator new @ 0x00BFEA1A (MSVCR71 scalar new; the `cmp esi,ebx ; je` is the new-expression's null guard),
// CResource::CResource() @ 0x009FBEC0.
// Shape: the inlined constructor chain (the landed out-of-line ??0CEngineDef@@QAE@XZ 0x00433F90 body), the vtable store last; the float/long members stay uninitialised.
// Model: the def-base chain of the landed CEngineDef::CEngineDef 0x00433F90 (engine/CResource.h, CDefClassBase.h and
// CParentDefClassBase.h flatten the vptr, so the polymorphic bases are written out locally; NOTES.md "Header debt").
void* __cdecl operator new(unsigned int size);           // 0x00BFEA1A
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
class CEngineDef : public CParentDefClassBase {               // vtable 0x0123118C
public:
    CEngineDef() { }
    static CDefClassBase* __fastcall Alloc();
    float          LODErrorTolerance;                       // +0x3C (Ego_r +0x40)
    float          CharacterLODErrorTolerance;              // +0x40 (Ego_r +0x44)
    float          LODErrorFactor;                          // +0x44 (Ego_r +0x48)
    float          SeaHeight;                               // +0x48 (Ego_r +0x4C)
    float          GamePrimitiveDefaultFadeStart;           // +0x4C (Ego_r +0x50)
    float          GamePrimitiveDefaultFadeRangeRatio;      // +0x50 (Ego_r +0x54)
    float          LocalDetailDefaultFadeStart;             // +0x54 (Ego_r +0x58)
    float          LocalDetailDefaultFadeRangeRatio;        // +0x58 (Ego_r +0x5C)
    float          FOV_2D;                                  // +0x5C (Ego_r +0x60)
    long           LocalDetailBooleanAlphaDefaultAlphaRef;  // +0x60 (Ego_r +0x64)
    long           DefaultPrimitiveAlphaRef;                // +0x64 (Ego_r +0x68)
    long           TestStaticMesh;                          // +0x68 (Ego_r +0x6C)
    long           TestAnimatedMesh;                        // +0x6C (Ego_r +0x70)
    long           TestAnim;                                // +0x70 (Ego_r +0x74)
    long           TestGraphic;                             // +0x74 (Ego_r +0x78)
    long           InvalidTextureStandin;                   // +0x78 (Ego_r +0x7C)
    long           InvalidThemeStandin;                     // +0x7C (Ego_r +0x80)
};

CDefClassBase* __fastcall CEngineDef::Alloc()
{
    return new CEngineDef;
}