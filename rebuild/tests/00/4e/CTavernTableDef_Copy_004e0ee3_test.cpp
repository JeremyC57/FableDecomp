#include <cstdio>

struct CTavernTableDef {
    unsigned char pad[0x25];
    unsigned char AddTankards;
    unsigned char HighQualityTankards;
    int base_copied;
};

static void Base_Copy(CTavernTableDef* self, CTavernTableDef* src) {
    self->base_copied = 1;
    (void)src;
}

static void Copy(CTavernTableDef* self, CTavernTableDef* src) {
    Base_Copy(self, src);
    self->AddTankards = src->AddTankards;
    self->HighQualityTankards = src->HighQualityTankards;
}

int main() {
    CTavernTableDef src = {};
    CTavernTableDef dst = {};
    src.AddTankards = 0xAB;
    src.HighQualityTankards = 0xCD;
    Copy(&dst, &src);
    if (dst.AddTankards == 0xAB && dst.HighQualityTankards == 0xCD && dst.base_copied == 1) {
        printf("COPY_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}