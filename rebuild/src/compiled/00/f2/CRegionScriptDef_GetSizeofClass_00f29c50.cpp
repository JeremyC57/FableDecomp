// CRegionScriptDef::GetSizeofClass() -- slot 20: sizeof(CRegionScriptDef) = 0x4c
// Leaf: ?GetSizeofClass@CRegionScriptDef@@UAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F29C50, 6 bytes: b84c000000c3.
// Identity (naming wave 35, family def): Anniversary wave 2026-11-04 (high): vtable 0x012FB630 slot 20; Anniversary RTTI CRegionScriptDef (string-literal match, validated 257/258); PDB name set has the GetSizeofClass member of CRegionScriptDef; slot layout from named TLC siblings; function referenced by this vtable only; body=mov eax,imm;ret.
//   ?GetSizeofClass@CRegionScriptDef@@UAEKXZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv); PDB also: ??_ECRegionScriptDef@@UAEPAXI@Z, ??1CRegionScriptDef@@UAE@XZ.
// Was: Constant-return member: returns 0x4c. __fastcall this=ecx. (catalog stand-in _gapscan).
// Class: the class model of the landed constructor row 0x00F29C10 (rebuild/src/compiled/00/f2/CRegionScriptDef_CRegionScriptDef_00f29c10.cpp); the PDB member declaration added; only this member is graded.
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
class CRegionScriptDef : public CParentDefClassBase {               // vtable 0x012FB630
public:
    CRegionScriptDef();
    long           RandomVillagerMax;                       // +0x3C (Ego_r +0x40)
    long           RandomGuardMax;                          // +0x40 (Ego_r +0x44)
    long           RandomBanditMax;                         // +0x44 (Ego_r +0x48)
    long           RegionDangerLevel;                       // +0x48 (Ego_r +0x4C)
public:
    virtual unsigned long GetSizeofClass();   // PDB ?GetSizeofClass@CRegionScriptDef@@UAEKXZ
};
CRegionScriptDef::CRegionScriptDef()
{
}
unsigned long CRegionScriptDef::GetSizeofClass()
{
    return sizeof(CRegionScriptDef);
}
