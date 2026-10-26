#include <cstdio>
// Behaviour test for 0x00737ad0: STLport remove_copy over 8-byte intelligent pointers compared by GetPItem().
class CThing { public: int id; };
struct CBaseIntelligentPointer {
    virtual void v00() { }
    const CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    const CThing* GetPItem() const { return m_pItem; }
    void SetPItem(const CThing* item) { m_pItem = item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00() { }
    CIntelligentPointer() { }
    CIntelligentPointer(const CThing* t) { SetPItem(t); }
    CIntelligentPointer(const CIntelligentPointer& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    bool operator==(const CIntelligentPointer& other) const { return GetPItem() == other.GetPItem(); }
};
template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter remove_copy(_InputIter __first, _InputIter __last, _OutputIter __result, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        if (!(*__first == __val)) { *__result = *__first; ++__result; }
    return __result;
}
typedef CIntelligentPointer<const CThing> CPtr;
int main()
{
    CThing t[4];
    CPtr v[7];
    const CThing* src[7] = { &t[0], 0, &t[1], 0, &t[2], &t[3], 0 };
    for (int i = 0; i < 7; ++i) v[i].SetPItem(src[i]);
    CPtr nul((const CThing*)0);
    // in place, as remove() calls it (from the first hit: index 1)
    CPtr* e = remove_copy(v + 2, v + 7, v + 1, nul);
    if (e != v + 4) { std::printf("00737ad0_TEST FAIL\n"); return 1; }
    const CThing* want[4] = { &t[0], &t[1], &t[2], &t[3] };
    for (int i = 0; i < 4; ++i) if (v[i].GetPItem() != want[i]) { std::printf("00737ad0_TEST FAIL\n"); return 1; }
    CPtr none[1];
    if (remove_copy(none, none, none, nul) != none) { std::printf("00737ad0_TEST FAIL\n"); return 1; }
    std::printf("00737ad0_TEST PASS\n");
    return 0;
}