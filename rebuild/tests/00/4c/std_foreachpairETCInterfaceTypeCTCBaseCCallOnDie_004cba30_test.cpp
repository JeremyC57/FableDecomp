// Behaviour test for 0x004CBA30 for_each<pair<ETCInterfaceType, CTCBase*>*, CCallOnDie>(first, last, f): every component
// in [first, last) gets its OnDie call exactly once, in order; an empty range calls nothing; the functor comes back.
#include <cstdio>

static int g_order[8]; static int g_n = 0;
class CTCBase { public: int id; virtual void OnDie() { g_order[g_n++] = id; } };
struct Pair { long first; CTCBase* second; Pair(const Pair& o) : first(o.first), second(o.second) {} Pair(long f, CTCBase* s) : first(f), second(s) {} };
class CCallOnDie { public: void operator()(Pair p) const { p.second->OnDie(); } };

template <class I, class F> F ForEach(I first, I last, F f) { for (; first != last; ++first) f(*first); return f; }

int main()
{
    CTCBase a, b, c; a.id = 1; b.id = 2; c.id = 3;
    Pair comps[3] = { Pair(10, &a), Pair(20, &b), Pair(30, &c) };
    ForEach(comps, comps + 3, CCallOnDie());
    if (g_n != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3) { std::printf("004cba30_TEST FAIL\n"); return 1; }
    ForEach(comps, comps, CCallOnDie());
    if (g_n != 3) { std::printf("004cba30_TEST FAIL\n"); return 1; }
    std::printf("004cba30_TEST PASS\n");
    return 0;
}