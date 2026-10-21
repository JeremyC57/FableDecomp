#include <cstdio>
namespace NUISystem {
class CAction;
struct CCPPointerInfo;
}
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
namespace NUISystem {
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
class CComponent;
class CUIInterface {
public:
    virtual ~CUIInterface();
    virtual void _slot01();
    virtual void _slot02();
    virtual CComponent* FindChild(CCharString name);     // slot 3 (+0x0c)
};
class CObserver {
public:
    virtual ~CObserver();
    unsigned char _body[0x10];
};
class CActionParam {
public:
    virtual ~CActionParam();
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    unsigned char _body1c[0x30 - 0x1c];                  // +0x1c
    float         Time;                                  // +0x30
    unsigned char _body34[0x15c - 0x34];                 // +0x34
};
class CChangingStateComponent : public CComponent {
public:
    virtual void Initialise();                           // 0x0052C730 for this class
};
class CFrontEndScreen : public CChangingStateComponent {
public:
    float                     m_fLastInputTime;          // +0x15c
    CCountedPointer<CAction>  PLeftClickAction;          // +0x160
    CComponent*               PAcceptButton;             // +0x168
    virtual void Initialise();
};
void __fastcall SetUpChildrenForAcceptButton(CComponent* component, CComponent* accept_button);   // 0x0054E350 (placeholder name)
void CFrontEndScreen::Initialise()
{
    CChangingStateComponent::Initialise();
    m_fLastInputTime = Time;
    PAcceptButton = FindChild(CCharString("UI_ACCEPT"));
    if (PAcceptButton)
        SetUpChildrenForAcceptButton(this, PAcceptButton);
}
}

static int g_base = 0, g_helper = 0; static NUISystem::CComponent* g_found = 0; static bool g_name_ok = false;
CCharString::CCharString(const char* text, long) { PStringData = (CCharStringData*)text; }
CCharString::CCharString(const CCharString& o) : PStringData(o.PStringData) {}
CCharString::~CCharString() {}
namespace NUISystem {
CUIInterface::~CUIInterface() {}
void CUIInterface::_slot01() {} void CUIInterface::_slot02() {}
CComponent* CUIInterface::FindChild(CCharString name) { g_name_ok = name.PStringData && ((const char*)name.PStringData)[3] == 'A'; return g_found; }
CObserver::~CObserver() {}
CActionParam::~CActionParam() {}
void CChangingStateComponent::Initialise() { ++g_base; }
void __fastcall SetUpChildrenForAcceptButton(CComponent* component, CComponent* accept_button) { if (accept_button == g_found) ++g_helper; }
}
int main()
{
    using namespace NUISystem;
    static CFrontEndScreen s; static CFrontEndScreen child;
    s.Time = 2.5f; g_found = &child;
    s.Initialise();
    bool ok = g_base == 1 && s.m_fLastInputTime == 2.5f && s.PAcceptButton == &child && g_helper == 1 && g_name_ok;
    g_found = 0; s.Initialise();
    ok = ok && g_base == 2 && s.PAcceptButton == 0 && g_helper == 1;
    if (ok) std::printf("0054e4b0_TEST PASS\n");
    return 0;
}