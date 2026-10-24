// Behaviour test for 0x0080DF20 (model copy): pair<CIntelligentPointer<CThing>,long> built from (pointer, value).
#include <cstdio>
class CBaseObject { public: int id; };
class CThing : public CBaseObject {};
static int g_setCalls = 0;
class CBaseIntelligentPointer {
public:
    virtual void v00() {}
    CBaseIntelligentPointer() : PData(0) {}
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { ++g_setCalls; PData = item; }
    CBaseObject* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    virtual void v00() {}
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
namespace model {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template <class T1, class T2> inline pair<T1, T2> make_pair(const T1& x, const T2& y) { return pair<T1, T2>(x, y); }
}
int main()
{
    CThing thing;
    CIntelligentPointer<CThing> p;
    p.PData = &thing;
    long v = 42;
    model::pair<CIntelligentPointer<CThing>, long> r(p, v);
    if (r.first.GetPItem() != &thing || r.second != 42 || g_setCalls < 1 || p.GetPItem() != &thing) { std::printf("0080df20_TEST FAIL\n"); return 1; }
    std::printf("0080df20_TEST PASS\n");
    return 0;
}