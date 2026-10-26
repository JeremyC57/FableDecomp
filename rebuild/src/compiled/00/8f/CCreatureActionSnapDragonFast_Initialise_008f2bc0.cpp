// CCreatureAction_SnapDragonFast::Initialise() -- names the animation "SNAP_ATTACK_FAST", runs the base Initialise, then sets the hit frame at +0x14 to a quarter of the duration at +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F2BC0, length 68 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetCombatAnimation @ 0x008574D0 (manifest label propagated),
// CCharString::~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520 (non-virtual call), __ftol2 @ 0x00BFEA70 (the float-to-long conversion).
// Shape: the landed "scoped name + base Initialise" family (0x008CC760) followed by `fild [this+10h] ; fmul [__real@0.25] ; call __ftol2`: a long member
// multiplied by the literal 0.25f (the literal is loaded second, GOTCHAS) and truncated back into a long member.
#include "fable_string.h"

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[0x0c];             // +0x04
    long m_duration;                        // +0x10 (CCreatureActionBase: the action's length in frames)
    long m_hitFrame;                        // +0x14
    unsigned char _pad18[0x114 - 0x18];
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_SnapDragonFast : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_SnapDragonFast::Initialise()
{
    {
        CCharString name("SNAP_ATTACK_FAST", -1);
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    m_hitFrame = (long)(m_duration * 0.25f);
}