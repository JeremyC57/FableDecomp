#include <cstdio>
// Behaviour copy of STLport __copy_backward_ptrs<CPrizeScoreDef>: *--result = *--last for (last - first) elements, returns
// the new result start; safe for an overlapping shift towards higher addresses.
static int g_baseAssigns = 0;
struct SBase { long index; SBase& operator=(const SBase& o) { index = o.index; ++g_baseAssigns; return *this; } };
struct SPrize : SBase {
    float Score; long Mult;
    SPrize& operator=(const SPrize& o) { SBase::operator=(o); Score = o.Score; Mult = o.Mult; return *this; }
};
static SPrize* CopyBackward(const SPrize* first, const SPrize* last, SPrize* result)
{
    for (int n = int(last - first); n > 0; --n) *--result = *--last;
    return result;
}
int main()
{
    bool ok = true;
    SPrize v[5];
    for (int i = 0; i < 5; ++i) { v[i].index = i; v[i].Score = 0.5f * i; v[i].Mult = 10 * i; }
    SPrize* r = CopyBackward(v + 1, v + 4, v + 5);          // shift elements 1..3 up by one (overlapping)
    if (r != v + 2 || g_baseAssigns != 3) ok = false;
    const long expect[5] = { 0, 1, 1, 2, 3 };
    for (int i = 0; ok && i < 5; ++i) if (v[i].index != expect[i] || v[i].Mult != 10 * expect[i] || v[i].Score != 0.5f * expect[i]) ok = false;
    if (CopyBackward(v, v, v + 3) != v + 3 || g_baseAssigns != 3) ok = false;   // empty range
    if (!ok) { std::printf("004edcc2_TEST FAIL\n"); return 1; }
    std::printf("004edcc2_TEST PASS\n");
    return 0;
}