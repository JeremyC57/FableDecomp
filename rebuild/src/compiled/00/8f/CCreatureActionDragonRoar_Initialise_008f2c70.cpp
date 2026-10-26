// CCreatureAction_DragonRoar::Initialise() -- names the animation "ROAR", runs the base Initialise, then sets ActionTime to 30% of TotalTime
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F2C70, length 68 bytes. Vtable slot 12 of CCreatureAction_DragonRoar (0x012937EC).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520, __ftol2 @ 0x00BFEA70.
// Shape: the landed Initialise family (scoped named CCharString, base call) followed by `fild [TotalTime] ; fmul [0.3f]`
// (the member loaded first, then the literal from the float pool @ 0x01238170) truncated to a long. TotalTime (+0x10) and
// ActionTime (+0x14) are the engine-header CCreatureActionBase names.
#include "fable_string.h"

class CCombatActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00857520
    void SetAnimationName(const CCharString& name);   // 0x008574D0 (propagated manifest label; named as in the landed family)
    unsigned char _pad04[0x0c];             // +0x04
    long TotalTime;                         // +0x10
    long ActionTime;                        // +0x14
};

class CCreatureAction_DragonRoar : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_DragonRoar::Initialise()
{
    {
        CCharString name("ROAR", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    ActionTime = (long)(TotalTime * 0.3f);
}