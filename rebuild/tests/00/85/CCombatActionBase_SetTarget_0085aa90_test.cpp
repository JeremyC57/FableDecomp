#include <cstdio>
// Behaviour test for 0x0085AA90 CCombatActionBase::SetTarget: links against the source object; the intelligent-pointer
// callees are stubbed so the temporary's round trip and the store into PTarget (+0xa8) are observable.
class CBaseObject { };
class CThing : public CBaseObject { public: int Id; };
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
    void SetPItem(CBaseObject* item);
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
class CCombatActionBase {
public:
    void SetTarget(CThing& target);
    unsigned char             _pad[0xa8];
    CIntelligentPointer<CThing> PTarget;
};
static int g_ctor = 0, g_dtor = 0, g_set = 0, g_get = 0;
void CBaseIntelligentPointer::v00() { }
template <> void CIntelligentPointer<CThing>::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { ++g_ctor; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtor; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { ++g_get; return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { ++g_set; m_pItem = item; }
int main()
{
    CCombatActionBase* action = new CCombatActionBase;
    if ((char*)&action->PTarget - (char*)action != 0xa8) { std::printf("0085aa90_TEST FAIL layout\n"); return 1; }
    CThing a, b;
    g_ctor = g_dtor = g_set = g_get = 0;
    action->SetTarget(a);
    if (action->PTarget.m_pItem != &a || g_ctor != 1 || g_dtor != 1 || g_set != 2 || g_get != 1) { std::printf("0085aa90_TEST FAIL first\n"); return 1; }
    action->SetTarget(b);
    if (action->PTarget.m_pItem != &b || g_ctor != 2 || g_dtor != 2 || g_set != 4 || g_get != 2) { std::printf("0085aa90_TEST FAIL second\n"); return 1; }
    std::printf("0085aa90_TEST PASS\n");
    return 0;
}
