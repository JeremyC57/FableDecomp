// CMap::AddThemeDefIndexToPalette(unsigned long themeDefIndex) -- the theme's palette index; when the theme has none, the first
// Grade: DIFFER(144v164) -- searches, ZeroAll call and registers match; left: retail keeps two store tails, the one after the inline FindUnusedThemePaletteEntryIndex storing ThemeDefIndex through edi (the vector start loaded at the top: `mov [edi+ecx*8],ebx`) and neither tail CSEs index*8; VC7.1 RTM (and QFE 4035) never reuses the start there (a toy without any call reloads it too), so ours keeps one tail with `shl ecx,3`. Tried: duplicated arms, early-return arm, separate variable, textually different arms, SetThemePaletteIndexUsed accessor, iterator begin()+index, &v[0] pointer, store guarded after both searches, early return of the found index, non-const GetThemePaletteIndexFromThemeDefIndex, FindUnused out of class in the TU (auto-inline), QFE compiler
// unused palette entry (re-marking the used entries with ZeroAllUnusedThemePaletteEntries when none is free) is given to it;
// 0 when the palette is full.
// Leaf: ?AddThemeDefIndexToPalette@CMap@@IAEEK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F70F0, 164 bytes, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?AddThemeDefIndexToPalette@CMap@@IAEEK@Z; Ego_r donor order GetThemePaletteIndexFromThemeDefIndex
//   / FindUnusedThemePaletteEntryIndex / ZeroAllUnusedThemePaletteEntries / AddThemeDefIndexToPalette = retail (inline) /
//   0x008F6F90 / 0x008F6FC0 / 0x008F70F0 (engine/CMap.h FABLE_RETAIL_CMAP_THEMEPALETTE).
// Callees: CMap::ZeroAllUnusedThemePaletteEntries 0x008F6FC0, CMap::FindUnusedThemePaletteEntryIndex 0x008F6F90 (inline at its
//   first call, out of line at its second); GetThemePaletteIndexFromThemeDefIndex inline.
// Layout: CMap ThemePaletteEntryList +0x1A6C (STLport vector<CThemePaletteEntry>, 8-byte entries).
#define FABLE_RETAIL_CMAP_THEMEPALETTE
#define FABLE_RETAIL_CMAP_THEMEPALETTE_GETINDEX_NONCONST
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CHEIGHTMAP_GETCELL
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/CMap.h"
#include "engine/CHeightMap.h"

// The real body of the landed CMap::ZeroAllUnusedThemePaletteEntries 0x008F6FC0 (CMap_ZeroAllUnusedThemePaletteEntries_008f6fc0.cpp),
// out of line in this TU as in retail: VC7.1 then knows it leaves ecx alone, so `this` is not reloaded before the second
// FindUnusedThemePaletteEntryIndex call (0x008F7149).
#pragma auto_inline(off)
void CMap::ZeroAllUnusedThemePaletteEntries()
{
    for (std::vector<CThemePaletteEntry>::iterator it = ThemePaletteEntryList.begin(); it != ThemePaletteEntryList.end(); ++it)
        it->ThemePaletteIndexUsed = false;
    ThemePaletteEntryList[0] = CThemePaletteEntry(1, false);
    for (long y = 0; y < HeightMap->SizeY; ++y)
        for (long x = 0; x < HeightMap->SizeX; ++x)
            for (long i = 0; i < 3; ++i)
                ThemePaletteEntryList[HeightMap->GetCell(C2DCoordI(x, y)).EngineThemePaletteIndex[i]].ThemePaletteIndexUsed = true;
}
#pragma auto_inline(on)

unsigned char CMap::AddThemeDefIndexToPalette(unsigned long themeDefIndex)
{
    unsigned char index = GetThemePaletteIndexFromThemeDefIndex(themeDefIndex);
    if (index == 0) {
        index = FindUnusedThemePaletteEntryIndex();
        if (index == 0) {
            ZeroAllUnusedThemePaletteEntries();
            index = FindUnusedThemePaletteEntryIndex();
            if (index == 0)
                return 0;
        }
        ThemePaletteEntryList[index].ThemeDefIndex = themeDefIndex;
        ThemePaletteEntryList[index].ThemePaletteIndexUsed = true;
    }
    return index;
}
