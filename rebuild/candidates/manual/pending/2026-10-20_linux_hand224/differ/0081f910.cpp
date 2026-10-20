// CMap::GetDominantEngineThemeAt(const C2DCoordI&, long*) -- the most-weighted of a game-map cell's three engine themes
// Grade: DIFFER(ours v retail) -- register assignment: pos/best/remaining in ebx/esi/edi (retail esi/edi/ebx), and the last cell lookups read pos.X/pos.Y as memory operands instead of loading them into edi/esi (6 bytes)
struct C2DCoordI { long X; long Y; };
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
    CHeightMapCell& GetCell(const C2DCoordI& pos) { return m_pCells[(m_maxY - pos.Y) * (m_width + 1) + pos.X]; }
};
struct SThemePaletteEntry {                        // 8 bytes
    unsigned long ThemeId;
    bool          Used;
};
struct CMap {
    unsigned char       _pad[0x1a6c];
    SThemePaletteEntry* m_pThemePalette;           // +0x1a6c (vector begin)
    SThemePaletteEntry* m_pThemePaletteEnd;        // +0x1a70
    SThemePaletteEntry* m_pThemePaletteCapacity;   // +0x1a74
    void*               m_pQuarterGrid;            // +0x1a78
    CGameMapGrid*       m_pGameMapGrid;            // +0x1a7c
    unsigned long GetDominantEngineThemeAt(const C2DCoordI& pos, long* weight);
};

unsigned long CMap::GetDominantEngineThemeAt(const C2DCoordI& pos, long* weight)
{
    long best = 0;
    long bestIndex = -1;
    long blend0 = m_pGameMapGrid->GetCell(pos).EngineBlend[0];
    if (blend0 > 0) {
        bestIndex = 0;
        best = blend0;
    }
    long remaining = 3 - blend0;
    long blend1 = m_pGameMapGrid->GetCell(pos).EngineBlend[1];
    if (blend1 > best) {
        bestIndex = 1;
        best = blend1;
    }
    remaining -= blend1;
    if (remaining > best) {
        *weight = remaining;
        return m_pThemePalette[m_pGameMapGrid->GetCell(pos).EngineThemePaletteIndex[2]].ThemeId;
    }
    *weight = best;
    return m_pThemePalette[m_pGameMapGrid->GetCell(pos).EngineThemePaletteIndex[bestIndex]].ThemeId;
}
