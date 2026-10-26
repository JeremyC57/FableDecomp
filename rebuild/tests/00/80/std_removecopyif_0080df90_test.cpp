#include <cstdio>
// Behaviour test for 0x0080DF90: STLport remove_copy_if over pair<CIntelligentPointer<CThing>, long> with a
// by-value predicate holding the target pointer.
class CThing { public: int id; };
static int g_dtors = 0;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    ~CBaseIntelligentPointer() { ++g_dtors; }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00() { }
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
template<class K, class V> struct pair { K first; V second; };
typedef pair<CIntelligentPointer<CThing>, long> Elem;
struct CIsFirstElementEqualTo {
    CIntelligentPointer<CThing> m_Target;
    bool operator()(const Elem& e) const { return e.first.GetPItem() == m_Target.GetPItem(); }
};
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for ( ; __first != __last; ++__first)
        if (!__pred(*__first)) { *__result = *__first; ++__result; }
    return __result;
}
int main()
{
    CThing t[3];
    Elem v[6];
    CThing* who[6] = { &t[0], &t[1], &t[2], &t[1], &t[0], &t[1] };
    for (int i = 0; i < 6; ++i) { v[i].first.SetPItem(who[i]); v[i].second = 10 * i; }
    CIsFirstElementEqualTo pred; pred.m_Target.SetPItem(&t[1]);
    int d0 = g_dtors;
    Elem* e = remove_copy_if(v + 2, v + 6, v + 1, pred);    // as remove_if after find_if hit v[1]
    if (g_dtors <= d0) { std::printf("0080df90_TEST FAIL\n"); return 1; }   // by-value predicate destroyed
    if (e != v + 3) { std::printf("0080df90_TEST FAIL\n"); return 1; }
    if (v[0].first.GetPItem() != &t[0] || v[1].first.GetPItem() != &t[2] || v[1].second != 20 || v[2].first.GetPItem() != &t[0] || v[2].second != 40) {
        std::printf("0080df90_TEST FAIL\n"); return 1;
    }
    std::printf("0080df90_TEST PASS\n");
    return 0;
}