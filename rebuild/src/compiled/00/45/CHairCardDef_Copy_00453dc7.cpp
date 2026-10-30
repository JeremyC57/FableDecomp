
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CHairCardDef.h"  // retyped onto the PDB layout; byte parity re-verified
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

void __fastcall CHairCardDef_Copy(CHairCardDef* self, int edx_dummy, CHairCardDef* other)
{
    (void)edx_dummy;
    reinterpret_cast<CDefClassBase*>(self)->operator=(*(const CDefClassBase*)(other));
    self->HairObject = other->HairObject;
}