#include <cstdio>

struct S { void* vt; unsigned long Pos; unsigned long Size; unsigned char* Ptr; unsigned long ChunkPos; unsigned long Rem;
    void Set(unsigned long p) { long d = p - Pos; Ptr += d; Rem -= d; Pos = p; } };
int main()
{
    unsigned char buf[64]; S s; s.Pos = 10; s.Ptr = buf + 10; s.Rem = 54; s.Set(20);
    if (s.Pos != 20 || s.Ptr != buf + 20 || s.Rem != 44) { std::printf("00993c10_TEST FAIL\n"); return 1; }
    std::printf("00993c10_TEST PASS\n");
    return 0;
}