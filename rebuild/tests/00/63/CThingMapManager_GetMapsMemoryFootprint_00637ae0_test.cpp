#include <cstdio>

struct Map { long size; long GetMemoryFootprint() { return size; } };
struct Info { Map* PMap; };
struct M { void* vt; void* w; unsigned char tm[12]; Info** First; Info** Last;
    long Get() { long t = 0; for (Info** it = First; it != Last; ++it) t += (*it)->PMap->GetMemoryFootprint(); return t; } };
int main()
{
    Map m1, m2; m1.size = 10; m2.size = 32; Info i1, i2; i1.PMap = &m1; i2.PMap = &m2; Info* arr[2] = { &i1, &i2 };
    M m; m.First = arr; m.Last = arr + 2; if (m.Get() != 42) { std::printf("00637ae0_TEST FAIL\n"); return 1; }
    m.Last = arr; if (m.Get() != 0) { std::printf("00637ae0_TEST FAIL\n"); return 1; }
    std::printf("00637ae0_TEST PASS\n");
    return 0;
}