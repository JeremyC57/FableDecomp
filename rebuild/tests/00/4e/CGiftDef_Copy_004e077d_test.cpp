#include <stdio.h>
#include <string.h>

struct Base { char basepad[0x28]; };
struct Gift : Base {
    unsigned int Type;
    unsigned char IsWeddingRing;
};

// model: base copy fills the base region, derived copies f28/f2c
static void BaseCopy(Gift* self, const Gift* src) {
    memcpy((Base*)self, (const Base*)src, sizeof(Base));
}

static void Copy(Gift* self, const Gift* src) {
    BaseCopy(self, src);
    self->Type = src->Type;
    self->IsWeddingRing = src->IsWeddingRing;
}

int main() {
    Gift src; memset(&src, 0, sizeof(src));
    for (int i = 0; i < 0x28; i++) src.basepad[i] = (char)(i + 1);
    src.Type = 0xDEADBEEF;
    src.IsWeddingRing = 0x5A;

    Gift dst; memset(&dst, 0xFF, sizeof(dst));
    Copy(&dst, &src);

    bool ok = true;
    for (int i = 0; i < 0x28; i++) if (dst.basepad[i] != (char)(i + 1)) ok = false;
    if (dst.Type != 0xDEADBEEF) ok = false;
    if (dst.IsWeddingRing != 0x5A) ok = false;

    if (ok) printf("GIFTCOPY_OK\n");
    else printf("FAIL\n");
    return 0;
}