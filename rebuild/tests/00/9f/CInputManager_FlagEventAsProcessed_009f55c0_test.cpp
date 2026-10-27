// behaviour test for 0x009F55C0: the method appends copies in order, growing when full
#include <cstdio>
#include <vector>
struct E { int v; };
struct C { std::vector<E> m; void Add(const E& e) { m.push_back(e); } };
int main()
{
    C c; for (int i = 0; i < 5; ++i) { E e = { i * 3 }; c.Add(e); }
    int ok = c.m.size() == 5;
    for (int i = 0; i < 5; ++i) ok = ok && c.m[i].v == i * 3;
    if (!ok) { std::printf("009f55c0_TEST FAIL\n"); return 1; }
    std::printf("009f55c0_TEST PASS\n");
    return 0;
}