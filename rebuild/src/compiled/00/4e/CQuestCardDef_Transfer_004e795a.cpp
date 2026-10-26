// CQuestCardDef::Transfer(CPersistContext& context) -- persist the CQuestCardDef's fields (18 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CQuestCardDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E795A, 211 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CQuestCardDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x01241E44, whose RTTI type descriptor is .?AVCQuestCardDef@@ (bases CSubDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CQuestCardDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CQuestCardDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J063: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(long&) 0x00431102, Transfer(CDefString&) 0x0045228F, Transfer(std::vector<long >&) 0x00466A47, Transfer(bool&) 0x0043314A, Transfer(std::map<EQuestCardPrerequisite, long >&) 0x004E7A2D, Transfer(unsigned long&) 0x00431020.
// Layout: Ego_r.pdb CQuestCardDef members rebased to retail (def base at its PDB size, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
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
enum EQuestCardPrerequisite { };                       // Ego_r.pdb (enumerators not needed)
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
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(CDefString& value);                            // 0x0045228F
    void Transfer(std::vector<long >& value);                    // 0x00466A47
    void Transfer(bool& value);                                  // 0x0043314A
    void Transfer(std::map<EQuestCardPrerequisite, long >& value); // 0x004E7A2D
    void Transfer(unsigned long& value);                         // 0x00431020
};
class CQuestCardDef : public CSubDefClassBase {    // vtable 0x01241E44
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x01241E44 slot 18
    long QuestName;                                    // +0x28
    long QuestSummary;                                 // +0x2C
    long QuestObjective;                               // +0x30
    long SuccessSummary;                               // +0x34
    CDefString RegionName;                             // +0x38
    CDefString TeleporterRegionName;                   // +0x3C
    long InventoryCategory;                            // +0x40
    long RenownReward;                                 // +0x44
    long GoldReward;                                   // +0x48
    std::vector<long > RewardObjects;                  // +0x4C
    bool IsCore;                                       // +0x58 (Ego_r +0x5C)
    bool IsVignette;                                   // +0x59 (Ego_r +0x5D)
    bool IsExclusive;                                  // +0x5A (Ego_r +0x5E)
    bool MakeVignetteRouteAppearOnMinimap;             // +0x5B (Ego_r +0x5F)
    std::map<EQuestCardPrerequisite, long > Prerequisites; // +0x5C (Ego_r +0x60)
    long NumBoasts;                                    // +0x68 (Ego_r +0x70)
    bool CanPlayerCancel;                              // +0x6C (Ego_r +0x74)
    unsigned long QuestEpilogue;                       // +0x70 (Ego_r +0x78)
};

void CQuestCardDef::Transfer(CPersistContext& context)
{
    context.Transfer(QuestName);
    context.Transfer(QuestSummary);
    context.Transfer(QuestObjective);
    context.Transfer(SuccessSummary);
    context.Transfer(RegionName);
    context.Transfer(TeleporterRegionName);
    context.Transfer(InventoryCategory);
    context.Transfer(RenownReward);
    context.Transfer(GoldReward);
    context.Transfer(RewardObjects);
    context.Transfer(IsCore);
    context.Transfer(IsVignette);
    context.Transfer(IsExclusive);
    context.Transfer(MakeVignetteRouteAppearOnMinimap);
    context.Transfer(Prerequisites);
    context.Transfer(NumBoasts);
    context.Transfer(CanPlayerCancel);
    context.Transfer(QuestEpilogue);
}