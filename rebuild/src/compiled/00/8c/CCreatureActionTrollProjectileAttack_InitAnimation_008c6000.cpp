// CCreatureAction_TrollProjectileAttack::InitAnimation() -- names the animation from the custom name at +0x118 when the flag at +0x114 is set, else "PROJECTILE_ATTACK"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C6000, length 80 bytes.
// Callees: CCombatActionBase::SetAnimationName @ 0x008574D0 (twice; manifest label "CTCInGameMenu::SetPDef" is propagated), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the taken branch passes the member by address with ecx still holding `this` from entry; the other builds the scoped literal name in the
// one-word frame (the landed 0x008CC760 family) and destroys it.
#include "fable_string.h"

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_TrollProjectileAttack : public CCombatActionBase { public:
    unsigned char m_useCustomAnimation;     // +0x114
    CCharString m_customAnimation;          // +0x118
    void InitAnimation();
};

void CCreatureAction_TrollProjectileAttack::InitAnimation()
{
    if (m_useCustomAnimation) {
        SetAnimationName(m_customAnimation);
    } else {
        CCharString name("PROJECTILE_ATTACK", -1);
        SetAnimationName(name);
    }
}