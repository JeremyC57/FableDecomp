// Behaviour test for 0x0066FB10 (NF33 2026-11-16): links the row; same class model as the row.
#include <stdio.h>
#include <string.h>
// Layout: Ego_r.pdb CTCCoopSpirit (NoMovementTimer +0x0C, Score +0x10, OffscreenCounter +0x14, MovementVector +0x18, Master +0x24,
//   PCoopSpiritDef +0x28 (CDefPointer<const CCoopSpiritDef>, one pointer word in retail), PParticleEmitter +0x2C); CCoopSpiritDef
//   MeleeTargetRange +0x38. No shared header for either class; modelled here (CTCBase words opaque).
class CCoopSpiritDef { public:
    unsigned char _pad00[0x38];
    float MeleeTargetRange;                     // +0x38
};
class CTCCoopSpirit {
    friend int main();
public:
    unsigned char _pad00[0x10];                 // CTCBase (+0x00..+0x0B), NoMovementTimer +0x0C
    long Score;                                 // +0x10
    unsigned char _pad14[0x28 - 0x14];          // OffscreenCounter, MovementVector, Master
    const CCoopSpiritDef* PCoopSpiritDef;       // +0x28 CDefPointer<const CCoopSpiritDef>
    void AddScore(long amount);
};

int main()
{
    int ok = 1;
    CTCCoopSpirit s; memset(&s, 0xa5, sizeof(s)); s.Score = -20;
    s.AddScore(70); ok &= s.Score == 50;
    s.AddScore(-75); ok &= s.Score == -25;
    s.AddScore(0); ok &= s.Score == -25;
    ok &= s._pad00[0] == 0xa5 && s._pad00[0x0f] == 0xa5 && s._pad14[0] == 0xa5;
    if (!ok) { puts("FAIL"); return 1; }
    puts("PASS CTCCoopSpirit::AddExperience 0x0066FB10");   // catalog PassPattern (the row is now AddScore)
    return 0;
}
