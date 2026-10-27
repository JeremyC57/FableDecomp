// behaviour test for 00c03eb0 (standalone copy of the model)
#include <cstdio>

typedef unsigned long uLong;
uLong compressBound_(uLong sourceLen) { return sourceLen + (sourceLen >> 12) + (sourceLen >> 14) + 11; }

int main()
{
    if (!(compressBound_(0) == 11 && compressBound_(4096) == 4096 + 1 + 0 + 11 && compressBound_(65536) == 65536 + 16 + 4 + 11)) { std::printf("00c03eb0_TEST FAIL\n"); return 1; }
    std::printf("00c03eb0_TEST PASS\n");
    return 0;
}