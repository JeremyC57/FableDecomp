// CMap::GetMemoryFootprint() -- the game map grid's footprint (+0x1a7c, 0x008F5EE0) plus the quarter grid's (+0x1a78, 0x006B74B0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081E7B0, length 33 bytes.
// Callees: 0x008F5EE0 (on the +0x1a7c grid), 0x006B74B0 (on the +0x1a78 grid). Shape: the first result kept in edi, `add eax,edi`, `ret`.
struct CGameMapGrid {
    long GetMemoryFootprint();                     // 0x008F5EE0
};
struct CQuarterGrid {
    long GetMemoryFootprint();                     // 0x006B74B0
};
struct CMap {
    unsigned char _pad[0x1a78];
    CQuarterGrid* m_pQuarterGrid;                  // +0x1a78
    CGameMapGrid* m_pGameMapGrid;                  // +0x1a7c
    long GetMemoryFootprint();
};

long CMap::GetMemoryFootprint()
{
    long footprint = m_pGameMapGrid->GetMemoryFootprint();
    return m_pQuarterGrid->GetMemoryFootprint() + footprint;
}
