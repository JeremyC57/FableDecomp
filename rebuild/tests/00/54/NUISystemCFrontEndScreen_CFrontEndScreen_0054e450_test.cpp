#include <cstdio>
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
namespace NUISystem {
class CAction;
class CUIInterface {
public:
    virtual ~CUIInterface();
};
class CObserver {
public:
    virtual ~CObserver();
    unsigned char _pad04[0x10];                          // observer body (+0x08..+0x17 of the screen)
};
class CActionParam {
public:
    virtual ~CActionParam();
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    virtual ~CComponent();
};
class CChangingStateComponent : public CComponent {
public:
    virtual ~CChangingStateComponent();                  // 0x0052CCF0
    unsigned char _pad1c[0x160 - 0x1c];                  // component and changing-state members
};
class CFrontEndScreen : public CChangingStateComponent {
public:
    CCountedPointer<CAction> PLeftClickAction;           // +0x160
    virtual ~CFrontEndScreen();
};
CFrontEndScreen::~CFrontEndScreen()
{
}
}

static int g_base, g_del; static void* g_delData;
namespace NUISystem {
CUIInterface::~CUIInterface() {}
CObserver::~CObserver() {}
CActionParam::~CActionParam() {}
CComponent::~CComponent() {}
CChangingStateComponent::~CChangingStateComponent() { ++g_base; }
}
static void __fastcall Del(void* d) { ++g_del; g_delData = d; }
int main()
{
    using namespace NUISystem;
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 2; info->DeleteFunc = Del; info->Data = (void*)0x1234;
    CFrontEndScreen* a = new CFrontEndScreen; a->PLeftClickAction.Data = (CAction*)0x1234; a->PLeftClickAction.Info = info;
    CFrontEndScreen* b = new CFrontEndScreen; b->PLeftClickAction.Data = (CAction*)0x1234; b->PLeftClickAction.Info = info;
    CFrontEndScreen* c = new CFrontEndScreen; c->PLeftClickAction.Data = 0; c->PLeftClickAction.Info = 0;
    delete a;
    bool ok = g_base == 1 && g_del == 0 && info->RefCount == 1;
    CUIInterface* bi = b; delete bi;                       // through the base: virtual
    ok = ok && g_base == 2 && g_del == 1 && g_delData == (void*)0x1234;
    ok = ok && (char*)&c->PLeftClickAction - (char*)c == 0x160;
    delete c;
    ok = ok && g_base == 3 && g_del == 1;
    std::printf(ok ? "0054e450_TEST PASS\n" : "0054e450_TEST FAIL\n");
    return ok ? 0 : 1;
}