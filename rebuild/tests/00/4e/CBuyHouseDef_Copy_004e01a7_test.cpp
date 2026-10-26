#include <stdio.h>

class CDefClassBase { public: char pad[0x40]; 
public:
    CDefClassBase& operator=(const CDefClassBase& other);
};

static int g_basecalled = 0;

struct CBuyHouseBase {
    void BaseCopy(CDefClassBase const* src);
};
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other_)
{
    CDefClassBase const* src = (CDefClassBase const*)&other_; (void)src; g_basecalled = 1; (void)src;
    return *this;
}

class CBuyHouseDef { public:
    char pad[0x25];
    unsigned char Main;
};

void __fastcall CBuyHouseDef_Copy(CBuyHouseDef* self, int, CDefClassBase const* src);

int main() {
    CBuyHouseDef dst; CBuyHouseDef srcObj;
    for (int i=0;i<(int)sizeof(dst);++i) ((char*)&dst)[i]=0;
    for (int i=0;i<(int)sizeof(srcObj);++i) ((char*)&srcObj)[i]=0;
    srcObj.Main = 0xAB;
    dst.Main = 0x00;
    CBuyHouseDef_Copy(&dst, 0, (CDefClassBase const*)&srcObj);
    if (g_basecalled && dst.Main == 0xAB) {
        printf("004e01a7_TEST PASS\n");
    } else {
        printf("FAIL base=%d f=%02x\n", g_basecalled, dst.Main);
    }
    return 0;
}