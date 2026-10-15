#include <cstdio>
// CTurncoatDef::operator=(const CTurncoatDef&) -- def copy: CDefClassBase part, turncoat flag, effects, resistance scalars, stage-effects vector
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2CE4, 87 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::vector<long>::operator= @ 0x00454886.
// Identification: PDB CTurncoatDef = CDefClassBase + Turncoatable (bool) +0x25, TurncoatedEffect, TurncoatActivateEffect,
//   TurncoatDeactivateEffect +0x28..+0x30, MinimumResistance, TimeDelayBeforeResistanceRecoverySecs, ResistanceRecoveryPerSec
//   (floats) +0x34..+0x3c, TurncoatStageEffects (std::vector<long>) +0x40, PercentageOfHealthAsResistance, ExtraResistance +0x4c/+0x50.
// Shape: memberwise; the vector goes through the out-of-line vector<long>::operator= 0x00454886.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.

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


CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { for (int i = 4; i < 0x25; ++i) _body[i] = other._body[i]; return *this; }
CLongVector& CLongVector::operator=(const CLongVector& other) { m_pFirst = other.m_pFirst; m_pLast = other.m_pLast; m_pEndOfStorage = other.m_pEndOfStorage; return *this; }

int main()
{
    static CTurncoatDef a, b;
    for (int i = 4; i < 0x25; ++i) { b._body[i] = (unsigned char)(i * 3); a._body[i] = 0; }
    b.Turncoatable = true;
    b.TurncoatedEffect = 1;
    b.TurncoatActivateEffect = 2;
    b.TurncoatDeactivateEffect = 3;
    b.MinimumResistance = 0.25f;
    b.TimeDelayBeforeResistanceRecoverySecs = 5.0f;
    b.ResistanceRecoveryPerSec = 0.5f;
    b.TurncoatStageEffects.m_pFirst = (long*)&b;
    b.PercentageOfHealthAsResistance = 0.3f;
    b.ExtraResistance = 12.0f;
    CTurncoatDef* r = &(a = b);
    if (r != &a) { std::printf("004e2ce4_TEST FAIL\n"); return 1; }
    for (int j = 4; j < 0x25; ++j) if (a._body[j] != (unsigned char)(j * 3)) { std::printf("%s_TEST FAIL\n", "004e2ce4"); return 1; }
    if (!(a.Turncoatable == b.Turncoatable)) { std::printf("004e2ce4_TEST FAIL\n"); return 1; }
    if (!(a.TurncoatedEffect == b.TurncoatedEffect)) { std::printf("004e2ce4_TEST FAIL\n"); return 1; }
    if (!(a.TurncoatActivateEffect == b.TurncoatActivateEffect)) { std::printf("004e2ce4_TEST FAIL\n"); return 1; }
    if (!(a.TurncoatDeactivateEffect == b.TurncoatDeactivateEffect)) { std::printf("004e2ce4_TEST FAIL\n"); return 1; }
    if (!(a.MinimumResistance == b.MinimumResistance)) { std::printf("004e2ce4_TEST FAIL\n"); return 1; }
    if (!(a.TimeDelayBeforeResistanceRecoverySecs == b.TimeDelayBeforeResistanceRecoverySecs)) { std::printf("004e2ce4_TEST FAIL\n"); return 1; }
    if (!(a.ResistanceRecoveryPerSec == b.ResistanceRecoveryPerSec)) { std::printf("004e2ce4_TEST FAIL\n"); return 1; }
    if (!(a.TurncoatStageEffects.m_pFirst == b.TurncoatStageEffects.m_pFirst)) { std::printf("004e2ce4_TEST FAIL\n"); return 1; }
    if (!(a.PercentageOfHealthAsResistance == b.PercentageOfHealthAsResistance)) { std::printf("004e2ce4_TEST FAIL\n"); return 1; }
    if (!(a.ExtraResistance == b.ExtraResistance)) { std::printf("004e2ce4_TEST FAIL\n"); return 1; }
    std::printf("004e2ce4_TEST PASS\n");
    return 0;
}