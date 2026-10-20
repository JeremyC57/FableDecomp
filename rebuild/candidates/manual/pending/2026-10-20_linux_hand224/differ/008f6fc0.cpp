// CMap::ZeroAllUnusedThemePaletteEntries() -- clear every palette entry's used flag, reset entry 0, then mark the entries any game-map cell references
// Grade: DIFFER(ours v retail) -- register assignment: the inner x counter lands in edx (retail esi), so the game-map grid pointer is not carried in edx across the loop back edge and is reloaded at the top of every cell (6 bytes)
struct CHeightMapCell {                           // engine/CHeightMapCell.h: 12 bytes
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char NavFlags;                        // +0x09
    unsigned char _pad0a[2];
};
struct CGameMapGrid {                              // the game map's cell grid (+0x1a7c of CMap)
    unsigned char   _pad[0x10];
    CHeightMapCell* m_pCells;                      // +0x10
    unsigned char   _pad14[0x1f0];
    long            m_width;                       // +0x204 (cells per row minus one)
    long            m_maxY;                        // +0x208 (rows are stored top-down)
    CHeightMapCell& GetCell(long x, long y) { return m_pCells[(m_maxY - y) * (m_width + 1) + x]; }
};
struct SThemePaletteEntry {                        // 8 bytes
    unsigned long ThemeId;
    bool          Used;
    SThemePaletteEntry(unsigned long themeId, bool used) : ThemeId(themeId), Used(used) { }
};
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long i) { return _M_start[i]; }
};
struct CMap {
    unsigned char              _pad[0x1a6c];
    vector<SThemePaletteEntry> m_ThemePalette;     // +0x1a6c
    void*                      m_pQuarterGrid;     // +0x1a78
    CGameMapGrid*              m_pGameMapGrid;     // +0x1a7c
    void ZeroAllUnusedThemePaletteEntries();
};

void CMap::ZeroAllUnusedThemePaletteEntries()
{
    for (SThemePaletteEntry* it = m_ThemePalette._M_start; it != m_ThemePalette._M_finish; ++it)
        it->Used = false;
    m_ThemePalette[0] = SThemePaletteEntry(1, false);
    for (long y = 0; y < m_pGameMapGrid->m_maxY; ++y) {
        for (long x = 0; x < m_pGameMapGrid->m_width; ++x) {
            m_ThemePalette[m_pGameMapGrid->GetCell(x, y).EngineThemePaletteIndex[0]].Used = true;
            m_ThemePalette[m_pGameMapGrid->GetCell(x, y).EngineThemePaletteIndex[1]].Used = true;
            m_ThemePalette[m_pGameMapGrid->GetCell(x, y).EngineThemePaletteIndex[2]].Used = true;
        }
    }
}
