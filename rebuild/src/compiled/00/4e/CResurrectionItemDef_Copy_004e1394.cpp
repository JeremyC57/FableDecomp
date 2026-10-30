#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResurrectionItemDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CSubField { int v; };
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

void __fastcall CSpecialAbilitiesDrainLifeDataDef_Copy(CResurrectionItemDef* self, int _edx, const CResurrectionItemDef* other)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)other);
    self->OnUseParticleEffect = other->OnUseParticleEffect;
}