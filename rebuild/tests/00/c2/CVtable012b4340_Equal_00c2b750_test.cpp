#include <cstdio>

// Behaviour model of CVtable_012b4340::Equal (0x00C2B750): float-key comparator returning an int.
struct Item { void* _00; float m_key; };
struct Cmp { int F(const Item* a, const Item* b) const { return a->m_key == b->m_key ? 1 : 0; } };

int main()
{

    Cmp c;
    Item x; x.m_key = 1.5f; Item y; y.m_key = 2.5f; Item z; z.m_key = 1.5f;
    int xy = c.F(&x, &y), yx = c.F(&y, &x), xz = c.F(&x, &z);
    if (xy != 0 || yx != 0 || xz != 1) { std::printf("00c2b750_TEST FAIL\n"); return 1; }
    std::printf("00c2b750_TEST PASS\n");
    return 0;
}