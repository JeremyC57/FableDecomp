#include <cstdio>

struct E { E* sub[2]; unsigned long Sizeof() const { unsigned long a = sub[0] ? sub[0]->Sizeof() + 4 : 4; unsigned long b = sub[1] ? sub[1]->Sizeof() + 4 : 4; return b + a; } };
int main()
{
    E leaf = { { 0, 0 } }, mid = { { &leaf, 0 } }, root = { { &mid, &leaf } };
    if (leaf.Sizeof() != 8 || mid.Sizeof() != 16 || root.Sizeof() != 32) { std::printf("00ac2930_TEST FAIL\n"); return 1; }
    std::printf("00ac2930_TEST PASS\n");
    return 0;
}
