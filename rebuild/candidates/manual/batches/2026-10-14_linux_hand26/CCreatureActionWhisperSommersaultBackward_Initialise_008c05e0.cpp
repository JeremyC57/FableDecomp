// CCreatureAction_WhisperSommersaultBackward::Initialise() -- names the animation "MOVEMENT_LONG_BACK", clears the flag at +0x114, then runs the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C05E0, length 64 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0 (manifest label
// "CTCInGameMenu::SetPDef" is propagated; the landed 63-body family names it so), CCharString::~CCharString @ 0x0099EAE0,
// CCombatActionBase::Initialise @ 0x00857520 (non-virtual call).
// Shape: the landed "scoped name + base Initialise" family (0x008CC760) with one byte store scheduled between `mov ecx,esi` and the base call.
#include "fable_string.h"

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _body[0x114 - 4];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_WhisperSommersaultBackward : CCombatActionBase {
    unsigned char m_hasLanded;              // +0x114
    virtual void Initialise();
};

void CCreatureAction_WhisperSommersaultBackward::Initialise()
{
    {
        CCharString name("MOVEMENT_LONG_BACK", -1);   // a named local: a bare temporary would reuse the ctor's eax
        SetAnimationName(name);
    }
    m_hasLanded = 0;
    CCombatActionBase::Initialise();
}
