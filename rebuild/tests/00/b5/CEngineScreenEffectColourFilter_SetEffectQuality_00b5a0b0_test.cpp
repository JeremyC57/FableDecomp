#include <stdio.h>

struct Def { unsigned char pad[0x72]; unsigned char EnableGlow; };
struct Filter { unsigned char pad0[8]; unsigned char enabled; unsigned char pad1[0x44-9]; unsigned char field44; };

static void SetEffectQuality(Filter* self, Def* def)
{
    if (def->EnableGlow && self->field44)
        self->enabled = 1;
    else
        self->enabled = 0;
}

int main()
{
    Def d; Filter f;
    d.EnableGlow = 5; f.field44 = 9; f.enabled = 0xEE;
    SetEffectQuality(&f, &d);
    if (f.enabled != 1) { printf("BAD1\n"); return 1; }
    d.EnableGlow = 0; f.field44 = 9; f.enabled = 0xEE;
    SetEffectQuality(&f, &d);
    if (f.enabled != 0) { printf("BAD2\n"); return 1; }
    d.EnableGlow = 1; f.field44 = 0; f.enabled = 0xEE;
    SetEffectQuality(&f, &d);
    if (f.enabled != 0) { printf("BAD3\n"); return 1; }
    printf("QUALITY_OK\n");
    return 0;
}