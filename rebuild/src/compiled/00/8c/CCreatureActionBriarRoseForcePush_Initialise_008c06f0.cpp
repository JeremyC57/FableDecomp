// CCreatureAction_BriarRoseForcePush::Initialise() -- names this action's animation "ST_FORCE_PUSH_INTO" then runs the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C06F0, oracle length 51 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; push esi ; push -1 ; mov esi,ecx ; push offset "ST_FORCE_PUSH_INTO" ; lea ecx,[esp+0Ch] ; call CCharString::CCharString(const char*,long) ; lea eax,[esp+4] ; push eax ; mov ecx,esi ; call SetAnimationName ; lea ecx,[esp+4] ; call CCharString::~CCharString ; mov ecx,esi ; call CCombatActionBase::Initialise ; pop esi ; pop ecx ; ret
// Virtual Initialise override: builds a scoped CCharString from the literal (ctor
// @ 0x0099EBF0, dtor @ 0x0099EAE0, both from fable_string.h), hands it to the base-class
// setter @ 0x008574D0 (named SetAnimationName here because every literal of the
// 63-body family is an animation name; the manifest label of that address is
// propagated and untrusted), destroys the temporary and finally calls the base
// CCombatActionBase::Initialise @ 0x00857520 directly (non-virtual call).
#include "fable_string.h"

class CCombatActionBase { public:
    virtual void v00();
    virtual void Initialise();                        // 0x00857520
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_BriarRoseForcePush : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_BriarRoseForcePush::Initialise()
{
    {
        CCharString name("ST_FORCE_PUSH_INTO", -1);       // a named local: a bare temporary would reuse the ctor's eax
        SetCombatAnimation(name);            // instead of re-deriving `lea eax,[esp+4]` as retail does
    }
    CCombatActionBase::Initialise();
}