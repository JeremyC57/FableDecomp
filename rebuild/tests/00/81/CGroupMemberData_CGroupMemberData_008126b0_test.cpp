// Behaviour test for 0x008126B0 CGroupMemberData copy constructor.
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
class CGroupMemberData {
public:
    CIntelligentPointer<const CThingAICreature> PMember;
    bool                                        IsActive;
    unsigned long                               JoinedFrame;
    unsigned long                               DeactivatedFrame;
    CGroupMemberData(CBaseObject* p) : PMember(p), IsActive(true), JoinedFrame(100), DeactivatedFrame(250) {}
    CGroupMemberData(const CGroupMemberData& other)
        : PMember(other.PMember), IsActive(other.IsActive), JoinedFrame(other.JoinedFrame), DeactivatedFrame(other.DeactivatedFrame) {}
};

int main()
{
    CBaseObject obj; obj.Id = 2;
    CGroupMemberData a(&obj);
    CBaseIntelligentPointer::s_sets = 0;
    CGroupMemberData b(a);
    if (!(CBaseIntelligentPointer::s_sets == 1 && b.PMember.GetPItem() == &obj && b.IsActive && b.JoinedFrame == 100 && b.DeactivatedFrame == 250)) {
        std::printf("008126b0_TEST FAIL\n");
        return 1;
    }
    std::printf("008126b0_TEST PASS\n");
    return 0;
}