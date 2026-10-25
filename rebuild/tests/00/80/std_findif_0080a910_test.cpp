// Behaviour test for 0x0080a910 std::__find_if(CTargetPoint*, CFilter_Bind2<CThingFilter_IsInFOV, unary_negate<CThingFilter_IsTargetableBehind> >): first point in view not targetable from behind
#include <cstdio>

struct TargetPoint {             // stands in for NPlayerTargeting::CTargetPoint
    int inFOV;
    int flag;                    // 1 when the thing is targetable from behind
};
struct IsInFOV { bool operator()(const TargetPoint& p) const { return p.inFOV != 0; } };
struct Second {                  // unary_negate<CThingFilter_IsTargetableBehind>
    bool operator()(const TargetPoint& p) const { return !(p.flag != 0); }
};
struct Bind2 {                   // CFilter_Bind2: (*F1)(t) && (*F2)(t)
    IsInFOV* F1;
    Second* F2;
    bool operator()(const TargetPoint& t) const { return (*F1)(t) && (*F2)(t); }
};
struct random_access_iterator_tag { };

template <class It, class Pred>
It find_if_ra(It first, It last, Pred pred, const random_access_iterator_tag&)
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}

static unsigned int g_rng = 777u;
static int next_rand(int n) { g_rng = g_rng * 1103515245u + 12345u; return (int)((g_rng >> 8) % (unsigned int)n); }

int main()
{
    IsInFOV f1;
    Second f2;
    Bind2 pred;
    pred.F1 = &f1;
    pred.F2 = &f2;
    random_access_iterator_tag tag;
    for (int round = 0; round < 500; ++round) {
        TargetPoint v[23];
        int n = next_rand(24);
        int i;
        for (i = 0; i < n; ++i) { v[i].inFOV = next_rand(3) != 0; v[i].flag = next_rand(4) == 0; }
        TargetPoint* expect = v + n;
        for (i = 0; i < n; ++i)
            if (v[i].inFOV && !v[i].flag) { expect = v + i; break; }
        if (find_if_ra(v, v + n, pred, tag) != expect) { std::printf("0080a910_TEST FAIL\n"); return 1; }
    }
    std::printf("0080a910_TEST PASS\n");
    return 0;
}