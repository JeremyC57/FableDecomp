// Behaviour test for 0x0080DE90 pair<CIntelligentPointer<CThing>, long> converting constructor from pair<CIntelligentPointer<CThing>, int>.
#include <cstdio>
#include <cstring>

class CBaseObject {
public:
    int Id;
};
class CBaseIntelligentPointer {
public:
    CBaseObject* PData;
    static int s_sets;
    CBaseIntelligentPointer() : PData(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; ++s_sets; }
};
int CBaseIntelligentPointer::s_sets = 0;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    explicit CIntelligentPointer(CBaseObject* p) { SetPItem(p); }
    CIntelligentPointer(const CIntelligentPointer<T>& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
};
class CThing;
class CThingAICreature;
namespace stlp {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& p) : first(p.first), second(p.second) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
}

int main()
{
    CBaseObject obj; obj.Id = 7;
    stlp::pair<CIntelligentPointer<CThing>, int> a(CIntelligentPointer<CThing>(&obj), -42);
    CBaseIntelligentPointer::s_sets = 0;
    stlp::pair<CIntelligentPointer<CThing>, long> b(a);
    if (!(CBaseIntelligentPointer::s_sets == 1 && b.first.GetPItem() == &obj && b.second == -42L)) {
        std::printf("0080de90_TEST FAIL\n");
        return 1;
    }
    std::printf("0080de90_TEST PASS\n");
    return 0;
}