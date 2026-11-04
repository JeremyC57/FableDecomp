// CMap::AddThemeDefIndexToPalette(unsigned long themeDefIndex) -- return the theme's palette index, allocating a free palette slot (after zeroing the unused entries when none is free); 0 when the palette is full
// Grade: DIFFER(144v164) -- dl=37 (try i, retail headers + CMAP_THEMEPALETTE overlay; the draft had no Leaf line): the inline searches keep the iterator in ecx and the index in eax (retail eax/edx, `mov al,dl`), and the free-slot store tail after the inline search reloads the vector begin and is cross-jumped into the post-retry tail (retail stores through the cached begin in edi and keeps two tails); single-store / nested forms 138-144 bytes
// Leaf: ?AddThemeDefIndexToPalette@CMap@@QAEEK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F70F0, 164 bytes (`ret 4`).
// Callees: CMap::ZeroAllUnusedThemePaletteEntries 0x008F6FC0 and CMap::FindUnusedThemePaletteEntryIndex 0x008F6F90 (both
//   landed; Ego_r.pdb donor order, see engine/CMap.h FABLE_RETAIL_CMAP_THEMEPALETTE). The inline searches are
//   GetThemePaletteIndexFromThemeDefIndex and the first FindUnusedThemePaletteEntryIndex call.
// Same TU: ZeroAllUnusedThemePaletteEntries is defined here (retail compiled it in this TU, right before this row), so VC7.1
//   knows the call leaves ecx alone and does not reload `this` for the next call (declaration only: 146 bytes).
// Layout: retail CMap ThemePaletteEntryList +0x1A6C, HeightMap +0x1A7C; CHeightMap Cells (vector<CHeightMapCell>) +0x10,
//   SizeX +0x204, SizeY +0x208 (Ego_r.pdb +0x10/+0x208/+0x20C, the 16-byte Ego_r vector); no CHeightMap header: local model.
#define FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMAP_THEMEPALETTE
#include "engine/CMap.h"
#include "engine/CHeightMapCell.h"

class CHeightMap {                                      // local model (no engine header)
public:
    unsigned char _pad00[0x10];
    std::vector<CHeightMapCell> Cells;                  // +0x10
    unsigned char _pad1c[0x204 - 0x1c];
    long SizeX;                                         // +0x204
    long SizeY;                                         // +0x208
    CHeightMapCell& GetCell(long x, long y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};

#pragma auto_inline(off)
void CMap::ZeroAllUnusedThemePaletteEntries()          // 0x008F6FC0 (landed; same-TU body)
{
    for (std::vector<CThemePaletteEntry>::iterator it = ThemePaletteEntryList.begin(); it != ThemePaletteEntryList.end(); ++it)
        it->ThemePaletteIndexUsed = false;
    ThemePaletteEntryList[0] = CThemePaletteEntry(1, false);
    for (long y = 0; y < HeightMap->SizeY; ++y)
        for (long x = 0; x < HeightMap->SizeX; ++x)
            for (long i = 0; i < 3; ++i)
                ThemePaletteEntryList[HeightMap->GetCell(x, y).EngineThemePaletteIndex[i]].ThemePaletteIndexUsed = true;
}
#pragma auto_inline(on)

unsigned char CMap::AddThemeDefIndexToPalette(unsigned long themeDefIndex)
{
    unsigned char index = GetThemePaletteIndexFromThemeDefIndex(themeDefIndex);
    if (index)
        return index;
    index = FindUnusedThemePaletteEntryIndex();
    if (index) {
        ThemePaletteEntryList[index].ThemeDefIndex = themeDefIndex;
        ThemePaletteEntryList[index].ThemePaletteIndexUsed = true;
        return index;
    }
    ZeroAllUnusedThemePaletteEntries();
    index = FindUnusedThemePaletteEntryIndex();
    if (index) {
        ThemePaletteEntryList[index].ThemeDefIndex = themeDefIndex;
        ThemePaletteEntryList[index].ThemePaletteIndexUsed = true;
        return index;
    }
    return 0;
}
