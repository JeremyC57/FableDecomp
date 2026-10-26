// Behaviour test for 0x00671160 (copy of the model + stand-in callee bodies).
#include <cstdio>
#include <cstring>
// CThingCreatureDef::operator=(const CThingCreatureDef&) -- memberwise def assignment: CThingBaseDef part, the creature stun/speed/sense tuning, navigator-type vector, weapon/brain defs, the two engine graphics and the creature flag bits; returns *this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00671160, length 848 bytes.
// Callees: CThingBaseDef::operator= @ 0x00463950, vector<ENavigatorType>::operator= @ 0x006714B0.
// Identity: reached through the jmp thunk 0x00671150 in slot 19 of vtable 0x0125B2DC =
// CThingCreatureDef (rtti-vtables.tsv); the manifest label "CThingBaseDef::operator=" is propagated.
// Layout: PDB CThingCreatureDef (Ego_r) starts its own members at +0x74 with a 16-byte
// NavigatorTypes vector; retail's base is 0x70 bytes and the STLport vector 12 bytes, so the
// members sit 4 bytes lower up to the vector (+0xc8) and 8 bytes lower after it -- exactly the
// offsets the body uses (Graphic +0x100, EyeGraphic +0x10e, the flag bytes +0x148/+0x149).
// CEngineGraphic is a 14-byte 2-aligned struct (PDB size 14, EyeGraphic at an odd word), so it is
// copied as one block (three dwords and a word).
// Shape: memberwise assignment in declaration order; the one-bit bool fields copy with the
// xor/and/xor merge per bit.

class CThingBaseDef { public:
    unsigned char _body[0x70];          // CDefClassBase part + the thing-base def fields
    CThingBaseDef& operator=(const CThingBaseDef& other);   // 0x00463950
};

struct CNavigatorTypeVector {           // retail std::vector<ENavigatorType> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CNavigatorTypeVector& operator=(const CNavigatorTypeVector& other);   // 0x006714B0
};

#pragma pack(push, 2)
class CEngineGraphic { public:                 // 14 bytes, 2-aligned
    unsigned long BankIndex;
    float AnimStep;
    float RenderSizeX;
    bool AdditiveAlpha;
    unsigned char Type;
};
#pragma pack(pop)

class CThingCreatureDef : public CThingBaseDef { public:                          // vtable 0x0125B2DC
    float StunTimeMin;                          // +0x70
    float StunTimeMax;                          // +0x74
    float StunHealthThreshold;                  // +0x78
    float DazedTimeMin;                         // +0x7c
    float DazedTimeMax;                         // +0x80
    long DazedHealthThreshold;                  // +0x84
    float NumSecondsToPersistOnDeath;           // +0x88
    float MaxSlowWalkingSpeed;                  // +0x8c
    float MaxWalkingSpeed;                      // +0x90
    float MaxJoggingSpeed;                      // +0x94
    float MaxRunningSpeed;                      // +0x98
    float MaxPushingSpeed;                      // +0x9c
    float MaxFlyingSpeed;                       // +0xa0
    float MaxHoverSpeedSlow;                    // +0xa4
    float MaxHoverSpeedMedium;                  // +0xa8
    float MaxHoverSpeedFast;                    // +0xac
    float SpeedRandomisationFactor;             // +0xb0
    float MaxAcceleration;                      // +0xb4
    float HearingRadius;                        // +0xb8
    float FOV;                                  // +0xbc
    long Sex;                                   // +0xc0 ESex
    long CreatureType;                          // +0xc4 ECreatureType
    CNavigatorTypeVector NavigatorTypes;        // +0xc8
    long InitialWeaponDef;                      // +0xd4
    long PrimaryMeleeWeaponDef;                 // +0xd8
    long SecondaryMeleeWeaponDef;               // +0xdc
    long RangedWeaponDef;                       // +0xe0
    long InitialSheathedWeapon;                 // +0xe4
    long BrainDef;                              // +0xe8
    long CreatureInteractionType;               // +0xec ECreatureInteractionType
    long Property;                              // +0xf0 EThingCreatureProperty
    float Damage;                               // +0xf4
    long DamageEffectDef;                       // +0xf8
    long StunnedParticle;                       // +0xfc
    CEngineGraphic Graphic;                     // +0x100
    CEngineGraphic EyeGraphic;                  // +0x10e
    long DefaultOwner;                          // +0x11c
    long ControlledMovementType;                // +0x120 EControlledMovementType
    long CombatTypeDef;                         // +0x124
    bool ResetCombatTypeOnUnsheatheDefaultWeapons;   // +0x128
    long HeroCombatDef;                         // +0x12c
    long HeroCombatPCDef;                       // +0x130
    float CombatDiameter;                       // +0x134
    bool DeferHitsIfBehindHitter : 1;           // +0x138
    long LeaderTypeDef;                         // +0x13c
    float RecoverEvadeHitsPercent;              // +0x140
    long DeadBodyDecayEffect;                   // +0x144
    bool LeaveDeadCreature : 1;                 // +0x148
    bool AbleToStrafe : 1;
    bool CreateDummyOnDeath : 1;
    bool ImmuneToAttack : 1;
    bool UseMeleeWeaponAsDefault : 1;
    bool UnsheatheWeaponsAutomatically : 1;
    bool IsWillable : 1;
    bool IsIndestructible : 1;
    bool IsInvulnerableDuringGenericResponse : 1;   // +0x149
    bool IsPushedByExplosions : 1;
    CThingCreatureDef& operator=(const CThingCreatureDef& other);
};

CThingCreatureDef& CThingCreatureDef::operator=(const CThingCreatureDef& other)
{
    CThingBaseDef::operator=(other);
    StunTimeMin = other.StunTimeMin;
    StunTimeMax = other.StunTimeMax;
    StunHealthThreshold = other.StunHealthThreshold;
    DazedTimeMin = other.DazedTimeMin;
    DazedTimeMax = other.DazedTimeMax;
    DazedHealthThreshold = other.DazedHealthThreshold;
    NumSecondsToPersistOnDeath = other.NumSecondsToPersistOnDeath;
    MaxSlowWalkingSpeed = other.MaxSlowWalkingSpeed;
    MaxWalkingSpeed = other.MaxWalkingSpeed;
    MaxJoggingSpeed = other.MaxJoggingSpeed;
    MaxRunningSpeed = other.MaxRunningSpeed;
    MaxPushingSpeed = other.MaxPushingSpeed;
    MaxFlyingSpeed = other.MaxFlyingSpeed;
    MaxHoverSpeedSlow = other.MaxHoverSpeedSlow;
    MaxHoverSpeedMedium = other.MaxHoverSpeedMedium;
    MaxHoverSpeedFast = other.MaxHoverSpeedFast;
    SpeedRandomisationFactor = other.SpeedRandomisationFactor;
    MaxAcceleration = other.MaxAcceleration;
    HearingRadius = other.HearingRadius;
    FOV = other.FOV;
    Sex = other.Sex;
    CreatureType = other.CreatureType;
    NavigatorTypes = other.NavigatorTypes;
    InitialWeaponDef = other.InitialWeaponDef;
    PrimaryMeleeWeaponDef = other.PrimaryMeleeWeaponDef;
    SecondaryMeleeWeaponDef = other.SecondaryMeleeWeaponDef;
    RangedWeaponDef = other.RangedWeaponDef;
    InitialSheathedWeapon = other.InitialSheathedWeapon;
    BrainDef = other.BrainDef;
    CreatureInteractionType = other.CreatureInteractionType;
    Property = other.Property;
    Damage = other.Damage;
    DamageEffectDef = other.DamageEffectDef;
    StunnedParticle = other.StunnedParticle;
    Graphic = other.Graphic;
    EyeGraphic = other.EyeGraphic;
    DefaultOwner = other.DefaultOwner;
    ControlledMovementType = other.ControlledMovementType;
    CombatTypeDef = other.CombatTypeDef;
    ResetCombatTypeOnUnsheatheDefaultWeapons = other.ResetCombatTypeOnUnsheatheDefaultWeapons;
    HeroCombatDef = other.HeroCombatDef;
    HeroCombatPCDef = other.HeroCombatPCDef;
    CombatDiameter = other.CombatDiameter;
    DeferHitsIfBehindHitter = other.DeferHitsIfBehindHitter;
    LeaderTypeDef = other.LeaderTypeDef;
    RecoverEvadeHitsPercent = other.RecoverEvadeHitsPercent;
    DeadBodyDecayEffect = other.DeadBodyDecayEffect;
    LeaveDeadCreature = other.LeaveDeadCreature;
    AbleToStrafe = other.AbleToStrafe;
    CreateDummyOnDeath = other.CreateDummyOnDeath;
    ImmuneToAttack = other.ImmuneToAttack;
    UseMeleeWeaponAsDefault = other.UseMeleeWeaponAsDefault;
    UnsheatheWeaponsAutomatically = other.UnsheatheWeaponsAutomatically;
    IsWillable = other.IsWillable;
    IsIndestructible = other.IsIndestructible;
    IsInvulnerableDuringGenericResponse = other.IsInvulnerableDuringGenericResponse;
    IsPushedByExplosions = other.IsPushedByExplosions;
    return *this;
}

CThingBaseDef& CThingBaseDef::operator=(const CThingBaseDef& other) { std::memcpy(_body, other._body, sizeof(_body)); return *this; }
CNavigatorTypeVector& CNavigatorTypeVector::operator=(const CNavigatorTypeVector& other) { m_first = other.m_first; m_last = other.m_last; m_end = other.m_end; return *this; }

int main()
{
    static CThingCreatureDef a, b;
    b._body[5] = 9; b.StunTimeMin = 0.5f; b.FOV = 1.25f; b.NavigatorTypes.m_last = &b;
    b.Graphic.BankIndex = 77; b.EyeGraphic.Type = 3; b.EyeGraphic.RenderSizeX = 2.0f;
    b.ResetCombatTypeOnUnsheatheDefaultWeapons = true; b.DeferHitsIfBehindHitter = true;
    b.IsWillable = true; b.IsPushedByExplosions = true; b.DeadBodyDecayEffect = -7;
    a.AbleToStrafe = true;
    CThingCreatureDef* r = &(a = b);
    if (r != &a || a._body[5] != 9 || a.StunTimeMin != 0.5f || a.FOV != 1.25f || a.NavigatorTypes.m_last != &b
        || a.Graphic.BankIndex != 77 || a.EyeGraphic.Type != 3 || a.EyeGraphic.RenderSizeX != 2.0f
        || !a.ResetCombatTypeOnUnsheatheDefaultWeapons || !a.DeferHitsIfBehindHitter || !a.IsWillable
        || !a.IsPushedByExplosions || a.AbleToStrafe || a.DeadBodyDecayEffect != -7) { std::printf("00671160_TEST FAIL\n"); return 1; }
    std::printf("00671160_TEST PASS\n");
    return 0;
}