// Behaviour test for 0x00748570 pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> copy constructor.
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
namespace NStealth {
enum EDangerStatus { DANGER_STATUS_NONE, DANGER_STATUS_SUSPICIOUS, DANGER_STATUS_ALERT };
class CDangerStatus {
public:
    EDangerStatus DangerStatus;
    long          Timer;
    CDangerStatus(EDangerStatus status) : DangerStatus(status), Timer(0) {}
};
}
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
    CBaseObject obj; obj.Id = 3;
    NStealth::CDangerStatus st(NStealth::DANGER_STATUS_ALERT); st.Timer = 90;
    stlp::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> a(CIntelligentPointer<const CThing>(&obj), st);
    CBaseIntelligentPointer::s_sets = 0;
    stlp::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> b(a);
    if (!(CBaseIntelligentPointer::s_sets == 1 && b.first.GetPItem() == &obj && b.second.DangerStatus == NStealth::DANGER_STATUS_ALERT && b.second.Timer == 90)) {
        std::printf("00748570_TEST FAIL\n");
        return 1;
    }
    std::printf("00748570_TEST PASS\n");
    return 0;
}