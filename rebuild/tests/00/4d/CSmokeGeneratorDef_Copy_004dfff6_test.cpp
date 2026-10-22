#include <cstdio>

struct CSmokeGeneratorDef {
    char pad[0x28];
    int m28;
    int ParticleEmitter;
    int base_called;
};

void __fastcall Base_Copy(CSmokeGeneratorDef* self, void* /*edx*/, const void* src)
{
    self->base_called = 1;
}

void __fastcall CSmokeGeneratorDef_Copy(CSmokeGeneratorDef* self, void* /*edx*/, const void* src)
{
    Base_Copy(self, 0, src);
    const CSmokeGeneratorDef* s = (const CSmokeGeneratorDef*)src;
    self->m28 = s->m28;
    self->ParticleEmitter = s->ParticleEmitter;
}

int main()
{
    CSmokeGeneratorDef dst; CSmokeGeneratorDef src;
    dst.m28 = 0; dst.ParticleEmitter = 0; dst.base_called = 0;
    src.m28 = 0x1111; src.ParticleEmitter = 0x2222;
    CSmokeGeneratorDef_Copy(&dst, 0, &src);
    if (dst.m28 == 0x1111 && dst.ParticleEmitter == 0x2222 && dst.base_called == 1)
        printf("COPY_OK\n");
    else
        printf("COPY_FAIL\n");
    return 0;
}