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

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

struct CLongVector {                    // std::vector<long> (retail STLport, 12 bytes)
    long* m_pFirst;
    long* m_pLast;
    long* m_pEndOfStorage;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

// def vtable 0x0124193C: slot 0 0x004E0FB2 (implicit inline dtor); real overrides from the def vtable scan
class CTurncoatDef : public CDefClassBase { public:
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
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004E7F24
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E2CDF
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004DEBC0
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