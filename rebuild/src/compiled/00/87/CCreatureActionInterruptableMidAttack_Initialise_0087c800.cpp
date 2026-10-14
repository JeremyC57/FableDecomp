// CCreatureAction_InterruptableMidAttack::Initialise() -- names the animation "STRIKE_MEDIUM_FRONT", runs the base Initialise, then marks InterruptTime as unset (-999)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087C800, length 64 bytes. Vtable slot 12.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520.
// Shape: the landed Initialise family followed by one immediate store. InterruptTime is the engine-header name (PDB +0x12c;
// retail CCombatActionBase is 0x114 bytes, so it is at +0x114 -- modelled locally).
#include "fable_string.h"

struct CCombatActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00857520
    void SetAnimationName(const CCharString& name);   // 0x008574D0 (propagated manifest label; named as in the landed family)
    unsigned char _body[0x114 - 4];
};

struct CCreatureAction_InterruptableMidAttack : CCombatActionBase {
    long InterruptTime;                     // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_InterruptableMidAttack::Initialise()
{
    {
        CCharString name("STRIKE_MEDIUM_FRONT", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    InterruptTime = -999;
}