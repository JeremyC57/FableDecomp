// CTurncoatDef::operator=(const CTurncoatDef&) -- def copy: CDefClassBase part, turncoat flag, effects, resistance scalars, stage-effects vector
// Leaf: ??4CTurncoatDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2CE4, 87 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::vector<long>::operator= @ 0x00454886.
// Identification: PDB CTurncoatDef = CDefClassBase + Turncoatable (bool) +0x25, TurncoatedEffect, TurncoatActivateEffect,
//   TurncoatDeactivateEffect +0x28..+0x30, MinimumResistance, TimeDelayBeforeResistanceRecoverySecs, ResistanceRecoveryPerSec
//   (floats) +0x34..+0x3c, TurncoatStageEffects (std::vector<long>) +0x40, PercentageOfHealthAsResistance, ExtraResistance +0x4c/+0x50.
// Shape: memberwise; the vector goes through the out-of-line vector<long>::operator= 0x00454886.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

struct CDefClassBase {
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CLongVector {                    // std::vector<long> (retail STLport, 12 bytes)
    long* m_pFirst;
    long* m_pLast;
    long* m_pEndOfStorage;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CTurncoatDef : CDefClassBase {
    bool Turncoatable;                  // +0x25
    long TurncoatedEffect;              // +0x28
    long TurncoatActivateEffect;        // +0x2c
    long TurncoatDeactivateEffect;      // +0x30
    float MinimumResistance;            // +0x34
    float TimeDelayBeforeResistanceRecoverySecs;// +0x38
    float ResistanceRecoveryPerSec;     // +0x3c
    CLongVector TurncoatStageEffects;   // +0x40
    float PercentageOfHealthAsResistance;// +0x4c
    float ExtraResistance;              // +0x50
    CTurncoatDef& operator=(const CTurncoatDef& other);
};

CTurncoatDef& CTurncoatDef::operator=(const CTurncoatDef& other)
{
    CDefClassBase::operator=(other);
    Turncoatable = other.Turncoatable;
    TurncoatedEffect = other.TurncoatedEffect;
    TurncoatActivateEffect = other.TurncoatActivateEffect;
    TurncoatDeactivateEffect = other.TurncoatDeactivateEffect;
    MinimumResistance = other.MinimumResistance;
    TimeDelayBeforeResistanceRecoverySecs = other.TimeDelayBeforeResistanceRecoverySecs;
    ResistanceRecoveryPerSec = other.ResistanceRecoveryPerSec;
    TurncoatStageEffects = other.TurncoatStageEffects;
    PercentageOfHealthAsResistance = other.PercentageOfHealthAsResistance;
    ExtraResistance = other.ExtraResistance;
    return *this;
}
