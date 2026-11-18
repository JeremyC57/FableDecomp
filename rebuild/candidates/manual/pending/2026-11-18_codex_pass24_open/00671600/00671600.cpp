// CThingCreatureDef::Transfer -- Transfer the creature definition fields and normalize navigator types for text persistence.
// Grade: DIFFER(1449v1450) -- At 00671B06 the byte temporary and flag storage enter AL and CL in the opposite order while updating IsIndestructible. The corresponding shift and mask reverse as well.
// Leaf: ?Transfer@CThingCreatureDef@@UAEXAAVCPersistContext@@@Z
// Retail: 0x00671600, 1450 bytes, thiscall.
// Flags: /Gr
#define FABLE_RETAIL_CTHINGCREATUREDEF_TRANSFER_PASS24
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CPERSISTCONTEXT_ISBINARY
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/CPersistContext.h"
#include "engine/CThingCreatureDef.h"

namespace std {
template <class _Tp> struct equal_to {
    equal_to() { }
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x == __y; }
};
template <class _Tp> inline equal_to<_Tp> __equal_to(_Tp*) { return equal_to<_Tp>(); }
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter> void sort(_RandomAccessIter __first, _RandomAccessIter __last);                    // 0x006720B0
template <class _ForwardIter, class _BinaryPredicate>
_ForwardIter adjacent_find(_ForwardIter __first, _ForwardIter __last, _BinaryPredicate __binary_pred);                  // 0x00671BB0
template <class _InputIterator, class _ForwardIter, class _BinaryPredicate>
_ForwardIter __unique_copy(_InputIterator __first, _InputIterator __last, _ForwardIter __result,
                           _BinaryPredicate __binary_pred, const forward_iterator_tag&);                                    // 0x00671BE0
template <class _ForwardIter> inline _ForwardIter adjacent_find(_ForwardIter __first, _ForwardIter __last)
{
    return adjacent_find(__first, __last, __equal_to(__value_type(__first)));
}
template <class _InputIter, class _OutputIter>
inline _OutputIter unique_copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    if (__first == __last) return __result;
    return __unique_copy(__first, __last, __result, __equal_to(__value_type(__first)), random_access_iterator_tag());
}
template <class _ForwardIter> inline _ForwardIter unique(_ForwardIter __first, _ForwardIter __last)
{
    __first = adjacent_find(__first, __last);
    return unique_copy(__first, __last, __first);
}
}

#define TRANSFER_FLAG(name, def) { bool temp = name; persist.Transfer(#name, temp, def); name = temp; }

void CThingCreatureDef::Transfer(CPersistContext& persist)
{
    CThingBaseDef::Transfer(persist);
    persist.Transfer(StunTimeMin);
    persist.Transfer(StunTimeMax);
    persist.Transfer(StunHealthThreshold);
    persist.Transfer(DazedTimeMin);
    persist.Transfer(DazedTimeMax);
    persist.Transfer(DazedHealthThreshold);
    persist.Transfer(NumSecondsToPersistOnDeath);
    persist.Transfer(MaxSlowWalkingSpeed);
    persist.Transfer(MaxWalkingSpeed);
    persist.Transfer(MaxJoggingSpeed);
    persist.Transfer(MaxRunningSpeed);
    persist.Transfer(MaxPushingSpeed);
    persist.Transfer(MaxFlyingSpeed);
    persist.Transfer(MaxHoverSpeedSlow);
    persist.Transfer(MaxHoverSpeedMedium);
    persist.Transfer(MaxHoverSpeedFast);
    persist.Transfer(SpeedRandomisationFactor);
    persist.Transfer(MaxAcceleration);
    persist.Transfer(HearingRadius);
    persist.Transfer(FOV);
    persist.Transfer(Sex);
    persist.Transfer(CreatureType);
    persist.TransferVector(NavigatorTypes);
    if (!persist.IsBinary()) {
        std::sort(NavigatorTypes.begin(), NavigatorTypes.end());
        NavigatorTypes.erase(std::unique(NavigatorTypes.begin(), NavigatorTypes.end()), NavigatorTypes.end());
    }
    persist.Transfer(InitialWeaponDef);
    persist.Transfer(PrimaryMeleeWeaponDef);
    persist.Transfer(SecondaryMeleeWeaponDef);
    persist.Transfer(RangedWeaponDef);
    persist.Transfer(InitialSheathedWeapon);
    persist.Transfer(BrainDef);
    persist.Transfer(CreatureInteractionType);
    persist.Transfer(Property);
    persist.Transfer(Damage);
    persist.Transfer(DamageEffectDef);
    persist.Transfer(StunnedParticle);
    persist.Transfer(Graphic);
    persist.Transfer(EyeGraphic);
    persist.Transfer(DefaultOwner);
    persist.Transfer(ControlledMovementType);
    persist.Transfer(CombatTypeDef);
    persist.Transfer(ResetCombatTypeOnUnsheatheDefaultWeapons);
    persist.Transfer(HeroCombatDef);
    persist.Transfer(HeroCombatPCDef);
    persist.Transfer(CombatDiameter);
    {
        bool temp = DeferHitsIfBehindHitter;
        persist.Transfer("DeferHitsIfBehindHitter", temp, true);
        DeferHitsIfBehindHitter = temp;
    }
    persist.Transfer(LeaderTypeDef);
    persist.Transfer(RecoverEvadeHitsPercent);
    persist.Transfer(DeadBodyDecayEffect);
    {
        bool temp = LeaveDeadCreature;
        persist.Transfer("LeaveDeadCreature", temp, false);
        LeaveDeadCreature = temp;
    }
    {
        bool temp = AbleToStrafe;
        persist.Transfer("AbleToStrafe", temp, false);
        AbleToStrafe = temp;
    }
    {
        bool temp = CreateDummyOnDeath;
        persist.Transfer("CreateDummyOnDeath", temp, false);
        CreateDummyOnDeath = temp;
    }
    {
        bool temp = ImmuneToAttack;
        persist.Transfer("ImmuneToAttack", temp, false);
        ImmuneToAttack = temp;
    }
    {
        bool temp = UseMeleeWeaponAsDefault;
        persist.Transfer("UseMeleeWeaponAsDefault", temp, true);
        UseMeleeWeaponAsDefault = temp;
    }
    {
        bool temp = UnsheatheWeaponsAutomatically;
        persist.Transfer("UnsheatheWeaponsAutomatically", temp, true);
        UnsheatheWeaponsAutomatically = temp;
    }
    {
        bool temp = IsWillable;
        persist.Transfer("IsWillable", temp, false);
        IsWillable = temp;
    }
    {
        bool temp = IsIndestructible;
        persist.Transfer("IsIndestructible", temp, false);
        IsIndestructible = temp;
    }
    {
        bool temp = IsInvulnerableDuringGenericResponse;
        persist.Transfer("IsInvulnerableDuringGenericResponse", temp, false);
        IsInvulnerableDuringGenericResponse = temp;
    }
    {
        bool temp = IsPushedByExplosions;
        persist.Transfer("IsPushedByExplosions", temp, true);
        IsPushedByExplosions = temp;
    }
}
