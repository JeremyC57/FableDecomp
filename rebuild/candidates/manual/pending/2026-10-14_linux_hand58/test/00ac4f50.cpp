#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CStream { unsigned long w[8]; int n; void WriteULong(unsigned long v) { w[n++] = v; } };
struct CChunk { void* vt; long a, b, c; unsigned long* begin; unsigned long* end; unsigned long* cap; long d;
    void WriteToFile(CStream& s, long) { unsigned long count = end - begin; s.WriteULong(count); for (unsigned long i = 0; i < count; ++i) s.WriteULong(begin[i]); } };
int main()
{
    unsigned long ix[3] = { 5, 6, 7 }; CChunk ch; ch.begin = ix; ch.end = ix + 3; CStream s; s.n = 0;
    ch.WriteToFile(s, 0);
    if (s.n != 4 || s.w[0] != 3 || s.w[1] != 5 || s.w[2] != 6 || s.w[3] != 7) { std::printf("00ac4f50_TEST FAIL\n"); return 1; }
    ch.end = ix; s.n = 0; ch.WriteToFile(s, 0); if (s.n != 1 || s.w[0] != 0) { std::printf("00ac4f50_TEST FAIL\n"); return 1; }
    std::printf("00ac4f50_TEST PASS\n");
    return 0;
}
