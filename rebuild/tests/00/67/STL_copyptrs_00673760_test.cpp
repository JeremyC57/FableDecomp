// Behaviour test for 0x00673760: __copy_ptrs over 44-byte records (counted forward block assignment, returns dest end)
#include <cstdio>

struct Rec { unsigned int Words[11]; };

static Rec* CopyPtrs(const Rec* first, const Rec* last, Rec* result)
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
    Rec src[5], dst[6];
    for (int i = 0; i < 5; ++i) for (int k = 0; k < 11; ++k) src[i].Words[k] = (unsigned int)(i * 100 + k);
    for (int i = 0; i < 6; ++i) for (int k = 0; k < 11; ++k) dst[i].Words[k] = 0xdeadu;
    Rec* e = CopyPtrs(src, src + 5, dst);
    if (e != dst + 5 || dst[5].Words[0] != 0xdeadu) { std::printf("00673760_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) for (int k = 0; k < 11; ++k)
        if (dst[i].Words[k] != (unsigned int)(i * 100 + k)) { std::printf("00673760_TEST FAIL\n"); return 1; }
    if (CopyPtrs(src, src, dst) != dst) { std::printf("00673760_TEST FAIL\n"); return 1; }
    std::printf("00673760_TEST PASS\n");
    return 0;
}