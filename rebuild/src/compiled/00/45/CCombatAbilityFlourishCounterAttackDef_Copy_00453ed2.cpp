#include "engine/CCombatAbilityFlourishCounterAttackDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CMemberDef {
    void Copy(const CMemberDef* other);
};

struct CCombatAbilityFlourishCounterAttackDef_Methods : CCombatAbilityFlourishCounterAttackDef {
    void Copy(const CCombatAbilityFlourishCounterAttackDef* other);
};

void CCombatAbilityFlourishCounterAttackDef_Methods::Copy(
        const CCombatAbilityFlourishCounterAttackDef* other)
{
    ((CDefClassBase*)this)->operator=(*(const CDefClassBase*)other);
    // header types the +0x28 sub-object as `long m_member`; the retail code copies it via
    // the member class's Copy (ecx = this+0x28), so view the slot through that type here.
    ((CMemberDef*)&m_member)->Copy((const CMemberDef*)&other->m_member);
}
