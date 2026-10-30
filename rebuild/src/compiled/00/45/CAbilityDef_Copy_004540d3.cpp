#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
class CAbilityDef { public:
    unsigned char _tail[0x2c];
};

void __fastcall CAbilityDef_Copy(CAbilityDef* self, void* dummy, const CAbilityDef* other)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(other));
    *(unsigned long*)(self->_tail + 0x28) = *(const unsigned long*)(other->_tail + 0x28);
}