// CMap::Clear() -- clear the game map grid (+0x1a7c, 0x008F6050) and the three words at +0x1d00..+0x1d08
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081E7E0, length 36 bytes.
// Callees: 0x008F6050 (on the +0x1a7c grid; the manifest label "CHeightMap::Clear"). Shape: one zero register after the call, `ret`.
struct CGameMapGrid {
    void Clear();                                  // 0x008F6050
};
class CMap { public:
    unsigned char _pad[0x1a7c];
    CGameMapGrid* m_pGameMapGrid;                  // +0x1a7c
    unsigned char _pad1a80[0x280];
    long          m_1d00;                          // +0x1d00
    long          m_1d04;                          // +0x1d04
    long          m_1d08;                          // +0x1d08
    void Clear();
};

void CMap::Clear()
{
    m_pGameMapGrid->Clear();
    m_1d00 = 0;
    m_1d04 = 0;
    m_1d08 = 0;
}