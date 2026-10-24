// Behaviour test for 007ca5a0 pair<CIntelligentPointer<CThingObject>, bool>::pair(const T1&, const T2&)
// (copy of the source model; the intelligent-pointer base is a counting stand-in).
#include <cstdio>
class CThing { public: int id; };
class CThingObject : public CThing { };
static int g_ctor = 0, g_get = 0, g_set = 0;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() {}
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctor; }
    CThing* GetPItem() const { ++g_get; return m_pItem; }
    void SetPItem(CThing* item) { ++g_set; m_pItem = item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b);
};
template <class T1, class T2>
pair<T1, T2>::pair(const T1& a, const T2& b) : first(a), second(b) { }

int main()
{
    CThingObject obj; obj.id = 7;
    CIntelligentPointer<CThingObject> p(&obj);
    g_ctor = g_get = g_set = 0;
    bool flag = true;
    pair<CIntelligentPointer<CThingObject>, bool> e(p, flag);
    int ok = 1;
    if (e.first.m_pItem != &obj || e.second != true) ok = 0;
    if (g_ctor != 1 || g_get != 1 || g_set != 1) ok = 0;          // base ctor, GetPItem on the source, SetPItem
    pair<CIntelligentPointer<CThingObject>, bool> f(CIntelligentPointer<CThingObject>(0), false);
    if (f.first.m_pItem != 0 || f.second != false) ok = 0;
    if (!ok) { std::printf("007ca5a0_TEST FAIL\n"); return 1; }
    std::printf("007ca5a0_TEST PASS\n");
    return 0;
}