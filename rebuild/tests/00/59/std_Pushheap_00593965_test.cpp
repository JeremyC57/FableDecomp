#include <cstdio>
// Behaviour test for 0x00593965: _Push_heap over pair<long, CDefPointer<const CContextSensitiveItemDef> >
// with a member-functor priority compare, out-of-line pair assignment and destructor.
class CContextSensitiveItemDef { public: long Priority; };
static int g_assigns = 0, g_dtors = 0;
template<class T> class CDefPointer { public: T* Def; };
template<class K, class V> struct pair {
    K first;
    V second;
    pair() { }
    pair(const pair& o) : first(o.first), second(o.second) { }
    pair& operator=(const pair& o) { first = o.first; second = o.second; ++g_assigns; return *this; }
    ~pair() { ++g_dtors; }
};
typedef pair<long, CDefPointer<const CContextSensitiveItemDef> > Elem;
class CContextSensitivePriorityCompare { public:
    bool operator()(const Elem& a, const Elem& b) const { return a.second.Def->Priority < b.second.Def->Priority; }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
int main()
{
    CContextSensitiveItemDef defs[8];
    long pr[8] = { 5, 1, 9, 3, 9, 12, 0, 7 };
    for (int i = 0; i < 8; ++i) defs[i].Priority = pr[i];
    Elem h[10]; int n = 0;
    for (int i = 0; i < 8; ++i) {
        Elem v; v.first = 100 + i; v.second.Def = &defs[i];
        int d0 = g_dtors;
        _Push_heap(h, n, 0, v, CContextSensitivePriorityCompare()); ++n;
        if (g_dtors <= d0) { std::printf("00593965_TEST FAIL\n"); return 1; }
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].second.Def->Priority < h[c].second.Def->Priority) { std::printf("00593965_TEST FAIL\n"); return 1; }
    }
    if (h[0].first != 105 || h[0].second.Def != &defs[5] || g_assigns < 8) { std::printf("00593965_TEST FAIL\n"); return 1; }
    std::printf("00593965_TEST PASS\n");
    return 0;
}