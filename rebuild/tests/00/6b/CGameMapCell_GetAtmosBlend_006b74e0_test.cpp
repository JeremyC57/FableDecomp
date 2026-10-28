// Behaviour test for 0x006B74E0 CGameMapCell::GetAtmosBlend: blend 2 is the remainder of 255 after blends 0 and 1 (standalone copy of the logic)
#include <cstdio>
struct Cell { unsigned char AtmosBlends[3];
    unsigned char Get(long index) const { if (index == 2) { long total = 0; for (long i = 0; i != index; ++i) total += AtmosBlends[i]; return (unsigned char)(255 - total); } return AtmosBlends[index]; } };

int main()
{
    Cell c; c.AtmosBlends[0] = 100; c.AtmosBlends[1] = 55; c.AtmosBlends[2] = 9;
    if (c.Get(0) != 100 || c.Get(1) != 55 || c.Get(2) != 100) { std::printf("006b74e0_TEST FAIL\n"); return 1; }
    std::printf("006b74e0_TEST PASS\n");
    return 0;
}