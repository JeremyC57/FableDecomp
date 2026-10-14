// CMap::DrawGetEngineBlendAt(long x, long y, long which) -- blend byte `which` (+7) of the game map cell at (x, y) (rows stored top-down: (maxY - y) * (width + 1) + x)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F75F0, length 47 bytes.
// Shape: the 12-byte cell address through `lea eax,[eax+eax*2] ; lea edx,[ecx+eax*4]`, `mov al, byte ptr [edx+eax+7]`, `ret 0Ch`.
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
    unsigned char DrawGetEngineBlendAt(long x, long y, long which);
};

unsigned char CMap::DrawGetEngineBlendAt(long x, long y, long which)
{
    CGameMapGrid* grid = m_pGameMapGrid;
    return grid->m_pCells[(grid->m_maxY - y) * (grid->m_width + 1) + x].EngineBlend[which];
}