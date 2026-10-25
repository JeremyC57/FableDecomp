#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour test for 00f3b1d0: __uninitialized_copy of 24-byte CRangeCompressor::CScriptEntry records -- every field copied,
// returns result + (last - first), an empty range returns result untouched.
struct Entry { unsigned int Flags; int BitCount; unsigned int RangeBias; int RangeShift; unsigned int ShuffleMask; unsigned int OrMask; };
static Entry* uninit_copy(Entry* first, Entry* last, Entry* result)
{
    Entry* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) Entry(*first);
    return cur;
}
int main()
{
    Entry src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].Flags = 1u + i; src[i].BitCount = 8 + i; src[i].RangeBias = 100u + i;
        src[i].RangeShift = -i; src[i].ShuffleMask = 0xF0u + i; src[i].OrMask = 0x0Fu * i;
    }
    Entry* dst = (Entry*)std::malloc(sizeof(Entry) * 3);
    Entry* end = uninit_copy(src, src + 3, dst);
    int ok = end == dst + 3;
    for (int j = 0; j < 3; ++j)
        ok = ok && dst[j].Flags == 1u + j && dst[j].BitCount == 8 + j && dst[j].RangeBias == 100u + j
                && dst[j].RangeShift == -j && dst[j].ShuffleMask == 0xF0u + j && dst[j].OrMask == 0x0Fu * j;
    ok = ok && uninit_copy(src, src, dst) == dst;
    std::free(dst);
    if (!ok) { std::printf("00f3b1d0_TEST FAIL\n"); return 1; }
    std::printf("00f3b1d0_TEST PASS\n");
    return 0;
}