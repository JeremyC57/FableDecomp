// CScriptedMapBrush::GetTheme(long x, long y, unsigned long layer) const -- theme index of a layer in the brush cell at (x, y), 0 outside the game area
// Leaf: ?GetTheme@CScriptedMapBrush@@QBEKJJK@Z
// Retail: Fable.exe @ 0x0082E330, 77 bytes, `ret 0Ch`; one caller 0x0082068F; Ego_r.pdb ?GetTheme@CScriptedMapBrush@@QBEKJJK@Z.
// Body: (helper names IsInside / Width are descriptive) x in [GameArea.TLX, BRX) and y in [TLY, BRY) (signed), else 0; cell = (y - TLY) * (BRX - TLX) + x - TLX;
// Cells[cell].Theme[layer].
// Layout: retail CScriptedMapBrush -- Cells (12-byte STLport vector<CScriptedMapCell>) +0x08, GameArea (C2DBoxI) +0x38
// (Ego_r.pdb +0x48 with 16-byte containers); CScriptedMapCell (Ego_r.pdb, 20 bytes): Height +0, Theme[3] +4, ThemeBlend[2] +0x10.
// Shape: /O2 (16-aligned entry); the bounds test is an inline C2DBoxI::IsInside guarding the lookup (the box fields are
// re-read after it), the column offset bracketed as (x - TLX).
class CScriptedMapCell {
public:
    float         Height;                                // +0x00
    unsigned long Theme[3];                              // +0x04
    unsigned char ThemeBlend[2];                         // +0x10
};
class C2DBoxI {
public:
    bool IsInside(long x, long y) const { return x >= TLX && x < BRX && y >= TLY && y < BRY; }
    long Width() const { return BRX - TLX; }
    long TLX;                                            // +0x00
    long TLY;                                            // +0x04
    long BRX;                                            // +0x08
    long BRY;                                            // +0x0c
};
template <class T> class CVector {                       // STLport vector, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
class CScriptedMapBrush {
public:
    void*                     _vfptr;                    // +0x00
    long                      _pad04;                    // +0x04
    CVector<CScriptedMapCell> Cells;                     // +0x08
    unsigned char             _pad14[0x38 - 0x14];       // +0x14 ThingsToDelete / ThingsToMove / ThingsToAdd (12-byte STLport set/map each)
    C2DBoxI                   GameArea;                  // +0x38
    unsigned long GetTheme(long x, long y, unsigned long layer) const;
};
unsigned long CScriptedMapBrush::GetTheme(long x, long y, unsigned long layer) const
{
    if (GameArea.IsInside(x, y))
        return Cells[(y - GameArea.TLY) * GameArea.Width() + (x - GameArea.TLX)].Theme[layer];
    return 0;
}