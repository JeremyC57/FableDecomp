// CMap::DrawGetEngineThemeAt(long x, long y, long which) -- the engine theme id of palette entry `which` (+4) of the game map cell at (x, y) (rows stored top-down: (maxY - y) * (width + 1) + x), looked up in the 8-byte palette at +0x1a6c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F75B0, length 61 bytes.
// Shape: the 12-byte cell address through `lea eax,[eax+eax*2] ; lea eax,[edx+eax*4]`, `movzx eax, byte ptr [eax+edx+4]`, `mov eax,[ecx+eax*8]`, `ret 0Ch`. Grid layout as the landed CMap::GetEngineThemeAt @ 0x0081F090.
struct CHeightMapCell {                           // engine/CHeightMapCell.h: 12 bytes
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char NavFlags0 : 1;                   // +0x09 bitfield group
    unsigned char NavFlags1 : 1;
    unsigned char NavFlags2 : 1;
    unsigned char CameraPassable : 1;
    unsigned char NavFlags4 : 4;
    unsigned char _pad0a[2];
};
struct CGameMapGrid {                              // the game map's cell grid (+0x1a7c of CMap)
    unsigned char   _pad[0x10];
    CHeightMapCell* m_pCells;                      // +0x10
    unsigned char   _pad14[0x1f0];
    long            m_width;                       // +0x204 (cells per row minus one)
    long            m_maxY;                        // +0x208 (rows are stored top-down)
};
struct C2DCoordI { long X; long Y; };
struct SThemePaletteEntry { unsigned long m_themeId; unsigned long m_reserved; };
struct CQuarterGrid {                              // the quarter-resolution sound/theme grid (+0x1a78 of CMap)
    unsigned char  _pad[4];
    unsigned char* m_pData;                        // +0x04 (7-byte entries)
    unsigned char  _pad08[0x14];
    long           m_width;                        // +0x1c
    long           m_height;                       // +0x20
};
struct CMap {
    unsigned char       _pad[0x1a6c];
    SThemePaletteEntry* m_pPalette;                // +0x1a6c
    unsigned char       _pad1a70[0xc];
    CGameMapGrid*       m_pGameMapGrid;            // +0x1a7c
    unsigned long DrawGetEngineThemeAt(long x, long y, long which);
};

unsigned long CMap::DrawGetEngineThemeAt(long x, long y, long which)
{
    CGameMapGrid* grid = m_pGameMapGrid;
    return m_pPalette[grid->m_pCells[(grid->m_maxY - y) * (grid->m_width + 1) + x].EngineThemePaletteIndex[which]].m_themeId;
}
