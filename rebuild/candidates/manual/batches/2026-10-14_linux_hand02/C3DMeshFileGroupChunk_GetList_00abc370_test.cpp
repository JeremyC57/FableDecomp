#include <cstdio>

struct Vec8 { void* b; void* e; void* c; };
struct Chunk { unsigned char pad[0x30]; Vec8 v;
    void* GetList() { Vec8* pv = &v; if ((unsigned)(((char*)pv->e - (char*)pv->b) >> 3) > 0) return pv->b; return pv; } };
int main()
{
    Chunk c; long data[4]; c.v.b = data; c.v.e = data; c.v.c = data + 4;
    if (c.GetList() != &c.v) { std::printf("00abc370_TEST FAIL\n"); return 1; }
    c.v.e = data + 2;
    if (c.GetList() != data) { std::printf("00abc370_TEST FAIL\n"); return 1; }
    std::printf("00abc370_TEST PASS\n");
    return 0;
}
