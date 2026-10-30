#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

struct CMemberDef {
    void Copy_00461bb7(const CMemberDef* other);
};

class CCombatAbilityFlourishCounterAttackDef { public:
    unsigned char _base[0x28];
    CMemberDef m_member; // at +0x28
    unsigned char _tail[0x18 - sizeof(CMemberDef)];

    void Copy(const CCombatAbilityFlourishCounterAttackDef* other);
};

void CCombatAbilityFlourishCounterAttackDef::Copy(
        const CCombatAbilityFlourishCounterAttackDef* other)
{
    ((CDefClassBase*)this)->operator=(*(const CDefClassBase*)other);
    m_member.Copy_00461bb7(&other->m_member);
}