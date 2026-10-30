#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

class CWifeDef { public:
    char pad[0x28];
    int f28;
    // base-class copy at 0x431f10; member => thiscall(this, src)
};

void __fastcall CWifeDef_Copy(CWifeDef* self, int /*edx*/, const CDefClassBase* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    self->f28 = ((const CWifeDef*)src)->f28;
}