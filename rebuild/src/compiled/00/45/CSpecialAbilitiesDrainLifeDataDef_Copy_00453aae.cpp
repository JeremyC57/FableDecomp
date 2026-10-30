struct CSubField { int v; };
class CSpecialAbilitiesDrainLifeDataDef { public:
    unsigned char _pad_0x0[0x28]; // to 0x28
    CSubField sub; // +0x28
};
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

void __fastcall CSpecialAbilitiesDrainLifeDataDef_Copy(CSpecialAbilitiesDrainLifeDataDef* self, int _edx, const CSpecialAbilitiesDrainLifeDataDef* other)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)other);
    self->sub = other->sub;
}