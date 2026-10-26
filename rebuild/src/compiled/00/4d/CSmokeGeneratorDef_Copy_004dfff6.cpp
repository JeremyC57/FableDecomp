#pragma optimize("s",on)
class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

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