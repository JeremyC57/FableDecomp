// Behaviour test for 0x008F71A0 CMap::InitThemePalette: the palette is emptied and refilled with 256 entries
// (index 0, unused) except entry 0 marked used, then the invalid-theme stand-in is added.
#include <cstdio>

class CThemePaletteEntry {
public:
    unsigned int ThemeDefIndex;
    bool ThemePaletteIndexUsed;
    CThemePaletteEntry() : ThemeDefIndex(0xFFFF), ThemePaletteIndexUsed(true) {}
    CThemePaletteEntry(unsigned int themeDefIndex, bool used) : ThemeDefIndex(themeDefIndex), ThemePaletteIndexUsed(used) {}
    void SetThemePaletteIndexUsed(bool used) { ThemePaletteIndexUsed = used; }
};
class CPaletteList {                     // fixed-capacity stand-in for the STLport vector
public:
    CThemePaletteEntry Items[300];
    unsigned int Count;
    CPaletteList() : Count(0) {}
    void clear() { Count = 0; }
    void push_back(const CThemePaletteEntry& entry) { Items[Count++] = entry; }
    unsigned int size() const { return Count; }
    CThemePaletteEntry& operator[](unsigned int i) { return Items[i]; }
};
class CGlobalDef { public: int InvalidThemeStandin; };
class CGameDefinitionManager {
public:
    CGlobalDef Global;
    const CGlobalDef& PeekGlobalDef() const { return Global; }
};
class CMap {
public:
    const CGameDefinitionManager* DefinitionManager;
    CPaletteList ThemePaletteEntryList;
    unsigned int Added;
    unsigned char AddThemeDefIndexToPalette(unsigned int themeDefIndex) { Added = themeDefIndex; return 1; }
    void InitThemePalette()
    {
        ThemePaletteEntryList.clear();
        CThemePaletteEntry entry(0, false);
        for (int i = 0; i < 256; ++i) {
            ThemePaletteEntryList.push_back(entry);
            ThemePaletteEntryList[i].SetThemePaletteIndexUsed(false);
        }
        ThemePaletteEntryList[0].SetThemePaletteIndexUsed(true);
        AddThemeDefIndexToPalette(DefinitionManager->PeekGlobalDef().InvalidThemeStandin);
    }
};

int main()
{
    CGameDefinitionManager defs;
    defs.Global.InvalidThemeStandin = 77;
    CMap map;
    map.DefinitionManager = &defs;
    map.Added = 0;
    for (int i = 0; i < 5; ++i) map.ThemePaletteEntryList.push_back(CThemePaletteEntry(9, true));
    map.InitThemePalette();
    int fail = 0;
    if (map.ThemePaletteEntryList.size() != 256) fail = 1;
    for (unsigned int i = 0; i < map.ThemePaletteEntryList.size(); ++i) {
        if (map.ThemePaletteEntryList[i].ThemeDefIndex != 0) fail = 1;
        if (map.ThemePaletteEntryList[i].ThemePaletteIndexUsed != (i == 0)) fail = 1;
    }
    if (map.Added != 77) fail = 1;
    if (fail) { std::printf("008f71a0_TEST FAIL\n"); return 1; }
    std::printf("008f71a0_TEST PASS\n");
    return 0;
}