// behaviour test for 0x00BDB670 (CVertexShaderConstant): STLport's forward copy of 16-byte records copies n = last - first records front to
// back (so an overlapping move down is safe) and returns the end of the destination range
#include <cstdio>
#include <cstring>
struct Rec { int Words[4]; };
static Rec* CopyForward(const Rec* first, const Rec* last, Rec* result)
{
    for (int n = (int)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
int main()
{
    int fails = 0;
    Rec r[6];
    std::memset(r, 0, sizeof(r));
    for (int i = 0; i < 6; ++i) for (int w = 0; w < 4; ++w) r[i].Words[w] = i * 100 + w;
    Rec* res = CopyForward(r + 2, r + 5, r + 1);          // move records 2..4 down one slot (overlapping)
    if (res != r + 4) fails |= 1;
    if (r[1].Words[0] != 200 || r[2].Words[3] != 303 || r[3].Words[0] != 400 || r[4].Words[0] != 400) fails |= 2;
    if (CopyForward(r, r, r + 3) != r + 3) fails |= 4;    // empty range
    if (sizeof(Rec) != 16) fails |= 8;
    if (fails) { std::printf("00bdb670_TEST FAIL %d\n", fails); return 1; }
    std::printf("00bdb670_TEST PASS\n");
    return 0;
}