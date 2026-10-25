#include <cstdio>

// Host model of STLport bitset<274>::test(pos) (in-range path): word pos / 32, mask 1 << (pos % 32), result != 0.
// The out-of-range path (pos >= 274: puts("bitset"); abort()) is not exercised.
struct CUsingComponents {
    enum { _Nb = 274, _Words = (_Nb + 31) / 32 };
    unsigned int _M_w[_Words];
    void set(unsigned int pos) { _M_w[pos / 32] |= 1u << (pos % 32); }
    bool test(unsigned int pos) const { return (_M_w[pos / 32] & (1u << (pos % 32))) != 0; }
};

int main()
{
    CUsingComponents b;
    for (int i = 0; i < CUsingComponents::_Words; ++i) b._M_w[i] = 0;
    if (CUsingComponents::_Words != 9) { std::printf("00410de0_TEST FAIL\n"); return 1; }
    const unsigned int on[6] = { 0, 4, 31, 32, 0x8F, 273 };
    for (int i = 0; i < 6; ++i) b.set(on[i]);
    for (unsigned int pos = 0; pos < 274; ++pos) {
        bool want = false;
        for (int i = 0; i < 6; ++i) if (on[i] == pos) want = true;
        if (b.test(pos) != want) { std::printf("00410de0_TEST FAIL\n"); return 1; }
    }
    if (b._M_w[8] != 0x00020000u) { std::printf("00410de0_TEST FAIL\n"); return 1; }   // bit 273 = word 8, bit 17
    std::printf("00410de0_TEST PASS\n");
    return 0;
}