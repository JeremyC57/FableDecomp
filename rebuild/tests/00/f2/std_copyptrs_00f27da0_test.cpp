// behaviour test for 0x00F27DA0 std::__copy_ptrs<const CHeroSoulsWaveDef*, CHeroSoulsWaveDef*>: assigns exactly last - first records front to front (base, count and vector each once per record, in order) and returns result + n; an empty range copies nothing and returns result
#include <cstdio>
#include <vector>
static int gBase = 0, gVec = 0;
struct Base { int Tag; Base& operator=(const Base& o) { ++gBase; Tag = o.Tag; return *this; } };
struct Rec : Base { long Count; std::vector<int> Items; };
template <class In, class Out> Out CopyPtrs(In first, In last, Out result)   // the function under test
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++gVec; ++first; ++result; }
    return result;
}
int main()
{
    int fail = 0;
    Rec src[3], dst[4];
    for (int i = 0; i < 3; ++i) { src[i].Tag = 10 + i; src[i].Count = 20 + i; src[i].Items.assign(i + 1, i); }
    for (int i = 0; i < 4; ++i) { dst[i].Tag = -1; dst[i].Count = -1; }
    const Rec* b = src;
    Rec* end = CopyPtrs(b, b + 3, dst);
    if (end != dst + 3) fail |= 1;
    for (int i = 0; i < 3; ++i) if (dst[i].Tag != 10 + i || dst[i].Count != 20 + i || dst[i].Items.size() != (unsigned int)(i + 1)) fail |= 2;
    if (dst[3].Tag != -1 || dst[3].Count != -1) fail |= 4;
    if (gBase != 3 || gVec != 3) fail |= 8;
    if (CopyPtrs(b, b, dst) != dst || gBase != 3) fail |= 16;
    if (fail) { std::printf("00f27da0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00f27da0_TEST PASS\n");
    return 0;
}