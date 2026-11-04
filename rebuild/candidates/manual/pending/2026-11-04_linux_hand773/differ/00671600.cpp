// CThingCreatureDef::Transfer(CPersistContext& context) -- persist the creature definition: the base thing def, then the stun / daze / speed / sense floats, sex and creature type, the navigator types (sorted and de-duplicated outside binary mode), the weapon / brain defs, interaction type, property, damage, graphics, owner, movement / combat defs, and the named flag bits with their defaults
// Grade: DIFFER(1449v1450) -- IsIndestructible (bit 7 of +0x148) merge: retail loads the flag first (mov al,[flag]; mov cl,[old]; and cl,7f; shl al,7; or al,cl), VC7.1 here loads the old byte first (and al,7f, 1 byte shorter); 10 grades
// Leaf: ?Transfer@CThingCreatureDef@@UAEXAAVCPersistContext@@@Z
// Flags: /Gr
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
enum ESex { };
enum ECreatureType { };
enum ENavigatorType { };
enum ECreatureInteractionType { };
enum EThingCreatureProperty { };
enum EControlledMovementType { };
#pragma pack(push, 2)
class CEngineGraphic { public: unsigned char _body[14]; };
#pragma pack(pop)

class CPersistContext {
public:
    enum EMode { MODE_LOAD_TEXT=0,MODE_SAVE_TEXT=1,MODE_LOAD_BINARY=2,MODE_SAVE_BINARY=3,MODE_CLEAR=4 };
    template<class T> void Transfer(T&);
    template<class T> void Transfer(const char*,T&,const T&);
    template<class T> void TransferVector(std::vector<T,std::allocator<T> >&);
    bool IsBinary() const { return Mode==MODE_LOAD_BINARY||Mode==MODE_SAVE_BINARY; }
protected:
    unsigned char _pad00[0x18];
    EMode Mode;                                         // +0x18
};

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

class CThingBaseDef {
public:
    virtual void Transfer(CPersistContext& context);    // 0x00676BE0
    unsigned char _body[0x70 - 4];
};

class CThingCreatureDef : public CThingBaseDef {
public:
    virtual void Transfer(CPersistContext& context);
    float StunTimeMin;                                  // +0x70
    float StunTimeMax;                                  // +0x74
    float StunHealthThreshold;                          // +0x78
    float DazedTimeMin;                                 // +0x7c
    float DazedTimeMax;                                 // +0x80
    long  DazedHealthThreshold;                         // +0x84
    float NumSecondsToPersistOnDeath;                   // +0x88
    float MaxSlowWalkingSpeed;                          // +0x8c
    float MaxWalkingSpeed;                              // +0x90
    float MaxJoggingSpeed;                              // +0x94
    float MaxRunningSpeed;                              // +0x98
    float MaxPushingSpeed;                              // +0x9c
    float MaxFlyingSpeed;                               // +0xa0
    float MaxHoverSpeedSlow;                            // +0xa4
    float MaxHoverSpeedMedium;                          // +0xa8
    float MaxHoverSpeedFast;                            // +0xac
    float SpeedRandomisationFactor;                     // +0xb0
    float MaxAcceleration;                              // +0xb4
    float HearingRadius;                                // +0xb8
    float FOV;                                          // +0xbc
    ESex  Sex;                                          // +0xc0
    ECreatureType CreatureType;                         // +0xc4
    std::vector<ENavigatorType> NavigatorTypes;         // +0xc8
    long  InitialWeaponDef;                             // +0xd4
    long  PrimaryMeleeWeaponDef;                        // +0xd8
    long  SecondaryMeleeWeaponDef;                      // +0xdc
    long  RangedWeaponDef;                              // +0xe0
    long  InitialSheathedWeapon;                        // +0xe4
    long  BrainDef;                                     // +0xe8
    ECreatureInteractionType CreatureInteractionType;   // +0xec
    EThingCreatureProperty Property;                    // +0xf0
    float Damage;                                       // +0xf4
    long  DamageEffectDef;                              // +0xf8
    long  StunnedParticle;                              // +0xfc
    CEngineGraphic Graphic;                             // +0x100
    CEngineGraphic EyeGraphic;                          // +0x10e
    long  DefaultOwner;                                 // +0x11c
    EControlledMovementType ControlledMovementType;     // +0x120
    long  CombatTypeDef;                                // +0x124
    bool  ResetCombatTypeOnUnsheatheDefaultWeapons;     // +0x128
    long  HeroCombatDef;                                // +0x12c
    long  HeroCombatPCDef;                              // +0x130
    float CombatDiameter;                               // +0x134
    bool  DeferHitsIfBehindHitter : 1;                  // +0x138
    long  LeaderTypeDef;                                // +0x13c
    float RecoverEvadeHitsPercent;                      // +0x140
    long  DeadBodyDecayEffect;                          // +0x144
    bool  LeaveDeadCreature : 1;                        // +0x148 bit 0
    bool  AbleToStrafe : 1;
    bool  CreateDummyOnDeath : 1;
    bool  ImmuneToAttack : 1;
    bool  UseMeleeWeaponAsDefault : 1;
    bool  UnsheatheWeaponsAutomatically : 1;
    bool  IsWillable : 1;
    bool  IsIndestructible : 1;
    bool  IsInvulnerableDuringGenericResponse : 1;      // +0x149 bit 0
    bool  IsPushedByExplosions : 1;
};

#define TRANSFER_FLAG(name, def) { bool flag = name; context.Transfer(#name, flag, def); name = flag; }

void CThingCreatureDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    context.Transfer(StunTimeMin);
    context.Transfer(StunTimeMax);
    context.Transfer(StunHealthThreshold);
    context.Transfer(DazedTimeMin);
    context.Transfer(DazedTimeMax);
    context.Transfer(DazedHealthThreshold);
    context.Transfer(NumSecondsToPersistOnDeath);
    context.Transfer(MaxSlowWalkingSpeed);
    context.Transfer(MaxWalkingSpeed);
    context.Transfer(MaxJoggingSpeed);
    context.Transfer(MaxRunningSpeed);
    context.Transfer(MaxPushingSpeed);
    context.Transfer(MaxFlyingSpeed);
    context.Transfer(MaxHoverSpeedSlow);
    context.Transfer(MaxHoverSpeedMedium);
    context.Transfer(MaxHoverSpeedFast);
    context.Transfer(SpeedRandomisationFactor);
    context.Transfer(MaxAcceleration);
    context.Transfer(HearingRadius);
    context.Transfer(FOV);
    context.Transfer(Sex);
    context.Transfer(CreatureType);
    context.TransferVector(NavigatorTypes);
    if (!context.IsBinary()) {
        std::sort(NavigatorTypes.begin(), NavigatorTypes.end());
        NavigatorTypes.erase(std::unique(NavigatorTypes.begin(), NavigatorTypes.end()), NavigatorTypes.end());
    }
    context.Transfer(InitialWeaponDef);
    context.Transfer(PrimaryMeleeWeaponDef);
    context.Transfer(SecondaryMeleeWeaponDef);
    context.Transfer(RangedWeaponDef);
    context.Transfer(InitialSheathedWeapon);
    context.Transfer(BrainDef);
    context.Transfer(CreatureInteractionType);
    context.Transfer(Property);
    context.Transfer(Damage);
    context.Transfer(DamageEffectDef);
    context.Transfer(StunnedParticle);
    context.Transfer(Graphic);
    context.Transfer(EyeGraphic);
    context.Transfer(DefaultOwner);
    context.Transfer(ControlledMovementType);
    context.Transfer(CombatTypeDef);
    context.Transfer(ResetCombatTypeOnUnsheatheDefaultWeapons);
    context.Transfer(HeroCombatDef);
    context.Transfer(HeroCombatPCDef);
    context.Transfer(CombatDiameter);
    TRANSFER_FLAG(DeferHitsIfBehindHitter, true)
    context.Transfer(LeaderTypeDef);
    context.Transfer(RecoverEvadeHitsPercent);
    context.Transfer(DeadBodyDecayEffect);
    TRANSFER_FLAG(LeaveDeadCreature, false)
    TRANSFER_FLAG(AbleToStrafe, false)
    TRANSFER_FLAG(CreateDummyOnDeath, false)
    TRANSFER_FLAG(ImmuneToAttack, false)
    TRANSFER_FLAG(UseMeleeWeaponAsDefault, true)
    TRANSFER_FLAG(UnsheatheWeaponsAutomatically, true)
    TRANSFER_FLAG(IsWillable, false)
    TRANSFER_FLAG(IsIndestructible, false)
    TRANSFER_FLAG(IsInvulnerableDuringGenericResponse, false)
    TRANSFER_FLAG(IsPushedByExplosions, true)
}
