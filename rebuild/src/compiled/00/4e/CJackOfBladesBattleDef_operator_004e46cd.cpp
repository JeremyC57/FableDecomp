// CJackOfBladesBattleDef::operator=(const CJackOfBladesBattleDef&) -- memberwise copy: CParentDefClassBase part, then the seventeen battle tuning scalars; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E46CD, length 123 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every scalar goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0.
// Identity: the catalog label "CParentDefClassBase::operator=_13fields" is propagated. Slot 19 (Copy) of
// CJackOfBladesBattleDef's vtable 0x01242E3C is the 5-byte `jmp 0x004E46CD` right before this body. The PDB
// members sit 4 bytes lower in retail (12-byte retail SubDefInfoMap): IdealShootDistance +0x3c ..
// RushAwayDistance +0x7c (17 dwords, retail size 0x80).
// Shape: compiler-style memberwise assignment -- base part out of line, scalars through eax in order.
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes); the retail base is 0x39 bytes,
// so it is modelled locally as a packed polymorphic base.
#pragma pack(push, 1)
class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

class CJackOfBladesBattleDef : public CParentDefClassBase { public:  // vtable 0x01242E3C
    float IdealShootDistance;               // +0x3c
    long  NumGetHitsBeforeRush;             // +0x40
    float TimeBetweenShots;                 // +0x44
    float ScreamInAirTime;                  // +0x48
    float TimeBetweenPillars;               // +0x4c
    float BattleFloatHeight;                // +0x50
    float BattleScale;                      // +0x54
    float ScaleUpTime;                      // +0x58
    float ScaleDownTime;                    // +0x5c
    float DeathScreamTime;                  // +0x60
    float DeathHeight;                      // +0x64
    float BurnDamage;                       // +0x68
    float FlashDuration;                    // +0x6c
    float FlashBuildUpTime;                 // +0x70
    float RushTriggerDistance;              // +0x74
    float RushToDistance;                   // +0x78
    float RushAwayDistance;                 // +0x7c
    CJackOfBladesBattleDef& operator=(const CJackOfBladesBattleDef& other);
};

CJackOfBladesBattleDef& CJackOfBladesBattleDef::operator=(const CJackOfBladesBattleDef& other)
{
    CParentDefClassBase::operator=(other);
    IdealShootDistance = other.IdealShootDistance;
    NumGetHitsBeforeRush = other.NumGetHitsBeforeRush;
    TimeBetweenShots = other.TimeBetweenShots;
    ScreamInAirTime = other.ScreamInAirTime;
    TimeBetweenPillars = other.TimeBetweenPillars;
    BattleFloatHeight = other.BattleFloatHeight;
    BattleScale = other.BattleScale;
    ScaleUpTime = other.ScaleUpTime;
    ScaleDownTime = other.ScaleDownTime;
    DeathScreamTime = other.DeathScreamTime;
    DeathHeight = other.DeathHeight;
    BurnDamage = other.BurnDamage;
    FlashDuration = other.FlashDuration;
    FlashBuildUpTime = other.FlashBuildUpTime;
    RushTriggerDistance = other.RushTriggerDistance;
    RushToDistance = other.RushToDistance;
    RushAwayDistance = other.RushAwayDistance;
    return *this;
}