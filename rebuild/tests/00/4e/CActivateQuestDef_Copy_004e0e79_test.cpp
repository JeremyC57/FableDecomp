#include <cstdio>

struct Base {
    int calls;
    Base(){calls=0;}
};

struct Def {
    char pad[0x28];
    int  field28;
    char LoadResources;
};

static int g_basecopy_calls = 0;

void BaseCopy(Def* self, Def const* src) {
    g_basecopy_calls++;
    (void)self; (void)src;
}

void Copy(Def* self, Def const* src) {
    BaseCopy(self, src);
    self->field28 = src->field28;
    self->LoadResources = src->LoadResources;
}

int main() {
    Def a; Def b;
    b.field28 = 0x12345678;
    b.LoadResources = (char)0xAB;
    a.field28 = 0; a.LoadResources = 0;
    Copy(&a, &b);
    if (a.field28 == 0x12345678 && a.LoadResources == (char)0xAB && g_basecopy_calls == 1) {
        printf("COPY_OK\n");
    } else {
        printf("COPY_FAIL\n");
    }
    return 0;
}