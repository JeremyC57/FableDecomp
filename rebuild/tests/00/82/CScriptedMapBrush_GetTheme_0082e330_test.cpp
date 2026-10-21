#include <cstdio>
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


#include <cstring>
int main()
{
    static CScriptedMapBrush b; std::memset(&b, 0, sizeof b);
    static CScriptedMapCell cells[6]; std::memset(cells, 0, sizeof cells);
    for (int i = 0; i < 6; ++i) for (int l = 0; l < 3; ++l) cells[i].Theme[l] = 100 * i + l + 1;
    b.Cells._M_start = cells; b.Cells._M_finish = cells + 6;
    b.GameArea.TLX = 10; b.GameArea.TLY = 20; b.GameArea.BRX = 13; b.GameArea.BRY = 22;
    bool ok = b.GetTheme(10, 20, 0) == 1 && b.GetTheme(12, 21, 2) == 503 && b.GetTheme(11, 21, 1) == 402
        && b.GetTheme(9, 20, 0) == 0 && b.GetTheme(13, 20, 0) == 0 && b.GetTheme(10, 19, 0) == 0 && b.GetTheme(10, 22, 0) == 0;
    if (ok) std::printf("0082e330_TEST PASS\n");
    return 0;
}