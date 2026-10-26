// Behaviour test for 0x006B78E0 std::fill(CGameMapCell*, CGameMapCell*, const CGameMapCell&).
#include <cstdio>
class CGameMapCell { public:
    unsigned char AtmosThemes[3];
    unsigned char AtmosBlends[2];
    signed char   SoundTheme;
    unsigned char Music;
    CGameMapCell() { }
};
static void SetCell(CGameMapCell& c, int k) { c.AtmosThemes[0] = (unsigned char)k; c.AtmosThemes[1] = (unsigned char)(k + 1); c.AtmosThemes[2] = (unsigned char)(k + 2); c.AtmosBlends[0] = (unsigned char)(k + 3); c.AtmosBlends[1] = (unsigned char)(k + 4); c.SoundTheme = (signed char)-k; c.Music = (unsigned char)(k * 2); }
static int IsCell(const CGameMapCell& c, int k) { return c.AtmosThemes[0] == (unsigned char)k && c.AtmosThemes[1] == (unsigned char)(k + 1) && c.AtmosThemes[2] == (unsigned char)(k + 2) && c.AtmosBlends[0] == (unsigned char)(k + 3) && c.AtmosBlends[1] == (unsigned char)(k + 4) && c.SoundTheme == (signed char)-k && c.Music == (unsigned char)(k * 2); }
template <class ForwardIter, class T>
void Fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
int main()
{
    CGameMapCell cells[5];
    for (int i = 0; i < 5; ++i) SetCell(cells[i], 10 * i);
    CGameMapCell v; SetCell(v, 99);
    Fill(cells + 1, cells + 4, v);
    int ok = sizeof(CGameMapCell) == 7 && IsCell(cells[0], 0) && IsCell(cells[1], 99) && IsCell(cells[2], 99) && IsCell(cells[3], 99) && IsCell(cells[4], 40);
    Fill(cells, cells, v);
    if (!IsCell(cells[0], 0)) ok = 0;
    if (!ok) { std::printf("006b78e0_TEST FAIL\n"); return 1; }
    std::printf("006b78e0_TEST PASS\n");
    return 0;
}