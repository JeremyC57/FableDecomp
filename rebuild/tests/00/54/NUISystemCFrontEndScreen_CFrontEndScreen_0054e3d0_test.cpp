#include <cstdio>
namespace NUISystem {
class CAction;
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};
class CUIInterface {
public:
    virtual ~CUIInterface();
};
class CObserver {
public:
    virtual ~CObserver();
    unsigned char _body[0x10];                           // +0x08 .. +0x18 of the component
};
class CActionParam {
public:
    virtual ~CActionParam();
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    unsigned char _body[0x15c - 0x1c];                   // +0x1c
};
class CChangingStateComponent : public CComponent {
public:
    CChangingStateComponent(long id);                    // 0x0052CC50
};
class CFrontEndScreen : public CChangingStateComponent {
public:
    CFrontEndScreen(long id);
    virtual ~CFrontEndScreen();
    float                     m_fLastInputTime;          // +0x15c
    CCountedPointer<CAction>  PLeftClickAction;          // +0x160
    CComponent*               PAcceptButton;             // +0x168
};
CFrontEndScreen::CFrontEndScreen(long id)
    : CChangingStateComponent(id), PAcceptButton(0)
{
}
}

#include <new>
#include <cstring>
#include <cstddef>
static long g_id = -1;
namespace NUISystem {
CUIInterface::~CUIInterface() {}
CObserver::~CObserver() {}
CActionParam::~CActionParam() {}
CChangingStateComponent::CChangingStateComponent(long id) { g_id = id; }
CFrontEndScreen::~CFrontEndScreen() {}
}
int main()
{
    using namespace NUISystem;
    static unsigned char buf[sizeof(CFrontEndScreen)];
    std::memset(buf, 0xcd, sizeof buf);
    CFrontEndScreen* s = new (buf) CFrontEndScreen(7);
    bool ok = g_id == 7 && s->PLeftClickAction.Data == 0 && s->PLeftClickAction.Info == 0 && s->PAcceptButton == 0
        && (unsigned char*)&s->PAcceptButton - buf == 0x168
        && (unsigned char*)static_cast<CObserver*>(s) - buf == 4 && (unsigned char*)static_cast<CActionParam*>(s) - buf == 0x18;
    if (ok) std::printf("0054e3d0_TEST PASS\n");
    return 0;
}