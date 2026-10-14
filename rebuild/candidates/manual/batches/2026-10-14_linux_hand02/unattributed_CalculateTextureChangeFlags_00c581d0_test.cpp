#include <cstdio>

struct T { unsigned long ids[7]; unsigned long n; long calls; unsigned long last;
    virtual void Process(void*, unsigned long id, unsigned long idx) { ++calls; last = id * 100 + idx; }
    void Calc(void* c, const unsigned short* ix) { for (unsigned long i = 0; i < n; ++i) Process(c, ids[i], ix[i]); } };
int main()
{
    T t; t.n = 3; t.calls = 0; t.ids[0] = 1; t.ids[1] = 2; t.ids[2] = 3; unsigned short ix[3] = { 7, 8, 9 };
    t.Calc(0, ix); if (t.calls != 3 || t.last != 309) { std::printf("00c581d0_TEST FAIL\n"); return 1; }
    t.n = 0; t.Calc(0, ix); if (t.calls != 3) { std::printf("00c581d0_TEST FAIL\n"); return 1; }
    std::printf("00c581d0_TEST PASS\n");
    return 0;
}
