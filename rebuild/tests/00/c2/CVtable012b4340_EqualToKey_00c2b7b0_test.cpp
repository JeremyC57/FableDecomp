#include <cstdio>

// Behaviour model of CVtable_012b4340::EqualToKey (0x00C2B7B0): float-key comparator returning an int.
struct Item { void* _00; float m_key; };
struct Cmp { int F(const float& key, const Item* b) const { return key == b->m_key ? 1 : 0; } };

int main()
{

    Cmp c;
    Item y; y.m_key = 2.5f; Item z; z.m_key = 1.5f;
    float k = 1.5f;
    int xy = c.F(k, &y), xz = c.F(k, &z); float k2 = 2.5f; int yx = c.F(k2, &z);
    if (xy != 0 || yx != 0 || xz != 1) { std::printf("00c2b7b0_TEST FAIL\n"); return 1; }
    std::printf("00c2b7b0_TEST PASS\n");
    return 0;
}