#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CCombatAbilityBlockCounterAttackDef.h"  // retyped onto the PDB layout; byte parity re-verified

// Base-class and +0x28 sub-object types are not expressed by the generated header
// (its bytes are flattened into _base_0x04), so they stay as local call-shape models.
struct CMember { void Copy(const CMember* src); };
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

void __fastcall CCombatAbilityBlockCounterAttackDef_Copy(CCombatAbilityBlockCounterAttackDef* self, void* edx, const CCombatAbilityBlockCounterAttackDef* src)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)src);
    CMember* m = (CMember*)(self->_base_0x04 + 0x24);
    m->Copy((const CMember*)(src->_base_0x04 + 0x24));
}
