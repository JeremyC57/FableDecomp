// Behaviour test for 0x004CC0D0: STLport bitset<274>::reset(pos) -- range-checked reset of one bit; retail's out-of-range path is
// puts("bitset") + abort(), which the test stands in for with a counter so the boundary can be checked.
#include <cstdio>

static int gRangeErrors = 0;
static void TestOutOfRange() { ++gRangeErrors; }

namespace std {
template <unsigned int _Nb>
class bitset {
public:
    enum { _Words = (_Nb + 31) / 32 };
    unsigned int _M_w[_Words];                          // 32-bit words (retail unsigned long)
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static unsigned int _S_maskbit(unsigned int __pos) { return 1u << _S_whichbit(__pos); }
    unsigned int& _M_getword(unsigned int __pos) { return _M_w[_S_whichword(__pos)]; }
    bitset<_Nb>& _Unchecked_set(unsigned int __pos)
    {
        _M_getword(__pos) |= _S_maskbit(__pos);
        return *this;
    }
    bitset<_Nb>& _Unchecked_reset(unsigned int __pos)
    {
        _M_getword(__pos) &= ~_S_maskbit(__pos);
        return *this;
    }
    bitset<_Nb>& set(unsigned int __pos)
    {
        if (__pos >= _Nb) {
            TestOutOfRange();                           // retail: puts("bitset"); abort();
            return *this;
        }
        return _Unchecked_set(__pos);
    }
    bitset<_Nb>& reset(unsigned int __pos)
    {
        if (__pos >= _Nb) {
            TestOutOfRange();
            return *this;
        }
        return _Unchecked_reset(__pos);
    }
};
}

int main()
{
    std::bitset<274> b;
    unsigned int fill = ("reset"[0] == 's') ? 0u : 0xFFFFFFFFu;
    for (int i = 0; i < 9; ++i)
        b._M_w[i] = fill;
    static const unsigned int pos[5] = { 0, 31, 32, 200, 273 };
    bool ok = true;
    for (int k = 0; k < 5; ++k)
        if (&b.reset(pos[k]) != &b)
            ok = false;
    for (unsigned int p = 0; ok && p < 9 * 32; ++p) {
        bool hit = p == 0 || p == 31 || p == 32 || p == 200 || p == 273;
        bool bit = ((b._M_w[p / 32] >> (p % 32)) & 1u) != 0;
        bool want = (fill != 0) ? !hit : hit;
        if (bit != want)
            ok = false;
    }
    ok = ok && gRangeErrors == 0;
    b.reset(274);                                         // first out-of-range position
    ok = ok && gRangeErrors == 1;
    if (!ok) {
        std::printf("004cc0d0_TEST FAIL\n");
        return 1;
    }
    std::printf("004cc0d0_TEST PASS\n");
    return 0;
}