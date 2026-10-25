// behaviour test for 0x00AB13D0 (N3DAnimationBuilder::CSequenceInfo): STLport's backward copy of 52-byte records copies n = last - first records
// from the top down (so an overlapping move up by k slots is safe) and returns the new start of the destination
#include <cstdio>
#include <cstring>
struct Rec { int Words[13]; };
static Rec* CopyBackward(Rec* first, Rec* last, Rec* result)
{
    for (int n = (int)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
int main()
{
    int fails = 0;
    Rec r[6];
    std::memset(r, 0, sizeof(r));
    for (int i = 0; i < 6; ++i) for (int w = 0; w < 13; ++w) r[i].Words[w] = i * 100 + w;
    Rec* res = CopyBackward(r + 1, r + 4, r + 5);        // move records 1..3 up one slot (overlapping)
    if (res != r + 2) fails |= 1;
    if (r[2].Words[0] != 100 || r[3].Words[13 - 1] != 200 + 13 - 1 || r[4].Words[0] != 300) fails |= 2;
    if (r[1].Words[0] != 100 || r[5].Words[0] != 500) fails |= 4;
    if (CopyBackward(r + 3, r + 3, r + 6) != r + 6) fails |= 8;   // empty range: result unchanged
    if (sizeof(Rec) != 52) fails |= 16;
    if (fails) { std::printf("00ab13d0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00ab13d0_TEST PASS\n");
    return 0;
}