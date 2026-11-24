// Leaf: ?Copy@CSmokeGeneratorDef@@UAEXPBVCDefClassBase@@@Z
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

class CSmokeGeneratorDef { public:
    char pad[0x24];
    int m28;
    int ParticleEmitter;
    virtual void Copy(const class CDefClassBase*);
};

void CSmokeGeneratorDef::Copy(const class CDefClassBase* src)
{
    CSmokeGeneratorDef* self = this;
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    const CSmokeGeneratorDef* s = (const CSmokeGeneratorDef*)src;
    self->m28 = s->m28;
    self->ParticleEmitter = s->ParticleEmitter;
}
// Flatten route (flatten_pilot v4, NF324): CSmokeGeneratorDef_Copy -> CSmokeGeneratorDef::Copy, the RTTI-placed PDB member; class model: the row's local class model.
