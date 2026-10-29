// Behaviour test for NUISystem::CEditBox::~CEditBox @ 0x005408F0: the edit box removes its CObserver subobject from the UI
// manager singleton, then its members are destroyed (the three wide strings, the escape and enter action lists) and the
// CChangingStateComponent base destructor runs last.
#include <stdio.h>
static int g_order[16]; static int g_n = 0;
static void* g_removed = 0; static int g_removeCalls = 0;
class CWideString { public: int Id; ~CWideString() { g_order[g_n++] = Id; } };
class CActionList { public: int Id; ~CActionList() { g_order[g_n++] = Id; } };
namespace NUISystem {
class CObserver { public: virtual ~CObserver() { } int PreventObservation; };
class CManager { public:
    void RemoveObserver(CObserver* observer) { g_removeCalls++; g_removed = observer; }
    static CManager* GetInstance() { static CManager instance; return &instance; }
};
class CUIInterface { public: virtual ~CUIInterface() { } };
class CChangingStateComponent : public CUIInterface, public CObserver { public:
    virtual ~CChangingStateComponent() { g_order[g_n++] = 99; }
};
class CEditBox : public CChangingStateComponent { public:
    CActionList ActionsOnEnter;
    CActionList ActionsOnEscape;
    CWideString Text;
    int         CursorPos;
    CWideString AllowedCharacters;
    CWideString ForbiddenCharacters;
    virtual ~CEditBox();
};
CEditBox::~CEditBox()
{
    CManager::GetInstance()->RemoveObserver(this);
}
}
static int fail(int line) { printf("005408f0_TEST FAIL\nline %d\n", line); return 1; }
int main()
{
    NUISystem::CEditBox* box = new NUISystem::CEditBox;
    box->ActionsOnEnter.Id = 1; box->ActionsOnEscape.Id = 2; box->Text.Id = 3;
    box->AllowedCharacters.Id = 4; box->ForbiddenCharacters.Id = 5;
    NUISystem::CObserver* asObserver = box;
    NUISystem::CUIInterface* base = box;
    delete base;
    if (g_removeCalls != 1 || g_removed != (void*)asObserver) return fail(__LINE__);
    if (g_n != 6 || g_order[0] != 5 || g_order[1] != 4 || g_order[2] != 3 || g_order[3] != 2 || g_order[4] != 1 ||
        g_order[5] != 99) return fail(__LINE__);
    printf("005408f0_TEST PASS\n");
    return 0;
}