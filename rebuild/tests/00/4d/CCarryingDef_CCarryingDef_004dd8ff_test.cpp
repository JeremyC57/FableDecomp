// behaviour test for 0x004DD8FF: every member vector of a new definition is empty
#include <cstdio>
struct V { void* s; void* f; void* e; V() : s(0), f(0), e(0) { } };
struct D { V v[1]; };
int main()
{
    D d; int ok = 1;
    for (int i = 0; i < 1; ++i) ok = ok && !d.v[i].s && !d.v[i].f && !d.v[i].e;
    if (!ok) { std::printf("004dd8ff_TEST FAIL\n"); return 1; }
    std::printf("004dd8ff_TEST PASS\n");
    return 0;
}