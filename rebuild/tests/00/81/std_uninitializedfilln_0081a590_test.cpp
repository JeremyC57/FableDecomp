// Behaviour test for 0x0081A590 uninitialized_fill_n<pair<unsigned long,CSoundPair>> (model copy).
#include <cstdio>
#include <new>
class CSoundPair { public: int FirstSound; int LastSound; };
struct E {
    unsigned int first; CSoundPair second;
    E(const E& o) : first(o.first), second(o.second) { }
    E(unsigned int k, int a, int b) : first(k) { second.FirstSound = a; second.LastSound = b; }
};
static E* uninitialized_fill_n_model(E* first, unsigned int n, const E& x)
{
    E* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) E(x);
    return cur;
}
int main()
{
    unsigned int raw[3 * 6];
    int i;
    for (i = 0; i < 18; ++i) raw[i] = 0xCDCDCDCDu;
    E x(7u, -3, 99);
    E* base = (E*)raw;
    E* end = uninitialized_fill_n_model(base + 1, 4u, x);
    int ok = (end == base + 5);
    for (i = 1; i < 5; ++i) if (base[i].first != 7u || base[i].second.FirstSound != -3 || base[i].second.LastSound != 99) ok = 0;
    if (raw[0] != 0xCDCDCDCDu || raw[15] != 0xCDCDCDCDu) ok = 0;
    if (uninitialized_fill_n_model(base, 0u, x) != base) ok = 0;
    if (!ok) { std::printf("0081a590_TEST FAIL\n"); return 1; }
    std::printf("0081a590_TEST PASS\n");
    return 0;
}