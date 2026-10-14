// CMap::GetThemeSizeZAt(const C2DCoordI& pos) -- the theme's minimum camera-size Z offset at pos (0x0081FA20) plus the height of the game map cell there (rows stored top-down: (maxY - y) * (width + 1) + x)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00820100, length 51 bytes.
// Callees: CMap::GetThemeMinCameraSizeZOffsetAt(const C2DCoordI&) @ 0x0081FA20. Shape: the cell index (esi) and the cell table (edi) computed before the call, `fadd dword ptr [edi+eax*4]` after it, `ret 4`.
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
struct CMap {
    unsigned char _pad[0x1a7c];
    CGameMapGrid* m_pGameMapGrid;                  // +0x1a7c
    float GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos);   // 0x0081FA20
    float GetThemeSizeZAt(const C2DCoordI& pos);
};

float CMap::GetThemeSizeZAt(const C2DCoordI& pos)
{
    CGameMapGrid* grid = m_pGameMapGrid;
    long index = (grid->m_maxY - pos.Y) * (grid->m_width + 1) + pos.X;
    CHeightMapCell* cells = grid->m_pCells;
    return GetThemeMinCameraSizeZOffsetAt(pos) + cells[index].Height;
}
