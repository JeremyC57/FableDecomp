// Behaviour test for 0x00670000 (NF33 2026-11-16): links the row; same class model as the row.
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
    float GetMeleeTargetRange() const;
};

int main()
{
    int ok = 1;
    CCoopSpiritDef d; d.MeleeTargetRange = 3.5f; CTCCoopSpirit s; memset(&s, 0, sizeof(s)); s.PCoopSpiritDef = &d;
    ok &= s.GetMeleeTargetRange() == 3.5f;
    d.MeleeTargetRange = -1.25f; ok &= s.GetMeleeTargetRange() == -1.25f;
    if (!ok) { puts("FAIL"); return 1; }
    puts("CTCPhysicsBase_00670000_TEST PASS");   // catalog PassPattern (the row is now CTCCoopSpirit::GetMeleeTargetRange)
    return 0;
}
