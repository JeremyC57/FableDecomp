// CMap::IsCameraPassableAt(const C2DCoordI& pos) -- the camera-passable bit (bit 3 of the flag byte at +9) of the game map cell at pos (rows stored top-down: (maxY - y) * (width + 1) + x), after a virtual call on the engine globals (0x013B86A0, slot 9)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081F460, length 64 bytes.
// Callees: the engine globals' virtual slot 9 (+0x24, result unused). Shape: the row count's `inc esi`, the 12-byte cell index through `lea edx,[eax+eax*2]`, the bitfield read `mov al ; shr al,3 ; and al,1`, `ret 4`.
class CHeightMapCell { public:                           // engine/CHeightMapCell.h: 12 bytes
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
class C2DCoordI { public: long X; long Y; };
struct CEngineGlobals {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008();
    virtual void Lock();   // slot 9 (+0x24)
};
extern CEngineGlobals* g_pEngineGlobals_013b86a0;   // 0x013B86A0
class CMap { public:
    unsigned char _pad[0x1a7c];
    CGameMapGrid* m_pGameMapGrid;                  // +0x1a7c
    bool IsCameraPassableAt(const C2DCoordI& pos);
};

bool CMap::IsCameraPassableAt(const C2DCoordI& pos)
{
    g_pEngineGlobals_013b86a0->Lock();
    CGameMapGrid* grid = m_pGameMapGrid;
    return grid->m_pCells[(grid->m_maxY - pos.Y) * (grid->m_width + 1) + pos.X].CameraPassable;
}