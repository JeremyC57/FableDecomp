// CExplosionDef::operator=(const CExplosionDef&) -- def copy: CDefClassBase part, then the CExplosionDef fields memberwise in declaration order
// Leaf: ??4CExplosionDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2ECF, 135 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x004E2ECA in slot 19 of vtable 0x012425F4 = CExplosionDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10, vector<CExplosionRing>::operator= @ 0x004E2F56.
// Layout (engine/CExplosionDef.h, PDB minus 4 after the 12-byte retail vector): Damage +0x28, ExplosionRadius +0x2c, FireDamage +0x30, ExplosionLifeTime +0x34, DamageFalloffWithRadius +0x38, ExplosionBlendSmall +0x3c, ExplosionBlendLarge +0x40, ExplosionReplacementBlendSmall +0x44, ExplosionReplacementBlendLarge +0x48, SecondsBeforeEffectReplacement +0x4c, ExplosionRadiusSmall +0x50, ExplosionRadiusLarge +0x54, ExplosionRings +0x58, ExplosionShouldKnockdown +0x64, CauseFireEffectOnHitCreatures +0x65, CausesRumble +0x66, RumbleStrength +0x68, RumbleLength +0x6c.
// Shape: the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
// RumbleStrength/RumbleLength are the 4-byte EQuakeStrength/EQuakeLength enums (long).
#pragma optimize("s", on)

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
struct CExplosionRingVector {                       // retail std::vector<CExplosionRing> (retail 0xc bytes)
    unsigned long m_words[3];
    CExplosionRingVector& operator=(const CExplosionRingVector& other);   // 0x004E2F56
};

struct CExplosionDef : CDefClassBase {
    float                    Damage;                      // +0x28
    float                    ExplosionRadius;             // +0x2c
    long                     FireDamage;                  // +0x30
    float                    ExplosionLifeTime;           // +0x34
    bool                     DamageFalloffWithRadius;     // +0x38
    long                     ExplosionBlendSmall;         // +0x3c
    long                     ExplosionBlendLarge;         // +0x40
    long                     ExplosionReplacementBlendSmall; // +0x44
    long                     ExplosionReplacementBlendLarge; // +0x48
    float                    SecondsBeforeEffectReplacement; // +0x4c
    float                    ExplosionRadiusSmall;        // +0x50
    float                    ExplosionRadiusLarge;        // +0x54
    CExplosionRingVector     ExplosionRings;              // +0x58
    bool                     ExplosionShouldKnockdown;    // +0x64
    bool                     CauseFireEffectOnHitCreatures; // +0x65
    bool                     CausesRumble;                // +0x66
    long                     RumbleStrength;              // +0x68
    long                     RumbleLength;                // +0x6c
    CExplosionDef& operator=(const CExplosionDef& other);
};

CExplosionDef& CExplosionDef::operator=(const CExplosionDef& other)
{
    CDefClassBase::operator=(other);
    Damage = other.Damage;
    ExplosionRadius = other.ExplosionRadius;
    FireDamage = other.FireDamage;
    ExplosionLifeTime = other.ExplosionLifeTime;
    DamageFalloffWithRadius = other.DamageFalloffWithRadius;
    ExplosionBlendSmall = other.ExplosionBlendSmall;
    ExplosionBlendLarge = other.ExplosionBlendLarge;
    ExplosionReplacementBlendSmall = other.ExplosionReplacementBlendSmall;
    ExplosionReplacementBlendLarge = other.ExplosionReplacementBlendLarge;
    SecondsBeforeEffectReplacement = other.SecondsBeforeEffectReplacement;
    ExplosionRadiusSmall = other.ExplosionRadiusSmall;
    ExplosionRadiusLarge = other.ExplosionRadiusLarge;
    ExplosionRings = other.ExplosionRings;
    ExplosionShouldKnockdown = other.ExplosionShouldKnockdown;
    CauseFireEffectOnHitCreatures = other.CauseFireEffectOnHitCreatures;
    CausesRumble = other.CausesRumble;
    RumbleStrength = other.RumbleStrength;
    RumbleLength = other.RumbleLength;
    return *this;
}