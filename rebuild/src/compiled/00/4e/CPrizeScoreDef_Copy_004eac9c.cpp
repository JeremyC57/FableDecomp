// Leaf: ?Copy@CPrizeScoreDef@@UAEXPBVCDefClassBase@@@Z
#pragma optimize("s",on)
// CPrizeScoreDef::Copy @ 0x004eac9c
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
class CPrizeScoreDef;

// base class Copy at 0x431f10 : void __fastcall CDefClassBase::Copy(CDefClassBase* self, CDefClassBase const* src)

class CPrizeScoreDef { public:
    char pad[0x24];
    int f28;
    int Mult;
    virtual void Copy(const class CDefClassBase*);
};

void CPrizeScoreDef::Copy(const class CDefClassBase* src)
{
    CPrizeScoreDef* self = this;
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    const CPrizeScoreDef* s = (const CPrizeScoreDef*)src;
    self->f28 = s->f28;
    self->Mult = s->Mult;
}
// Flatten route (flatten_pilot v4, NF324): CPrizeScoreDef_Copy -> CPrizeScoreDef::Copy, the RTTI-placed PDB member; class model: the row's local class model.
