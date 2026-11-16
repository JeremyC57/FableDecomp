// CTCCoopSpirit::ResetScore() -- Score (+0x10) = 0
// Leaf: ?ResetScore@CTCCoopSpirit@@QAEXXZ
// Name: Ego_r.pdb (and FableWin.pdb) @CTCCoopSpirit@@ set, in order AddExperience 0x00677210, AddScore 0x00677220, GetScore 0x00677230,
//   ResetScore 0x00677240, GetSpiritDef 0x00677250 (private), .. GetMeleeTargetRange 0x00677730; retail 0x0066FB10 / 0x0066FB30 / 0x0066FB40 run
//   in the same order. NF33 2026-11-16.
//   was the extern "C" ClearField10_0066fb30 stand-in; no landed binder.
// Layout: Ego_r.pdb CTCCoopSpirit (NoMovementTimer +0x0C, Score +0x10, OffscreenCounter +0x14, MovementVector +0x18, Master +0x24,
//   PCoopSpiritDef +0x28 (CDefPointer<const CCoopSpiritDef>, one pointer word in retail), PParticleEmitter +0x2C); CCoopSpiritDef
//   MeleeTargetRange +0x38. No shared header for either class; modelled here (CTCBase words opaque).
class CCoopSpiritDef { public:
    unsigned char _pad00[0x38];
    float MeleeTargetRange;                     // +0x38
};
class CTCCoopSpirit {
public:
    unsigned char _pad00[0x10];                 // CTCBase (+0x00..+0x0B), NoMovementTimer +0x0C
    long Score;                                 // +0x10
    unsigned char _pad14[0x28 - 0x14];          // OffscreenCounter, MovementVector, Master
    const CCoopSpiritDef* PCoopSpiritDef;       // +0x28 CDefPointer<const CCoopSpiritDef>
    void ResetScore();
};

void CTCCoopSpirit::ResetScore()
{
    Score = 0;
}
