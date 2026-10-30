#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CCombatAbilityFlourishCounterAttackDef.h"  // retyped onto the PDB layout; byte parity re-verified
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

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
