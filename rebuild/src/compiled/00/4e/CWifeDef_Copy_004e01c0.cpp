// Leaf: ?Copy@CWifeDef@@UAEXPBVCDefClassBase@@@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

class CWifeDef { public:
    char pad[0x24];
    int f28;
    // base-class copy at 0x431f10; member => thiscall(this, src)
    virtual void Copy(const class CDefClassBase*);
};

void CWifeDef::Copy(const class CDefClassBase* src)
{
    CWifeDef* self = this;
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    self->f28 = ((const CWifeDef*)src)->f28;
}
// Flatten route (flatten_pilot v6, NF335): CWifeDef_Copy -> CWifeDef::Copy, the RTTI-placed PDB member; class model: the row's local class model.
