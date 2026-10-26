// CClockDef::Transfer(CPersistContext& context) -- persist the CClockDef's fields (2 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CClockDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E448D, 33 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CClockDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x01242C34, whose RTTI type descriptor is .?AVCClockDef@@ (bases CSubDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CClockDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CClockDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J080: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(std::map<CCharString, long >&) 0x00431493, Transfer<EClockHandType>(EClockHandType&) 0x004E44AE.
// Layout: Ego_r.pdb CClockDef members rebased to retail (def base at its PDB size, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
// Shape: two members, so no edi: this in esi, the context re-read from its stack slot (`mov ecx,[esp+8]`) before each call,
//   the first address through eax, the last formed in place (`add esi,N`).
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
enum EClockHandType { };                               // Ego_r.pdb (enumerators not needed)
class CCharString {                                      // 4 bytes: the shared string data pointer
public:
    void* PStringData;
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
class CSubDefClassBase : public CDefClassBase {         // vtable 0x01231D54, Ego_r.pdb 0x25 (packed, no members of its own)
};
#pragma pack(pop)
class CPersistContext {
public:
    void Transfer(std::map<CCharString, long >& value);          // 0x00431493
    template <class T> void Transfer(T& value);              // enum instances: EClockHandType 0x004E44AE
};
class CClockDef : public CSubDefClassBase {    // vtable 0x01242C34
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x01242C34 slot 18
    std::map<CCharString, long > Sound;                // +0x28
    EClockHandType HandType;                           // +0x34 (Ego_r +0x38)
};

void CClockDef::Transfer(CPersistContext& context)
{
    context.Transfer(Sound);
    context.Transfer(HandType);
}