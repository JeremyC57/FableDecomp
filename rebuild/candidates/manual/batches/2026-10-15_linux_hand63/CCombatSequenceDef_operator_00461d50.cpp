// CCombatSequenceDef::operator=(const CCombatSequenceDef&) -- memberwise assignment: base CParentDefClassBase::operator=, six enum/long words, the ActionList vector by its operator=, the prediction time and the PreferIfValid flag; returns *this
// Leaf: ??4CCombatSequenceDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00461D50, length 81 bytes.
// Pragma: #pragma optimize("s", on) (one register, eax, for every copy pair).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0,
//          vector<EActionRegisteredType>::operator= @ 0x00461DA1.
// Identity: the manifest label "CParentDefClassBase::operator=" is propagated. The body is reached
// through a `jmp` thunk at 0x00461D4B in slot 19 of vtable 0x01234BBC = CCombatSequenceDef
// (rtti-vtables.tsv); the touched offsets are the PDB CCombatSequenceDef members 4 bytes lower
// (retail CParentDefClassBase is 0x3c, PDB 0x40) with a 12-byte retail vector (PDB 16):
// ValidCombatZones +0x3c, Type +0x40, InterruptionType +0x44, IsValidType +0x48,
// OnStartType +0x4c, OnStopType +0x50, ActionList +0x54, SecondsToPredictTargetPosition +0x60,
// PreferIfValid +0x64.
// Shape: compiler-generated memberwise assignment in declaration order; the enums are 4-byte
// values and the float is copied with an integer load/store pair (plain assignment, no FPU).
// The PDB header (engine/CParentDefClassBase.h) describes the Ego layout, so the retail base is
// modelled locally.
#pragma optimize("s", on)   // the def module is size-optimised: every copy pair goes through eax

struct CParentDefClassBase {
    virtual void v00();
    unsigned char _body[0x38];                                            // +0x04..+0x3c
    CParentDefClassBase& operator=(const CParentDefClassBase& other);     // 0x00431ED0
};

struct CActionRegisteredTypeVector {                                      // retail std::vector (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CActionRegisteredTypeVector& operator=(const CActionRegisteredTypeVector& other);   // 0x00461DA1
};

struct CCombatSequenceDef : CParentDefClassBase {                         // vtable 0x01234BBC
    long                        ValidCombatZones;                         // +0x3c
    long                        Type;                                     // +0x40 ECombatSequenceType
    long                        InterruptionType;                         // +0x44 ECombatSequenceInterruptionType
    long                        IsValidType;                              // +0x48 ECombatSequenceIsValidType
    long                        OnStartType;                              // +0x4c ECombatSequenceOnStartModuleType
    long                        OnStopType;                               // +0x50 ECombatSequenceOnStopModuleType
    CActionRegisteredTypeVector ActionList;                               // +0x54
    float                       SecondsToPredictTargetPosition;           // +0x60
    bool                        PreferIfValid;                            // +0x64
    virtual void v00();
    CCombatSequenceDef& operator=(const CCombatSequenceDef& other);
};

CCombatSequenceDef& CCombatSequenceDef::operator=(const CCombatSequenceDef& other)
{
    CParentDefClassBase::operator=(other);
    ValidCombatZones = other.ValidCombatZones;
    Type = other.Type;
    InterruptionType = other.InterruptionType;
    IsValidType = other.IsValidType;
    OnStartType = other.OnStartType;
    OnStopType = other.OnStopType;
    ActionList = other.ActionList;
    SecondsToPredictTargetPosition = other.SecondsToPredictTargetPosition;
    PreferIfValid = other.PreferIfValid;
    return *this;
}
