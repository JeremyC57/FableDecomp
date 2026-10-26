
#include "engine/CHairCardDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

void __fastcall CHairCardDef_Copy(CHairCardDef* self, int edx_dummy, CHairCardDef* other)
{
    (void)edx_dummy;
    reinterpret_cast<CDefClassBase*>(self)->operator=(*(const CDefClassBase*)(other));
    self->HairObject = other->HairObject;
}