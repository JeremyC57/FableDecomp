#include <stdio.h>

class CDefClassBase { public: char pad[0x40]; };

struct CReadableBase {
    void Copy(CDefClassBase const* src);
};

class CReadableDef : public CReadableBase { public:
    unsigned char field25pad[0x25];
    unsigned char UseInfoDisplay;
    void Copy(CDefClassBase const* src);
};

static int g_basecalled = 0;
void CReadableBase::Copy(CDefClassBase const* src) {
    g_basecalled = 1;
    (void)src;
}

void __fastcall CReadableDef_Copy(CReadableDef* self, int, CDefClassBase const* src);

int main() {
    CReadableDef dst; CReadableDef srcObj;
    for (int i=0;i<(int)sizeof(dst);++i) ((char*)&dst)[i]=0;
    for (int i=0;i<(int)sizeof(srcObj);++i) ((char*)&srcObj)[i]=0;
    srcObj.UseInfoDisplay = 0xAB;
    dst.UseInfoDisplay = 0x00;
    CReadableDef_Copy(&dst, 0, (CDefClassBase const*)&srcObj);
    if (g_basecalled && dst.UseInfoDisplay == 0xAB) {
        printf("004e00a3_TEST PASS\n");
    } else {
        printf("FAIL base=%d f=%02x\n", g_basecalled, (unsigned)dst.UseInfoDisplay);
    }
    return 0;
}