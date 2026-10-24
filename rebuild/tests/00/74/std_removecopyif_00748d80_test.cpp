#include <cstdio>
// Behaviour test for 0x00748D80: STLport remove_copy_if over pair<CIntelligentPointer<const CThing>,
// CDangerStatus>, dropping entries whose thing is null, dead (flag bit) or whose status is zero.
struct CThing { unsigned char Dead : 1; };
struct CBaseIntelligentPointer {
    virtual void v00() { }
    const CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    const CThing* GetPItem() const { return m_pItem; }
    void SetPItem(const CThing* item) { m_pItem = item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
enum EDangerStatus { DANGER_STATUS_NONE, DANGER_STATUS_SEEN, DANGER_STATUS_ALERT };
struct CDangerStatus { EDangerStatus DangerStatus; long Timer; };
template<class K, class V> struct pair { K first; V second; };
typedef pair<CIntelligentPointer<const CThing>, CDangerStatus> Elem;
struct CDeadDangerThing {
    bool operator()(const Elem& e) const
    {
        return e.first.GetPItem() == 0 || e.first.GetPItem()->Dead || e.second.DangerStatus == DANGER_STATUS_NONE;
    }
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
    CThing t[4];
    for (int i = 0; i < 4; ++i) t[i].Dead = 0;
    t[2].Dead = 1;
    Elem v[6];
    const CThing* who[6] = { &t[0], 0, &t[1], &t[2], &t[3], &t[0] };
    EDangerStatus st[6] = { DANGER_STATUS_SEEN, DANGER_STATUS_SEEN, DANGER_STATUS_ALERT, DANGER_STATUS_ALERT, DANGER_STATUS_NONE, DANGER_STATUS_ALERT };
    for (int i = 0; i < 6; ++i) { v[i].first.SetPItem(who[i]); v[i].second.DangerStatus = st[i]; v[i].second.Timer = 100 + i; }
    // in place from the first removable entry (index 1), as remove_if calls it
    Elem* e = remove_copy_if(v + 2, v + 6, v + 1, CDeadDangerThing());
    if (e != v + 3) { std::printf("00748d80_TEST FAIL\n"); return 1; }
    if (v[0].first.GetPItem() != &t[0] || v[1].first.GetPItem() != &t[1] || v[1].second.DangerStatus != DANGER_STATUS_ALERT || v[1].second.Timer != 102
        || v[2].first.GetPItem() != &t[0] || v[2].second.Timer != 105) {
        std::printf("00748d80_TEST FAIL\n"); return 1;
    }
    if (remove_copy_if(v, v, v, CDeadDangerThing()) != v) { std::printf("00748d80_TEST FAIL\n"); return 1; }
    std::printf("00748d80_TEST PASS\n");
    return 0;
}