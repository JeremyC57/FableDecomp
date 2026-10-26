#include <cstdio>

class CRegionDef { public:};

unsigned long __fastcall CRegionDef_GetSizeofClass(CRegionDef* def)
{
    return 0xc4;
}

int main()
{
    CRegionDef d;
    unsigned long r = CRegionDef_GetSizeofClass(&d);
    if (r == 0xc4) {
        std::printf("CRegionDef_004564e5_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}