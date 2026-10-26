// CEngineVideoOptionsDef::CEngineVideoOptionsDef() -- default constructor of a CParentDefClassBase-derived def: CResource base out of line, then the inlined def bases (PDefManager 0, the three flag bits cleared, InstantiationName -1, an empty SubDefInfoMap), then this class's vtable
// Leaf: ??0CEngineVideoOptionsDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004340D0, 51 bytes (+ int3 padding to 64), thiscall, plain `ret`, returns this.
// Identity (proven): CEngineVideoOptionsDef::CEngineVideoOptionsDef() (FableWin.pdb name set: ??0CEngineVideoOptionsDef@@QAE@XZ); evidence: RTTI of the stored vtable 0x012311F4 is .?AVCEngineVideoOptionsDef@@ (same def base chain); its size slot 20 (0x00434110) returns 0x78 = Ego_r.pdb CEngineVideoOptionsDef 0x7C less 4
// Label: the manifest's "CBlurredAttitudesDef" is propagated (family J048: five byte-similar def constructors; the stored vtable names each class; CBlurredAttitudesDef's own constructor would also build its Attitudes vector).
// Callees: CResource::CResource() 0x009FBEC0 (??0CResource@@QAE@XZ, catalog; it stores the CResource vtable and IVRefCount 1).
// Layout: Ego_r.pdb CDefClassBase (0x25, packed: PDefManager +0x1C, GlobalIndex +0x20, Setup/Template/DefaultValsApplied bits +0x24) and
//   CParentDefClassBase (InstantiationName +0x25, SubDefInfoMap +0x29: retail CVectorMap 0x10 = CArray Data, Compare +0xC, Dirty +0xD);
//   the derived def at natural alignment, so its first member lands at 0x3C (Ego_r +0x40 less the 4 bytes of the larger debug STLport vector).
//   Members per Ego_r.pdb CEngineVideoOptionsDef (retail offset = Ego_r offset - 4); retail sizeof 0x78 (vtable slot 20).
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
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc&, _Value __p) : _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc> class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}   // namespace std
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
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
class CEngineVideoOptionsDef : public CParentDefClassBase {               // vtable 0x012311F4
public:
    CEngineVideoOptionsDef();
    long           HiresTextureMemory;                      // +0x3C (Ego_r +0x40)
    float          LODErrorTolerance;                       // +0x40 (Ego_r +0x44)
    float          CharacterLODErrorTolerance;              // +0x44 (Ego_r +0x48)
    float          DrawDistanceMultiplier;                  // +0x48 (Ego_r +0x4C)
    float          DrawDistanceMinimum;                     // +0x4C (Ego_r +0x50)
    float          DrawDistanceMaximum;                     // +0x50 (Ego_r +0x54)
    float          RepeatedMeshDrawDistanceFactor;          // +0x54 (Ego_r +0x58)
    float          MinimumZSpriteAsMeshDistance;            // +0x58 (Ego_r +0x5C)
    float          MaximumZSpriteAsMeshDistance;            // +0x5C (Ego_r +0x60)
    float          ZSpriteDrawDistanceMultiplier;           // +0x60 (Ego_r +0x64)
    long           ShadowBufferSize;                        // +0x64 (Ego_r +0x68)
    float          ShadowDistanceScale;                     // +0x68 (Ego_r +0x6C)
    bool           Enable2DDisplacement;                    // +0x6C (Ego_r +0x70)
    bool           Enable3DDisplacement;                    // +0x6D (Ego_r +0x71)
    bool           EnableGlow;                              // +0x6E (Ego_r +0x72)
    bool           EnableRadialBlur;                        // +0x6F (Ego_r +0x73)
    bool           EnableWaterReflection;                   // +0x70 (Ego_r +0x74)
    bool           EnableWeatherEffects;                    // +0x71 (Ego_r +0x75)
    bool           EnableColourFilter;                      // +0x72 (Ego_r +0x76)
    bool           EnableRepeatedMeshes;                    // +0x73 (Ego_r +0x77)
    float          WeatherDensity;                          // +0x74 (Ego_r +0x78)
};
CEngineVideoOptionsDef::CEngineVideoOptionsDef()
{
}
