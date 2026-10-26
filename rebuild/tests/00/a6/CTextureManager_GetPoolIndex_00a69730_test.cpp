#include <cstdio>
class CTextureManager { public:
    unsigned int PoolCount; unsigned int PreallocSizes[16];
    long GetPoolIndex(unsigned int size) const {
        unsigned int count = PoolCount; long first = 0; long len = count;
        while (len > 0) { long half = len / 2; long mid = first + half; if (size == PreallocSizes[mid]) return mid; if (size > PreallocSizes[mid]) { first = mid + 1; len = len - half - 1; } else len = half; }
        if ((unsigned int)first < count) return first;
        return -1;
    }
};
int main()
{
    CTextureManager m = { 5, { 16, 64, 256, 1024, 4096 } };
    if (m.GetPoolIndex(256) != 2 || m.GetPoolIndex(100) != 2 || m.GetPoolIndex(1) != 0 || m.GetPoolIndex(4096) != 4 || m.GetPoolIndex(5000) != -1) { std::printf("00a69730_TEST FAIL\n"); return 1; }
    CTextureManager e = { 0, { 0 } };
    if (e.GetPoolIndex(5) != -1) { std::printf("00a69730_TEST FAIL\n"); return 1; }
    std::printf("00a69730_TEST PASS\n");
    return 0;
}