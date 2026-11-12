#include <cstdio>
// Behaviour test for CMap::AddThemeDefIndexToPalette (0x008F70F0): a copy of the logic over a model palette.
class CThemePaletteEntry { public: unsigned long ThemeDefIndex; bool ThemePaletteIndexUsed; };
struct CMapModel {
    CThemePaletteEntry Entries[4];
    unsigned long Count;
    unsigned char Used[4][1];         // cells' theme palette indices (ZeroAll re-marks these as used)
    unsigned long NoCells;
    unsigned char GetThemePaletteIndexFromThemeDefIndex(unsigned long themeDefIndex)
    {
        for (unsigned long index = 0; index < Count; ++index)
            if (index > 0 && Entries[index].ThemeDefIndex == themeDefIndex) return (unsigned char)index;
        return 0;
    }
    unsigned char FindUnusedThemePaletteEntryIndex()
    {
        for (unsigned long index = 0; index < Count; ++index)
            if (index > 0 && !Entries[index].ThemePaletteIndexUsed) return (unsigned char)index;
        return 0;
    }
    void ZeroAllUnusedThemePaletteEntries()
    {
        for (unsigned long i = 0; i < Count; ++i) Entries[i].ThemePaletteIndexUsed = false;
        Entries[0].ThemeDefIndex = 1; Entries[0].ThemePaletteIndexUsed = false;
        for (unsigned long c = 0; c < NoCells; ++c) Entries[Used[c][0]].ThemePaletteIndexUsed = true;
    }
    unsigned char AddThemeDefIndexToPalette(unsigned long themeDefIndex)
    {
        unsigned char index = GetThemePaletteIndexFromThemeDefIndex(themeDefIndex);
        if (index == 0) {
            index = FindUnusedThemePaletteEntryIndex();
            if (index != 0) {
                unsigned long i = index;
                CThemePaletteEntry* it = Entries; it += i;
                it->ThemeDefIndex = themeDefIndex;
                Entries[i].ThemePaletteIndexUsed = true;
            } else {
                ZeroAllUnusedThemePaletteEntries();
                index = FindUnusedThemePaletteEntryIndex();
                if (index != 0) {
                    unsigned long i = index;
                    CThemePaletteEntry* it = Entries; it += i;
                    it->ThemeDefIndex = themeDefIndex;
                    Entries[i].ThemePaletteIndexUsed = true;
                } else
                    return 0;
            }
        }
        return index;
    }
};
int main()
{
    CMapModel m;
    m.Count = 4; m.NoCells = 0;
    for (int i = 0; i < 4; ++i) { m.Entries[i].ThemeDefIndex = 0; m.Entries[i].ThemePaletteIndexUsed = false; }
    m.Entries[0].ThemePaletteIndexUsed = true;
    bool ok = true;
    ok = ok && m.AddThemeDefIndexToPalette(10) == 1 && m.Entries[1].ThemeDefIndex == 10 && m.Entries[1].ThemePaletteIndexUsed;
    ok = ok && m.AddThemeDefIndexToPalette(20) == 2 && m.AddThemeDefIndexToPalette(30) == 3;
    ok = ok && m.AddThemeDefIndexToPalette(20) == 2;              // already in the palette
    // palette full: cells only use entry 3, so ZeroAll frees 1 and 2 and the new theme takes entry 1
    m.Used[0][0] = 3; m.NoCells = 1;
    ok = ok && m.AddThemeDefIndexToPalette(40) == 1 && m.Entries[1].ThemeDefIndex == 40 && m.Entries[1].ThemePaletteIndexUsed;
    ok = ok && !m.Entries[2].ThemePaletteIndexUsed && m.Entries[3].ThemePaletteIndexUsed;
    // every entry in use by cells: full, returns 0
    m.Used[0][0] = 1; m.Used[1][0] = 2; m.Used[2][0] = 3; m.NoCells = 3;
    m.Entries[2].ThemePaletteIndexUsed = true;
    ok = ok && m.AddThemeDefIndexToPalette(50) == 0;
    if (!ok) { std::printf("008f70f0_TEST FAIL\n"); return 1; }
    std::printf("008f70f0_TEST PASS\n");
    return 0;
}