// Behaviour test for 0x006B7CC0 STLport uninitialized_fill_n over CGameMapCell.
#include <cstdio>
#include <new>
struct CGameMapCell {
    unsigned char AtmosThemes[3];
    unsigned char AtmosBlends[2];
    signed char   SoundTheme;
    unsigned char Music;
    CGameMapCell() { }
};
static void SetCell(CGameMapCell& c, int k) { c.AtmosThemes[0] = (unsigned char)k; c.AtmosThemes[1] = (unsigned char)(k + 1); c.AtmosThemes[2] = (unsigned char)(k + 2); c.AtmosBlends[0] = (unsigned char)(k + 3); c.AtmosBlends[1] = (unsigned char)(k + 4); c.SoundTheme = (signed char)-k; c.Music = (unsigned char)(k * 2); }
static int IsCell(const CGameMapCell& c, int k) { return c.AtmosThemes[0] == (unsigned char)k && c.AtmosThemes[1] == (unsigned char)(k + 1) && c.AtmosThemes[2] == (unsigned char)(k + 2) && c.AtmosBlends[0] == (unsigned char)(k + 3) && c.AtmosBlends[1] == (unsigned char)(k + 4) && c.SoundTheme == (signed char)-k && c.Music == (unsigned char)(k * 2); }
struct FalseType { FalseType() { } };
template <class ForwardIter, class Size, class T>
ForwardIter UninitFillNAux(ForwardIter first, Size n, const T& x, const FalseType&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template <class ForwardIter, class Size, class T>
ForwardIter UninitFillN(ForwardIter first, Size n, const T& x)
{
    return UninitFillNAux(first, n, x, FalseType());
}
static CGameMapCell* Run(CGameMapCell* p, unsigned int n, const CGameMapCell& x) { return UninitFillN(p, n, x); }
int main()
{
    CGameMapCell buf[6];
    for (int i = 0; i < 6; ++i) SetCell(buf[i], 7);
    CGameMapCell x; SetCell(x, 33);
    CGameMapCell* end = Run(buf + 1, 4u, x);
    int ok = end == buf + 5 && IsCell(buf[0], 7) && IsCell(buf[1], 33) && IsCell(buf[4], 33) && IsCell(buf[5], 7);
    if (Run(buf, 0u, x) != buf || !IsCell(buf[0], 7)) ok = 0;
    if (!ok) { std::printf("006b7cc0_TEST FAIL\n"); return 1; }
    std::printf("006b7cc0_TEST PASS\n");
    return 0;
}