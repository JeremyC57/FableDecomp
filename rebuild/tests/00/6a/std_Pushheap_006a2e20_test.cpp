#include <cstdio>
// Behaviour test for 0x006A2E20: _Push_heap over pair<long, CIntelligentPointer<CThing> > (assignment via
// Get/SetPItem, base destructor on the by-value argument).
class CThing { public: int id; };
static int g_sets = 0, g_dtors = 0;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    ~CBaseIntelligentPointer() { ++g_dtors; }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; ++g_sets; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00() { }
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
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
typedef pair<long, CIntelligentPointer<CThing> > Elem;
int main()
{
    CThing things[8];
    Elem h[10]; int n = 0;
    long keys[8] = { 2, 6, -4, 6, 11, 0, 3, 9 };
    for (int i = 0; i < 8; ++i) {
        Elem v; v.first = keys[i]; v.second.SetPItem(&things[i]);
        int d0 = g_dtors;
        _Push_heap(h, n, 0, v, CKeyPairCompareLess<long, CIntelligentPointer<CThing> >()); ++n;
        if (g_dtors <= d0) { std::printf("006a2e20_TEST FAIL\n"); return 1; }
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { std::printf("006a2e20_TEST FAIL\n"); return 1; }
    }
    if (h[0].first != 11 || h[0].second.GetPItem() != &things[4]) { std::printf("006a2e20_TEST FAIL\n"); return 1; }
    std::printf("006a2e20_TEST PASS\n");
    return 0;
}