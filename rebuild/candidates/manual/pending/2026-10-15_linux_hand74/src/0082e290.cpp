// CScriptedMapBrush::GetGameHeight(long x, long y) -- height of the brush cell at game-map (x, y), 0.0f outside the brush's game area
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082E290, 74 bytes (`?GetGameHeight@CScriptedMapBrush@@QBEMJJ@Z`, Ego_r.pdb).
// No callees; 0.0f is the float pool constant at 0x0122DEDC.
// Retail layout (the engine header CScriptedMapBrush.h is the PDB layout, 0x10 bytes higher from
// ThingsToDelete on: the PDB containers are 16 bytes, the retail VC6-era vector/set/map 12): +0 vptr,
// +8 Cells (CScriptedMapCell* first), +0x14 ThingsToDelete, +0x20 ThingsToMove, +0x2C ThingsToAdd,
// +0x38 C2DBoxI GameArea {TLX, TLY, BRX, BRY}. CScriptedMapCell is 0x14 bytes: float Height +0,
// unsigned long Theme[3] +4, unsigned char ThemeBlend[4] +0x10 (index arithmetic `lea eax,[eax+eax*4]`).
// The members are modelled locally with the header's names because the header's offsets disagree.
//
// Shape (calibrated on the sibling GetThemeBlend 0x0082E3D0, which reproduces byte-exact the same way):
// the bounds test is an inline C2DBoxI member (`Contains(x, y)`) and the row width an inline `Width()`;
// with the box members tested directly VC7.1 caches TLX/TLY in registers and pushes ebx, where retail
// compares against memory, re-reads TLY for the subtraction and copies TLX (`mov esi,edi ; sub eax,esi`).

struct C2DBoxI {
    long TLX;                                // +0x00
    long TLY;                                // +0x04
    long BRX;                                // +0x08
    long BRY;                                // +0x0C
    bool Contains(long x, long y) const { return x >= TLX && x < BRX && y >= TLY && y < BRY; }
    long Width() const { return BRX - TLX; }
};

struct CScriptedMapCell {
    float Height;                            // +0x00
    unsigned long Theme[3];                  // +0x04
    unsigned char ThemeBlend[4];             // +0x10
};

struct CScriptedMapBrush {
    void* __vftable;                         // +0x00
    unsigned char _pad_0x04[0x4];            // +0x04
    CScriptedMapCell* Cells;                 // +0x08 vector<CScriptedMapCell> first
    unsigned char _pad_0x0c[0x2c];           // +0x0C Cells last/end, ThingsToDelete, ThingsToMove, ThingsToAdd
    C2DBoxI GameArea;                        // +0x38
    float GetGameHeight(long x, long y) const;
};

float CScriptedMapBrush::GetGameHeight(long x, long y) const
{
    if (GameArea.Contains(x, y))
        return Cells[(y - GameArea.TLY) * GameArea.Width() - GameArea.TLX + x].Height;
    return 0.0f;
}
