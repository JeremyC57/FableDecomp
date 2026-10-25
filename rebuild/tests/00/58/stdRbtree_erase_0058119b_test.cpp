// behaviour test for 0x0058119b std::_Rb_tree::erase(const key_type&) (map<void*, float>): the count of keys equal to k is the
// distance across equal_range(k), that range is erased, and the count is returned (0 and nothing erased when absent)
#include <cstdio>
struct Tree {
    int keys[8]; int n; int erasedFirst, erasedLast;
    void equal_range(int k, int& lo, int& hi) const
    {
        lo = 0; while (lo < n && keys[lo] < k) ++lo;
        hi = lo; while (hi < n && !(k < keys[hi])) ++hi;
    }
    void erase(int lo, int hi)
    {
        erasedFirst = lo; erasedLast = hi;
        int w = lo; for (int r = hi; r < n; ++r) keys[w++] = keys[r];
        n = w;
    }
    unsigned int erase_key(int k)
    {
        int lo, hi; equal_range(k, lo, hi);
        unsigned int count = 0; for (int it = lo; it != hi; ++it) ++count;
        erase(lo, hi);
        return count;
    }
};
int main()
{
    int fails = 0;
    Tree t; int init[5] = { 1, 3, 3, 3, 7 };
    for (int i = 0; i < 5; ++i) t.keys[i] = init[i];
    t.n = 5;
    if (t.erase_key(3) != 3u || t.n != 2 || t.keys[0] != 1 || t.keys[1] != 7) fails |= 1;
    if (t.erase_key(5) != 0u || t.n != 2 || t.erasedFirst != t.erasedLast) fails |= 2;
    if (t.erase_key(7) != 1u || t.n != 1) fails |= 4;
    if (fails) { std::printf("0058119b_TEST FAIL %d\n", fails); return 1; }
    std::printf("0058119b_TEST PASS\n");
    return 0;
}