// CMap::AddThemeDefIndexToPalette(unsigned long themeDefIndex) -- the theme's palette index; when the theme has none, the first
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
// Shape: retail has TWO store tails, one per arm (the free entry found at once / the one found after ZeroAll), so the claim is
//   written in both arms. In each, the ThemeDefIndex store goes through an iterator variable set to begin() and advanced by a
//   widened copy of the index, and the used flag through operator[]:
//   - after the inline search VC7.1 shares that iterator's begin() with the loops' start register, so the first arm stores
//     through edi (0x008F7178 `mov [edi+ecx*8],ebx`); after the ZeroAll/FindUnused calls it reloads [esi+1A6Ch] (0x008F7152);
//   - the unsigned long copy keeps index*8 in the address modes (`[edx+ecx*8]`), where a direct `operator[](index)` on the
//     unsigned char CSEs `shl ecx,3`, and makes the two arms different so they are not merged;
//   - operator[] for the flag re-reads the vector start after the dword store (0x008F717B / 0x008F715E).
//   `if (index != 0) {...} else {...}` gives retail's layout: the first arm's block last, `return 0` at the shared exit
//   (0x008F718C `xor al,al`).
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
        if (index != 0) {
            unsigned long i = index;
            std::vector<CThemePaletteEntry>::iterator it = ThemePaletteEntryList.begin();
            it += i;
            it->ThemeDefIndex = themeDefIndex;
            ThemePaletteEntryList[i].ThemePaletteIndexUsed = true;
        } else {
            ZeroAllUnusedThemePaletteEntries();
            index = FindUnusedThemePaletteEntryIndex();
            if (index != 0) {
                unsigned long i = index;
            std::vector<CThemePaletteEntry>::iterator it = ThemePaletteEntryList.begin();
            it += i;
            it->ThemeDefIndex = themeDefIndex;
            ThemePaletteEntryList[i].ThemePaletteIndexUsed = true;
            } else
                return 0;
        }
    }
    return index;
}