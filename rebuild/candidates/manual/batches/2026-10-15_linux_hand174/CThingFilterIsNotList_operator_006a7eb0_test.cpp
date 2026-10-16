#include <cstdio>
struct Thing { int id; };
struct IP { Thing* item; Thing* GetPItem() const { return item; } };
struct F { Thing** l1; int n1; IP* l2; int n2;
    bool operator()(const Thing* t) const {
        if (l1) { for (int i = 0; i < n1; ++i) if (l1[i] == t) return false; return true; }
        for (int i = 0; i < n2; ++i) if (l2[i].GetPItem() == t) return false; return true; } };
int main()
{
    Thing a = { 1 }, b = { 2 }, c = { 3 };
    Thing* raw[2] = { &a, &b };
    IP ip[1] = { { &c } };
    F f1 = { raw, 2, 0, 0 };
    F f2 = { 0, 0, ip, 1 };
    bool ok = !f1(&a) && !f1(&b) && f1(&c) && !f2(&c) && f2(&a);
    if (!ok) { std::printf("006a7eb0_TEST FAIL\n"); return 1; }
    std::printf("006a7eb0_TEST PASS\n");
    return 0;
}
