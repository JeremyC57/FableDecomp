// Grade: DIFFER(144v164) -- the two inline palette searches keep the index in eax and the cursor in ecx (retail: cursor eax, index edx, then `mov al,dl`), and our free-slot store tails are merged where retail duplicates them (164 bytes)
// CMap::AddThemeDefIndexToPalette(unsigned long themeDefIndex) -- return the theme's palette index, allocating a free palette slot (after re-marking the used entries when none is free); 0 when the palette is full
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F70F0, length 164 bytes (`ret 4`).
// Callees: CMap::UpdateThemePaletteUsage @ 0x008F6FC0 (same TU, defined above: VC7.1 knows it leaves ecx alone, so `this` is not
// reloaded for the next call), CMap::GetFreeThemePaletteIndex @ 0x008F6F90 (the in-class inline helper's out-of-line copy: inlined at its
// first call, called at its second).
// Layout: retail CMap = Ego_r.pdb - 0xC from the palette on (4-byte list, 12-byte vector): ThemePaletteEntryList +0x1A6C (STLport vector of
// 8-byte CThemePaletteEntry {ThemeDefIndex, ThemePaletteIndexUsed}), the game map +0x1A7C (cells +0x10, SizeX +0x204, SizeY +0x208 in retail).
// Shape: index 0 is reserved (`i > 0` unsigned test in both searches); the result is an unsigned char.
struct CThemePaletteEntry {                     // 8 bytes
    unsigned long ThemeDefIndex;
    bool          ThemePaletteIndexUsed;
    CThemePaletteEntry(unsigned long index, bool used) : ThemeDefIndex(index), ThemePaletteIndexUsed(used) { }
};
struct CThemePaletteVector {                    // STLport vector<CThemePaletteEntry>
    CThemePaletteEntry* _M_start;
    CThemePaletteEntry* _M_finish;
    CThemePaletteEntry* _M_end_of_storage;
    CThemePaletteEntry* begin() const { return _M_start; }
    CThemePaletteEntry* end() const { return _M_finish; }
    CThemePaletteEntry& operator[](unsigned long i) const { return *(begin() + i); }
};
struct CGameMapCell {                           // 12 bytes
    long          Flags;
    unsigned char Themes[3];                    // +0x04
    unsigned char _pad7;
    long          Height;
};
struct CGameMap {
    unsigned char _pad00[0x10];
    CGameMapCell* Cells;                        // +0x10
    unsigned char _pad14[0x204 - 0x14];
    long          SizeX;                        // +0x204
    long          SizeY;                        // +0x208
    CGameMapCell& GetCell(long x, long y) const { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};
template<class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
};
struct CMap {
    void*                __vftable;             // +0x0000
    unsigned char        _pad04[0x1a6c - 4];
    CThemePaletteVector  ThemePaletteEntryList; // +0x1a6c
    scoped_ptr<CGameMap> GameMap;               // +0x1a7c
    unsigned char FindThemePaletteIndex(unsigned long themeDefIndex) const
    {
        CThemePaletteEntry* e = ThemePaletteEntryList.begin();
        for (unsigned long i = 0; e != ThemePaletteEntryList.end(); ++e, ++i)
            if (i > 0 && e->ThemeDefIndex == themeDefIndex)
                return (unsigned char)i;
        return 0;
    }
    unsigned char GetFreeThemePaletteIndex() const      // 0x008F6F90 when not inlined
    {
        CThemePaletteEntry* e = ThemePaletteEntryList.begin();
        for (unsigned long i = 0; e != ThemePaletteEntryList.end(); ++e, ++i)
            if (i > 0 && !e->ThemePaletteIndexUsed)
                return (unsigned char)i;
        return 0;
    }
    void UpdateThemePaletteUsage();             // 0x008F6FC0
    unsigned char AddThemeDefIndexToPalette(unsigned long themeDefIndex);
};

#pragma auto_inline(off)
void CMap::UpdateThemePaletteUsage()
{
    for (CThemePaletteEntry* e = ThemePaletteEntryList.begin(); e != ThemePaletteEntryList.end(); ++e)
        e->ThemePaletteIndexUsed = false;
    ThemePaletteEntryList[0] = CThemePaletteEntry(1, false);
    for (long y = 0; y < GameMap->SizeY; ++y)
        for (long x = 0; x < GameMap->SizeX; ++x) {
            ThemePaletteEntryList[GameMap->GetCell(x, y).Themes[0]].ThemePaletteIndexUsed = true;
            ThemePaletteEntryList[GameMap->GetCell(x, y).Themes[1]].ThemePaletteIndexUsed = true;
            ThemePaletteEntryList[GameMap->GetCell(x, y).Themes[2]].ThemePaletteIndexUsed = true;
        }
}
#pragma auto_inline(on)

unsigned char CMap::AddThemeDefIndexToPalette(unsigned long themeDefIndex)
{
    unsigned char index = FindThemePaletteIndex(themeDefIndex);
    if (index)
        return index;
    index = GetFreeThemePaletteIndex();
    if (index) {
        ThemePaletteEntryList[index].ThemeDefIndex = themeDefIndex;
        ThemePaletteEntryList[index].ThemePaletteIndexUsed = true;
        return index;
    }
    UpdateThemePaletteUsage();
    index = GetFreeThemePaletteIndex();
    if (index) {
        ThemePaletteEntryList[index].ThemeDefIndex = themeDefIndex;
        ThemePaletteEntryList[index].ThemePaletteIndexUsed = true;
        return index;
    }
    return 0;
}
