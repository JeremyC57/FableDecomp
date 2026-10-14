// CGameMap::Clear() -- resets every cell: atmos themes and blends zeroed, first blend to full (0xff), sound theme -1, music 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7510, length 55 bytes.
// Callees: none. Layout: retail std::vector<CGameMapCell> at +4 is 12 bytes (begin +4, end +8), the PDB's is 16; CGameMapCell is the
// 7-byte PDB record {AtmosThemes[3], AtmosBlends[2], SoundTheme, Music}.
// Shape: `for (cell = begin; cell != end; ++cell)` with end re-read each iteration (the byte stores may alias it); the cell
// reset is an inlined CGameMapCell::Clear: the two arrays zeroed by memset (word+byte, word), then the explicit stores; -1 for
// the signed SoundTheme is hoisted into dl, the unsigned 0xff blend stays an immediate.
#include <string.h>

struct CGameMapCell {
    unsigned char AtmosThemes[3];                   // +0
    unsigned char AtmosBlends[2];                   // +3
    signed char   SoundTheme;                       // +5
    unsigned char Music;                            // +6
    void Clear()
    {
        memset(AtmosThemes, 0, sizeof(AtmosThemes));
        memset(AtmosBlends, 0, sizeof(AtmosBlends));
        AtmosThemes[0] = 0;
        AtmosBlends[0] = 0xff;
        SoundTheme = -1;
        Music = 0;
    }
};

struct CGameMap {
    void* __vftable;                                // +0x00
    CGameMapCell* m_pCellsBegin;                    // +0x04 vector<CGameMapCell> (retail 12-byte vector)
    CGameMapCell* m_pCellsEnd;                      // +0x08
    CGameMapCell* m_pCellsEndOfStorage;             // +0x0c
    void Clear();
};

void CGameMap::Clear()
{
    for (CGameMapCell* cell = m_pCellsBegin; cell != m_pCellsEnd; ++cell)
        cell->Clear();
}