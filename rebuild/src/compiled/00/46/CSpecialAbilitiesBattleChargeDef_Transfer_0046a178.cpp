// CSpecialAbilitiesBattleChargeDef::Transfer(CPersistContext& context) -- persist the CSpecialAbilitiesBattleChargeDef's fields (18 typed binary transfers) in the def's own TRANSFER order
// Leaf: ?Transfer@CSpecialAbilitiesBattleChargeDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046A178, 232 bytes, virtual thiscall `ret 4`; size-optimised def TU (unaligned entry, every member address through eax).
// Identity (proven): CSpecialAbilitiesBattleChargeDef::Transfer(CPersistContext&); evidence: slot 18 (the def Transfer slot, as the landed CHeroCombatDef 0x004665B6) of
//   vtable 0x012340C4, whose RTTI type descriptor is .?AVCSpecialAbilitiesBattleChargeDef@@ (bases CParentDefClassBase <- CDefClassBase); the Ego_r/FableWin.pdb name sets hold ?Transfer@CSpecialAbilitiesBattleChargeDef@@UAEXAAVCPersistContext@@@Z.
// Label: the manifest's "?Transfer@CSpecialAbilitiesBattleChargeDef@@UAEXAAVCPersistContext@@@Z" agrees with the RTTI (family J063: one-transfer-per-member def bodies, each its own class).
// Callees (CPersistContext's typed transfers): Transfer(float&) 0x00431061, Transfer(std::vector<float >&) 0x00464CBF, Transfer(std::vector<long >&) 0x00466A47.
// Layout: Ego_r.pdb CSpecialAbilitiesBattleChargeDef members rebased to retail (def base 4 bytes shorter, each STLport container / CArray 12 bytes, 16 in the PDB); every transferred offset hits the PDB member of the asserted type.
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
class CPersistContext {
public:
    void Transfer(float& value);                                 // 0x00431061
    void Transfer(std::vector<float >& value);                   // 0x00464CBF
    void Transfer(std::vector<long >& value);                    // 0x00466A47
};
class CSpecialAbilitiesBattleChargeDef : public CParentDefClassBase {    // vtable 0x012340C4
public:
    virtual void Transfer(CPersistContext& context);   // vtable 0x012340C4 slot 18
    float RunSpeed;                                    // +0x3C (Ego_r +0x40)
    std::vector<float > DashDistance;                  // +0x40 (Ego_r +0x44)
    std::vector<float > RadiusOfEffect;                // +0x4C (Ego_r +0x54)
    std::vector<float > BattleChargeDamage;            // +0x58 (Ego_r +0x64)
    float BattleChargeCombatMultiplierFactor;          // +0x64 (Ego_r +0x74)
    float BattleChargeRadialStartSeconds;              // +0x68 (Ego_r +0x78)
    float BattleChargeRadialEndSeconds;                // +0x6C (Ego_r +0x7C)
    float BattleChargeRadialThreshold;                 // +0x70 (Ego_r +0x80)
    float BattleChargeRadialLength;                    // +0x74 (Ego_r +0x84)
    float BattleChargeRadialIntensity;                 // +0x78 (Ego_r +0x88)
    float BattleChargeRadialFade;                      // +0x7C (Ego_r +0x8C)
    float BattleChargeRadialRange;                     // +0x80 (Ego_r +0x90)
    float BattleChargeRadialTwist;                     // +0x84 (Ego_r +0x94)
    float PoseDelaySecs;                               // +0x88 (Ego_r +0x98)
    float ChargeDelay;                                 // +0x8C (Ego_r +0x9C)
    std::vector<long > NumPeopleHit;                   // +0x90 (Ego_r +0xA0)
    float DragonExtraGumphRadiusMultiplier;            // +0x9C (Ego_r +0xB0)
    float DragonExtraGumphRadius;                      // +0xA0 (Ego_r +0xB4)
};

void CSpecialAbilitiesBattleChargeDef::Transfer(CPersistContext& context)
{
    context.Transfer(RunSpeed);
    context.Transfer(DashDistance);
    context.Transfer(RadiusOfEffect);
    context.Transfer(BattleChargeDamage);
    context.Transfer(BattleChargeCombatMultiplierFactor);
    context.Transfer(BattleChargeRadialStartSeconds);
    context.Transfer(BattleChargeRadialEndSeconds);
    context.Transfer(BattleChargeRadialThreshold);
    context.Transfer(BattleChargeRadialLength);
    context.Transfer(BattleChargeRadialIntensity);
    context.Transfer(BattleChargeRadialFade);
    context.Transfer(BattleChargeRadialRange);
    context.Transfer(BattleChargeRadialTwist);
    context.Transfer(PoseDelaySecs);
    context.Transfer(ChargeDelay);
    context.Transfer(NumPeopleHit);
    context.Transfer(DragonExtraGumphRadiusMultiplier);
    context.Transfer(DragonExtraGumphRadius);
}