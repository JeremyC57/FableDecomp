// Behaviour test for 0x006B79A0 std::__copy(const CGameMapCell*, const CGameMapCell*, CGameMapCell*, random_access_iterator_tag, int*).
#include <cstdio>
struct CGameMapCell {
    unsigned char AtmosThemes[3];
    unsigned char AtmosBlends[2];
    signed char   SoundTheme;
    unsigned char Music;
    CGameMapCell() { }
};
static void SetCell(CGameMapCell& c, int k) { c.AtmosThemes[0] = (unsigned char)k; c.AtmosThemes[1] = (unsigned char)(k + 1); c.AtmosThemes[2] = (unsigned char)(k + 2); c.AtmosBlends[0] = (unsigned char)(k + 3); c.AtmosBlends[1] = (unsigned char)(k + 4); c.SoundTheme = (signed char)-k; c.Music = (unsigned char)(k * 2); }
static int IsCell(const CGameMapCell& c, int k) { return c.AtmosThemes[0] == (unsigned char)k && c.AtmosThemes[1] == (unsigned char)(k + 1) && c.AtmosThemes[2] == (unsigned char)(k + 2) && c.AtmosBlends[0] == (unsigned char)(k + 3) && c.AtmosBlends[1] == (unsigned char)(k + 4) && c.SoundTheme == (signed char)-k && c.Music == (unsigned char)(k * 2); }
struct RandomAccessTag { RandomAccessTag() { } };
template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter Copy(RandomAccessIter first, RandomAccessIter last, OutputIter result, const RandomAccessTag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
int main()
{
    CGameMapCell src[3], dst[5];
    for (int i = 0; i < 3; ++i) SetCell(src[i], 1 + i);
    for (int i = 0; i < 5; ++i) SetCell(dst[i], 50);
    CGameMapCell* end = Copy((const CGameMapCell*)src, (const CGameMapCell*)src + 3, dst + 1, RandomAccessTag(), (int*)0);
    int ok = end == dst + 4 && IsCell(dst[0], 50) && IsCell(dst[1], 1) && IsCell(dst[2], 2) && IsCell(dst[3], 3) && IsCell(dst[4], 50);
    // erase-style overlapping move down by one
    end = Copy((const CGameMapCell*)dst + 2, (const CGameMapCell*)dst + 5, dst + 1, RandomAccessTag(), (int*)0);
    if (end != dst + 4 || !IsCell(dst[1], 2) || !IsCell(dst[2], 3) || !IsCell(dst[3], 50)) ok = 0;
    if (Copy((const CGameMapCell*)src, (const CGameMapCell*)src, dst, RandomAccessTag(), (int*)0) != dst) ok = 0;
    if (!ok) { std::printf("006b79a0_TEST FAIL\n"); return 1; }
    std::printf("006b79a0_TEST PASS\n");
    return 0;
}