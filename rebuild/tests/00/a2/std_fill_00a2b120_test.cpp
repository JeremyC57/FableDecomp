#include <cstdio>
// Behaviour model: every element ends up sharing the value's block.
struct Info { long rc; };
int main()
{
    Info a = { 1 }, v = { 1 }; Info* elems[3] = { &a, &a, &v };
    a.rc = 3;
    for (int i = 0; i < 3; ++i) if (elems[i] != &v) { --elems[i]->rc; elems[i] = &v; ++v.rc; }
    if (a.rc != 1 || v.rc != 3) { std::printf("00a2b120_TEST FAIL\n"); return 1; }
    std::printf("00a2b120_TEST PASS\n");
    return 0;
}