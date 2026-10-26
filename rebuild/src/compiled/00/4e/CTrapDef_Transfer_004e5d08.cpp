// CTrapDef::Transfer(CPersistContext& context) -- persist the CTrapDef's fields (13 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CTrapDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E5D08, 156 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CTrapDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x01243054, whose RTTI type descriptor is .?AVCTrapDef@@ (bases CSubDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CTrapDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CTrapDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J058: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(std::map<CCharString, long >&) 0x00431493, Transfer<ETrapTriggerType>(ETrapTriggerType&) 0x004E5DA4, Transfer<ETrapType>(ETrapType&) 0x004E5E48, Transfer(float&) 0x00431061, Transfer(long&) 0x00431102, Transfer(CDefString&) 0x0045228F.
// Layout: Ego_r.pdb CTrapDef members rebased to retail (def base at its PDB size, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type (the def transfers out of declaration order).
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
class CDefString {                                       // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
enum ETrapTriggerType { };                             // Ego_r.pdb (enumerators not needed)
enum ETrapType { };                                    // Ego_r.pdb (enumerators not needed)
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
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(CDefString& value);                            // 0x0045228F
    template <class T> void Transfer(T& value);              // enum instances: ETrapTriggerType 0x004E5DA4, ETrapType 0x004E5E48
};
class CTrapDef : public CSubDefClassBase {    // vtable 0x01243054
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x01243054 slot 18
    std::map<CCharString, long > Sound;                // +0x28
    ETrapTriggerType TriggerType;                      // +0x34 (Ego_r +0x38)
    ETrapType TrapType;                                // +0x38 (Ego_r +0x3C)
    float TriggerRadius;                               // +0x3C (Ego_r +0x40)
    CDefString TriggerHelperName;                      // +0x40 (Ego_r +0x44)
    float TrapDamage;                                  // +0x44 (Ego_r +0x48)
    long TriggeredCollisionMesh;                       // +0x48 (Ego_r +0x4C)
    long UntriggeredCollisionMesh;                     // +0x4C (Ego_r +0x50)
    long ExplosionDefIndex;                            // +0x50 (Ego_r +0x54)
    long PhysicalObstructionDefIndex;                  // +0x54 (Ego_r +0x58)
    long NumShotsToFireBeforeIdle;                     // +0x58 (Ego_r +0x5C)
    float SecondsIdle;                                 // +0x5C (Ego_r +0x60)
    float SecondsBetweenShots;                         // +0x60 (Ego_r +0x64)
};

void CTrapDef::Transfer(CPersistContext& context)
{
    context.Transfer(Sound);
    context.Transfer(TriggerType);
    context.Transfer(TrapType);
    context.Transfer(TriggerRadius);
    context.Transfer(TriggeredCollisionMesh);
    context.Transfer(UntriggeredCollisionMesh);
    context.Transfer(TriggerHelperName);
    context.Transfer(TrapDamage);
    context.Transfer(ExplosionDefIndex);
    context.Transfer(PhysicalObstructionDefIndex);
    context.Transfer(NumShotsToFireBeforeIdle);
    context.Transfer(SecondsIdle);
    context.Transfer(SecondsBetweenShots);
}