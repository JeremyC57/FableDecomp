#include <stdio.h>

// Behaviour model of CDecapitationDef::Copy
struct Base { unsigned int baseCopied; };

struct Def {
    char pad[0x28];
    unsigned int BodyParticleEffect;
    unsigned int Head;
    unsigned int baseFlag;
};

static void base_copy(Def* self, const Def* src) {
    self->baseFlag = src->baseFlag; // stand-in for base class field copy
}

static void Copy(Def* self, const Def* src) {
    base_copy(self, src);
    self->BodyParticleEffect = src->BodyParticleEffect;
    self->Head = src->Head;
}

int main() {
    Def src; Def dst;
    for (int i = 0; i < 0x28; ++i) { src.pad[i] = (char)0; dst.pad[i] = (char)0; }
    src.BodyParticleEffect = 0x11112222;
    src.Head = 0x33334444;
    src.baseFlag = 0xABCD;
    dst.BodyParticleEffect = 0; dst.Head = 0; dst.baseFlag = 0;
    Copy(&dst, &src);
    if (dst.BodyParticleEffect == 0x11112222 && dst.Head == 0x33334444 && dst.baseFlag == 0xABCD) {
        printf("PARITY_OK_004e1225\n");
    } else {
        printf("PARITY_FAIL\n");
    }
    return 0;
}