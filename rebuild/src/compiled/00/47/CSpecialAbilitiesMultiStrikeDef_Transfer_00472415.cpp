// CSpecialAbilitiesMultiStrikeDef::Transfer(CPersistContext& context) -- persist the CSpecialAbilitiesMultiStrikeDef's fields (14 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CSpecialAbilitiesMultiStrikeDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00472415, 173 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CSpecialAbilitiesMultiStrikeDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x0123412C, whose RTTI type descriptor is .?AVCSpecialAbilitiesMultiStrikeDef@@ (bases CParentDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CSpecialAbilitiesMultiStrikeDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CSpecialAbilitiesMultiStrikeDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J057: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(CDefString&) 0x0045228F, Transfer(float&) 0x00431061, Transfer(std::vector<long >&) 0x00466A47, Transfer(std::vector<float >&) 0x00464CBF, Transfer(std::vector<CCharString >&) 0x004331F9.
// Layout: Ego_r.pdb CSpecialAbilitiesMultiStrikeDef members rebased to retail (def base 4 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
// Shape: context in edi, this in esi, one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member, the last address formed in place (`add esi,N`).
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { };   // STLport, 12 bytes
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                                              // STLport, 12 bytes (its _Rb_tree: header, count, compare)
public:
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}   // namespace std
template <class T> class CArray : public std::vector<T> { };   // 12 bytes in retail (16 in Ego_r.pdb)
class CDefString {                                       // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
class CCharString {                                      // 4 bytes: the shared string data pointer
public:
    void* PStringData;
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes (natural packing)
public:
    CArray<std::pair<K, V> > Data;                       // +0x00
    TCompare                 Compare;                    // +0x0C
    bool                     Dirty;                      // +0x0D
};
class CResourceList;
class CResource {                                        // Ego_r.pdb 0x1C (CIVCountedPointeeBase: vptr, IVRefCount)
public:
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
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CParentDefClassBase : public CDefClassBase {       // vtable 0x01230CBC, retail 0x39 (packed)
public:
    CDefString InstantiationName;                        // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)
class CPersistContext {
public:
    void Transfer(CDefString& value);                            // 0x0045228F
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(std::vector<long >& value);                    // 0x00466A47
    void Transfer(std::vector<float >& value);                   // 0x00464CBF
    void Transfer(std::vector<CCharString >& value);             // 0x004331F9
};
class CSpecialAbilitiesMultiStrikeDef : public CParentDefClassBase {    // vtable 0x0123412C
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x0123412C slot 18
    CDefString Sounds[4];                              // +0x3C (Ego_r +0x40)
    CDefString Animations[4];                          // +0x4C (Ego_r +0x50)
    float HighlightWidth;                              // +0x5C (Ego_r +0x60)
    std::vector<long > HighlightColor;                 // +0x60 (Ego_r +0x64)
    std::vector<float > DelayTimeSecs;                 // +0x6C (Ego_r +0x74)
    std::vector<long > NumHits;                        // +0x78 (Ego_r +0x84)
    std::vector<float > PerHitDamagerMultiplier;       // +0x84 (Ego_r +0x94)
    std::vector<CCharString > AttachmentPoint;         // +0x90 (Ego_r +0xA4)
};

void CSpecialAbilitiesMultiStrikeDef::Transfer(CPersistContext& context)
{
    context.Transfer(Sounds[0]);
    context.Transfer(Sounds[1]);
    context.Transfer(Sounds[2]);
    context.Transfer(Sounds[3]);
    context.Transfer(Animations[0]);
    context.Transfer(Animations[1]);
    context.Transfer(Animations[2]);
    context.Transfer(Animations[3]);
    context.Transfer(HighlightWidth);
    context.Transfer(HighlightColor);
    context.Transfer(DelayTimeSecs);
    context.Transfer(NumHits);
    context.Transfer(PerHitDamagerMultiplier);
    context.Transfer(AttachmentPoint);
}