// CConfigOptionsDefaultsDef::Alloc() -- the def class's static allocator: `new CConfigOptionsDefaultsDef` (inlined default constructor), or null when the allocation fails
// Leaf: ?Alloc@CConfigOptionsDefaultsDef@@SIPAVCDefClassBase@@XZ
// Retail: Fable.exe @ 0x00434660, 73 bytes, static __fastcall (no arguments), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?Alloc@CConfigOptionsDefaultsDef@@SIPAVCDefClassBase@@XZ; the object it builds stores vtable 0x0123125C
// (RTTI .?AVCConfigOptionsDefaultsDef@@, bases CParentDefClassBase / CDefClassBase / CDefPointeeBase / CResource), and the
// `operator new(0x74)` size matches the class (its size slot 20 returns 0x74). Callers: CStartupDefinitionManager::InitAndCompilePlatformSpecific (0x00433C48) and CGameDefinitionManager::InitAndCompile (0x0044E8F3) register it.
// Callees: operator new @ 0x00BFEA1A (MSVCR71 scalar new; the `cmp esi,ebx ; je` is the new-expression's null guard),
// CResource::CResource() @ 0x009FBEC0.
// Shape: the inlined constructor chain (the landed ??0CConfigOptionsDefaultsDef@@QAE@XZ 0x00434240 body), the vtable store last.
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
class CConfigOptionsDefaultsDef : public CParentDefClassBase {               // vtable 0x0123125C
public:
    CConfigOptionsDefaultsDef() { }
    static CDefClassBase* __fastcall Alloc();
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

CDefClassBase* __fastcall CConfigOptionsDefaultsDef::Alloc()
{
    return new CConfigOptionsDefaultsDef;
}