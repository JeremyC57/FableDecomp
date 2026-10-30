#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

class CSmokeGeneratorDef { public:
    char pad[0x28];
    int m28;
    int ParticleEmitter;
};

void __fastcall CSmokeGeneratorDef_Copy(CSmokeGeneratorDef* self, void* /*edx*/, const CDefClassBase* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    const CSmokeGeneratorDef* s = (const CSmokeGeneratorDef*)src;
    self->m28 = s->m28;
    self->ParticleEmitter = s->ParticleEmitter;
}