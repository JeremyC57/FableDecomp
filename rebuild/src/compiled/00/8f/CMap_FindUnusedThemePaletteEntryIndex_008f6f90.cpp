// CMap::FindUnusedThemePaletteEntryIndex() -- the index of the first unused theme palette entry after entry 0 (0 when every entry is used)
// Leaf: ?FindUnusedThemePaletteEntryIndex@CMap@@IAEEXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F6F90, 42 bytes (+ int3 padding), thiscall, `ret`.
// Identity (proven): label correction -- the row is labelled CMusicManager::ResetMusicEntriesOffsets (a void PDB method; this
//   body returns the index in al). Ego_r.pdb donor order GetThemePaletteIndexFromThemeDefIndex (0x008545D0) /
//   FindUnusedThemePaletteEntryIndex (0x00854600) / ZeroAllUnusedThemePaletteEntries (0x00854630) / AddThemeDefIndexToPalette
//   (0x00854760) / InitThemePalette (0x00854830) = retail 0x008F6F90 (this row) / 0x008F6FC0 / 0x008F70F0 / 0x008F71A0: the
//   only caller is AddThemeDefIndexToPalette 0x008F70F0 (right after ZeroAllUnusedThemePaletteEntries 0x008F6FC0), whose
//   result indexes the palette at +0x1A6C; InitThemePalette fills the same vector.
// Layout: Ego_r.pdb CMap ThemePaletteEntryList vector<CThemePaletteEntry> at retail +0x1A6C (Ego_r +0x1A78, STLport vectors);
//   CThemePaletteEntry {ThemeDefIndex +0, ThemePaletteIndexUsed +4}. No engine header for CMap: local model.
// Shape: index counter alongside the iterator; `index > 0 && !used` returns the index (unsigned `jbe`), else 0.
namespace std {
template <class _Tp> class vector {                     // STLport 4.6 layout (12 bytes)
public:
    typedef _Tp* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
class CThemePaletteEntry {                              // Ego_r.pdb: 8 bytes
public:
    unsigned long ThemeDefIndex;                        // +0x00
    bool ThemePaletteIndexUsed;                         // +0x04
};
class CMap {
public:
    unsigned char _pad0000[0x1A6C];
    std::vector<CThemePaletteEntry> ThemePaletteEntryList;   // +0x1A6C (Ego_r.pdb +0x1A78)
protected:
    unsigned char FindUnusedThemePaletteEntryIndex();
};

unsigned char CMap::FindUnusedThemePaletteEntryIndex()
{
    unsigned long index = 0;
    for (std::vector<CThemePaletteEntry>::iterator it = ThemePaletteEntryList.begin(); it != ThemePaletteEntryList.end(); ++it, ++index) {
        if (index > 0 && !it->ThemePaletteIndexUsed)
            return (unsigned char)index;
    }
    return 0;
}