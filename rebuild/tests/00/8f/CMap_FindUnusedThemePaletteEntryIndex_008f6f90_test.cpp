// Test: CMap::FindUnusedThemePaletteEntryIndex -- first unused palette slot after slot 0, else 0
#include <cstdio>
class CThemePaletteEntry { public: unsigned long ThemeDefIndex; bool ThemePaletteIndexUsed; };
static unsigned char FindUnused(CThemePaletteEntry* b, CThemePaletteEntry* e)
{
    unsigned long index = 0;
    for (CThemePaletteEntry* it = b; it != e; ++it, ++index)
        if (index > 0 && !it->ThemePaletteIndexUsed) return (unsigned char)index;
    return 0;
}
int main()
{
    CThemePaletteEntry p[4] = { { 0, false }, { 1, true }, { 2, false }, { 3, false } };
    CThemePaletteEntry q[2] = { { 0, false }, { 1, true } };
    if (FindUnused(p, p + 4) != 2 || FindUnused(q, q + 2) != 0 || FindUnused(p, p + 1) != 0) { std::printf("008f6f90_TEST FAIL\n"); return 1; }
    std::printf("008f6f90_TEST PASS\n");
    return 0;
}