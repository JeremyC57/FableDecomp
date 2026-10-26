// CThingCreatureDef::CThingCreatureDef() -- default-construct the creature definition: CThingBaseDef part, zero every speed/stun/combat tuning value, the navigator-type vector, both graphics and the def indices, then the default flag bits (only IsPushedByExplosions set)
// Leaf: ??0CThingCreatureDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00670F70, length 331 bytes.
// Callees: CThingBaseDef::CThingBaseDef @ 0x006772D0. Vtable 0x0125B2DC (CThingCreatureDef, RTTI).
// Layout: Ego_r.pdb CThingCreatureDef (0x154) minus 4 up to the STLport vector (retail def base 4 bytes smaller, the
//   CThingBuildingDef rule) and minus 8 after it (PDB vector 16 bytes, retail 12): StunTimeMin +0x70 .. CreatureType
//   +0xc4, NavigatorTypes +0xc8, InitialWeaponDef +0xd4 .. StunnedParticle +0xfc, Graphic +0x100 and EyeGraphic +0x10e
//   (14-byte CEngineGraphic, pack(2)), DefaultOwner +0x11c .. DeadBodyDecayEffect +0x144, flag bytes +0x148/+0x149.
//   No generated engine header exists for the class.
// Shape: every member is zeroed in the mem-initialiser list; the vptr store sinks across the plain zero stores and
//   stops in front of the vector's inline ctor (VC7.1 never sinks an immediate across a member ctor). Members the
//   retail ctor leaves alone (InitialSheathedWeapon, ResetCombatType..., HeroCombat(PC)Def, DeferHits..., LeaderType's
//   neighbours) are simply not initialised. UnsheatheWeaponsAutomatically (bit 5) keeps its value: `and 20h`.
class CPersistContext;

class CThingBaseDef { public:
    CThingBaseDef();                                    // 0x006772D0
    virtual void Transfer(CPersistContext& context);
    unsigned char _body[0x70 - 4];
};

#pragma pack(push, 2)
class CEngineGraphic { public:
    unsigned long BankIndex;
    float         AnimStep;
    float         RenderSizeX;
    bool          AdditiveAlpha;
    unsigned char Type;
    CEngineGraphic() : BankIndex(0), AnimStep(0), RenderSizeX(0), AdditiveAlpha(false), Type(0) {}
};
#pragma pack(pop)

struct CNavigatorTypeVector {                           // STLport vector<ENavigatorType> (12 bytes in retail)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CNavigatorTypeVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};

class CThingCreatureDef : public CThingBaseDef { public:              // vtable 0x0125B2DC
    float  StunTimeMin;                                 // +0x70
    float  StunTimeMax;                                 // +0x74
    float  StunHealthThreshold;                         // +0x78
    float  DazedTimeMin;                                // +0x7c
    float  DazedTimeMax;                                // +0x80
    long   DazedHealthThreshold;                        // +0x84
    float  NumSecondsToPersistOnDeath;                  // +0x88
    float  MaxSlowWalkingSpeed;                         // +0x8c
    float  MaxWalkingSpeed;                             // +0x90
    float  MaxJoggingSpeed;                             // +0x94
    float  MaxRunningSpeed;                             // +0x98
    float  MaxPushingSpeed;                             // +0x9c
    float  MaxFlyingSpeed;                              // +0xa0
    float  MaxHoverSpeedSlow;                           // +0xa4
    float  MaxHoverSpeedMedium;                         // +0xa8
    float  MaxHoverSpeedFast;                           // +0xac
    float  SpeedRandomisationFactor;                    // +0xb0
    float  MaxAcceleration;                             // +0xb4
    float  HearingRadius;                               // +0xb8
    float  FOV;                                         // +0xbc
    long   Sex;                                         // +0xc0 ESex
    long   CreatureType;                                // +0xc4 ECreatureType
    CNavigatorTypeVector NavigatorTypes;                // +0xc8
    long   InitialWeaponDef;                            // +0xd4
    long   PrimaryMeleeWeaponDef;                       // +0xd8
    long   SecondaryMeleeWeaponDef;                     // +0xdc
    long   RangedWeaponDef;                             // +0xe0
    long   InitialSheathedWeapon;                       // +0xe4
    long   BrainDef;                                    // +0xe8
    long   CreatureInteractionType;                     // +0xec ECreatureInteractionType
    long   Property;                                    // +0xf0 EThingCreatureProperty
    float  Damage;                                      // +0xf4
    long   DamageEffectDef;                             // +0xf8
    long   StunnedParticle;                             // +0xfc
    CEngineGraphic Graphic;                             // +0x100
    CEngineGraphic EyeGraphic;                          // +0x10e
    long   DefaultOwner;                                // +0x11c
    long   ControlledMovementType;                      // +0x120 EControlledMovementType
    long   CombatTypeDef;                               // +0x124
    bool   ResetCombatTypeOnUnsheatheDefaultWeapons;    // +0x128
    long   HeroCombatDef;                               // +0x12c
    long   HeroCombatPCDef;                             // +0x130
    float  CombatDiameter;                              // +0x134
    bool   DeferHitsIfBehindHitter : 1;                 // +0x138
    long   LeaderTypeDef;                               // +0x13c
    float  RecoverEvadeHitsPercent;                     // +0x140
    long   DeadBodyDecayEffect;                         // +0x144
    bool   LeaveDeadCreature : 1;                       // +0x148 bit 0
    bool   AbleToStrafe : 1;                            // bit 1
    bool   CreateDummyOnDeath : 1;                      // bit 2
    bool   ImmuneToAttack : 1;                          // bit 3
    bool   UseMeleeWeaponAsDefault : 1;                 // bit 4
    bool   UnsheatheWeaponsAutomatically : 1;           // bit 5
    bool   IsWillable : 1;                              // bit 6
    bool   IsIndestructible : 1;                        // bit 7
    bool   IsInvulnerableDuringGenericResponse : 1;     // +0x149 bit 0
    bool   IsPushedByExplosions : 1;                    // +0x149 bit 1
    CThingCreatureDef();
    virtual void Transfer(CPersistContext& context);
};

CThingCreatureDef::CThingCreatureDef()
    : StunTimeMin(0), StunTimeMax(0), StunHealthThreshold(0), DazedTimeMin(0), DazedTimeMax(0),
      DazedHealthThreshold(0), NumSecondsToPersistOnDeath(0), MaxSlowWalkingSpeed(0), MaxWalkingSpeed(0),
      MaxJoggingSpeed(0), MaxRunningSpeed(0), MaxPushingSpeed(0), MaxFlyingSpeed(0), MaxHoverSpeedSlow(0),
      MaxHoverSpeedMedium(0), MaxHoverSpeedFast(0), SpeedRandomisationFactor(0), MaxAcceleration(0),
      HearingRadius(0), FOV(0), Sex(0), CreatureType(0),
      InitialWeaponDef(0), PrimaryMeleeWeaponDef(0), SecondaryMeleeWeaponDef(0), RangedWeaponDef(0),
      BrainDef(0), CreatureInteractionType(0), Property(0), Damage(0), DamageEffectDef(0), StunnedParticle(0),
      DefaultOwner(0), ControlledMovementType(0), CombatTypeDef(0), CombatDiameter(0), LeaderTypeDef(0)
{
    LeaveDeadCreature = false;
    AbleToStrafe = false;
    CreateDummyOnDeath = false;
    ImmuneToAttack = false;
    UseMeleeWeaponAsDefault = false;
    IsWillable = false;
    IsIndestructible = false;
    IsInvulnerableDuringGenericResponse = false;
    IsPushedByExplosions = true;
}