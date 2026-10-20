// Behaviour test for 0x00B27EE0 CEngine::GetScratchZBufferD24S8 (model copy).
#include <cstdio>

static long HighestSetBit(unsigned int value)
{
    if (value == 0) return -1;
    if (value >= 0x80000000u) return 31;
    ++value;
    long r = ((int)((1 << 16) - value) >> 31) & 16;
    r += ((int)((1 << (r + 8)) - value) >> 31) & 8;
    r += ((int)((1 << (r + 4)) - value) >> 31) & 4;
    r += ((int)((1 << (r + 2)) - value) >> 31) & 2;
    r += ((int)((1 << (r + 1)) - value) >> 31) & 1;
    return r;
}
struct CSurf { long w, h, fmt; CSurf() : w(0), h(0), fmt(0) {} void Init(long ww, long hh, long f) { w = ww; h = hh; fmt = f; } };
struct CEng {
    CSurf* cache[32]; unsigned int count;
    CEng() : count(0) { for (int i = 0; i < 32; ++i) cache[i] = 0; }
    CSurf* Get(unsigned int size)
    {
        unsigned int level = (unsigned int)HighestSetBit(size);
        if (level >= count) count = level + 1;
        if (cache[level] == 0) { cache[level] = new CSurf; cache[level]->Init(size, size, 0x4b); }
        return cache[level];
    }
};

int main()
{
    if (HighestSetBit(1) != 0 || HighestSetBit(2) != 1 || HighestSetBit(255) != 7 || HighestSetBit(256) != 8 ||
        HighestSetBit(0x80000000u) != 31) { std::printf("00b27ee0_TEST FAIL\n"); return 1; }
    CEng e;
    CSurf* a = e.Get(256); CSurf* b = e.Get(300); CSurf* c = e.Get(512);
    if (a != b || a == c || a->w != 256 || a->fmt != 0x4b || c->w != 512 || e.count != 10)
    { std::printf("00b27ee0_TEST FAIL\n"); return 1; }
    delete a; delete c;
    std::printf("00b27ee0_TEST PASS\n");
    return 0;
}
