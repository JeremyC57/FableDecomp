// CMeleeCombatAbilityDef::Transfer(CPersistContext& context) -- persist the CMeleeCombatAbilityDef's fields (14 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CMeleeCombatAbilityDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00463BFF, 167 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CMeleeCombatAbilityDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x01234D5C, whose RTTI type descriptor is .?AVCMeleeCombatAbilityDef@@ (bases CParentDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CMeleeCombatAbilityDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CMeleeCombatAbilityDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J057: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(CDefString&) 0x0045228F, Transfer(float&) 0x00431061, Transfer(bool&) 0x0043314A, Transfer(long&) 0x00431102, Transfer(std::vector<EWeaponClass >&) 0x00463F3C, Transfer(CHeroAbilityStatPackage&) 0x00463CA6.
// Layout: Ego_r.pdb CMeleeCombatAbilityDef members rebased to retail (def base 4 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
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
enum EHeroTrainableStatType { };                       // Ego_r.pdb (enumerators not needed)
enum EWeaponClass { };                                 // Ego_r.pdb (enumerators not needed)
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes (natural packing)
public:
    CArray<std::pair<K, V> > Data;                       // +0x00
    TCompare                 Compare;                    // +0x0C
    bool                     Dirty;                      // +0x0D
};
class CResourceList;
class CDefinitionManager;
#pragma pack(push, 1)
// Layout: the def chain is the retail one from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, vtable 0x01230CBC,
//   0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98, operator= 0x00431ED0);
//   FABLE_PDCB_TYPED_MEMBERS: InstantiationName / SubDefInfoMap are typed with this row's CDefString / CVectorMap.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)
class CHeroAbilityStatPackage {                           // Ego_r.pdb 16 bytes, retail 12 (each STLport container 12 bytes)
public:
    std::map<EHeroTrainableStatType, long >  StatThresholds;
};
class CPersistContext {
public:
    void Transfer(CDefString& value);                            // 0x0045228F
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(bool& value);                                  // 0x0043314A
    void Transfer(long& value);                                  // 0x00431102
    void Transfer(std::vector<EWeaponClass >& value);            // 0x00463F3C
    void Transfer(CHeroAbilityStatPackage& value);               // 0x00463CA6
};
class CMeleeCombatAbilityDef : public CParentDefClassBase {    // vtable 0x01234D5C
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x01234D5C slot 18
    CDefString FlourishAnimName;                       // +0x3C (Ego_r +0x40)
    CDefString FlourishResponseAnimName;               // +0x40 (Ego_r +0x44)
    float DamageMultiplier;                            // +0x44 (Ego_r +0x48)
    bool Flourish;                                     // +0x48 (Ego_r +0x4C)
    bool BlockCounter;                                 // +0x49 (Ego_r +0x4D)
    long AbilityButtonGraphicOverride;                 // +0x4C (Ego_r +0x50)
    std::vector<EWeaponClass > ValidWeaponClasses;     // +0x50 (Ego_r +0x54)
    bool Decapitate;                                   // +0x5C (Ego_r +0x64)
    long KnockDownEffects;                             // +0x60 (Ego_r +0x68)
    bool EvadeAllHits;                                 // +0x64 (Ego_r +0x6C)
    bool CauseRecoil;                                  // +0x65 (Ego_r +0x6D)
    CHeroAbilityStatPackage StatsNeeded;               // +0x68 (Ego_r +0x70)
    long ComboMultiplierLowerBound;                    // +0x74 (Ego_r +0x80)
    long ComboMultiplierUpperBound;                    // +0x78 (Ego_r +0x84)
};

void CMeleeCombatAbilityDef::Transfer(CPersistContext& context)
{
    context.Transfer(FlourishAnimName);
    context.Transfer(FlourishResponseAnimName);
    context.Transfer(DamageMultiplier);
    context.Transfer(Flourish);
    context.Transfer(BlockCounter);
    context.Transfer(AbilityButtonGraphicOverride);
    context.Transfer(ValidWeaponClasses);
    context.Transfer(Decapitate);
    context.Transfer(KnockDownEffects);
    context.Transfer(EvadeAllHits);
    context.Transfer(CauseRecoil);
    context.Transfer(StatsNeeded);
    context.Transfer(ComboMultiplierLowerBound);
    context.Transfer(ComboMultiplierUpperBound);
}