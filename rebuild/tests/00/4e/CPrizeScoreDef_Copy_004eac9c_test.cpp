#include <cstdio>

struct Base { int tag; };

class CPrizeScoreDef { public:
    char pad[0x28];
    int f28;
    int Mult;
    int basecalled;
};

static void base_dup(CPrizeScoreDef* self, const Base* src) {
    self->basecalled = 1;
    (void)src;
}

static void Copy_model(CPrizeScoreDef* self, const Base* src) {
    base_dup(self, src);
    const CPrizeScoreDef* s = (const CPrizeScoreDef*)src;
    self->f28 = s->f28;
    self->Mult = s->Mult;
}

int main() {
    CPrizeScoreDef src; src.f28 = 111; src.Mult = 222;
    CPrizeScoreDef dst; dst.f28 = 0; dst.Mult = 0; dst.basecalled = 0;
    Copy_model(&dst, (const Base*)&src);
    if (dst.f28 == 111 && dst.Mult == 222 && dst.basecalled == 1)
        printf("PRIZECOPY_OK\n");
    else
        printf("FAIL\n");
    return 0;
}