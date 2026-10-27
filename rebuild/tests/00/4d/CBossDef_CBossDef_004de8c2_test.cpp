// behaviour test for 0x004DE8C2: every member vector of a new definition is empty
#include <cstdio>
struct V { void* s; void* f; void* e; V() : s(0), f(0), e(0) { } };
struct D { V v[3]; };
int main()
{
    D d; int ok = 1;
    for (int i = 0; i < 3; ++i) ok = ok && !d.v[i].s && !d.v[i].f && !d.v[i].e;
    if (!ok) { std::printf("004de8c2_TEST FAIL\n"); return 1; }
    std::printf("004de8c2_TEST PASS\n");
    return 0;
}