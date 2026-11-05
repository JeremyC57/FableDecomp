// Leaf: ?CSpecialAbilitiesForcePushDataDef_Copy@@YIXPAVCSpecialAbilitiesDrainLifeDataDef@@HPBV1@@Z
struct CSubField { int v; };
class CSpecialAbilitiesDrainLifeDataDef { public:
    unsigned char _pad_0x0[0x28]; // to 0x28
    CSubField sub; // +0x28
};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

void __fastcall CSpecialAbilitiesForcePushDataDef_Copy(CSpecialAbilitiesDrainLifeDataDef* self, int _edx, const CSpecialAbilitiesDrainLifeDataDef* other)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)other);
    self->sub = other->sub;
}
// Stand-in (owner rule 2026-11-05): CSpecialAbilitiesForcePushDataDef_Copy for CSpecialAbilitiesForcePushDataDef::Copy; RTTI vtable slot ?Copy@CSpecialAbilitiesForcePushDataDef@@UAEXPBVCDefClassBase@@@Z + PDB name set (fablewin).
