// Behaviour test for CVirtualKeyboard::~CVirtualKeyboard (0x00851720): the entry box is deactivated, stops observing every event, and is
// removed from the UI manager's observers, in that order; the profile-name string member is destroyed afterwards.
#include <cstdio>
static int gOrder, gSetActiveAt, gIgnoreAt, gRemoveAt, gStringDtorAt; static bool gActiveArg; static void* gRemoved;
class CObserver { public:
    virtual ~CObserver() { }
    virtual void IgnoreAllEvents() { gIgnoreAt = ++gOrder; }
};
class CUIInterface { public: virtual ~CUIInterface() { } };
class CComponent : public CUIInterface, public CObserver { };
class CEditBox : public CComponent { public:
    virtual void SetActive(bool active) { gSetActiveAt = ++gOrder; gActiveArg = active; }
};
class CManager { public:
    virtual void RemoveObserver(CObserver* observer) { gRemoveAt = ++gOrder; gRemoved = observer; }
    static CManager* GetInstance() { static CManager m; return &m; }
};
class CWideString { public: ~CWideString() { gStringDtorAt = ++gOrder; } };
class CVirtualKeyboard { public:
    CWideString ProfileName; bool Canceled, Done; void* PEntryScreen; CEditBox* PEntryBox;
    ~CVirtualKeyboard();
};
CVirtualKeyboard::~CVirtualKeyboard()
{
    PEntryBox->SetActive(false);
    PEntryBox->IgnoreAllEvents();
    CManager::GetInstance()->RemoveObserver(PEntryBox);
}
int main()
{
    CEditBox box;
    { CVirtualKeyboard kb; kb.PEntryBox = &box; gActiveArg = true; }
    if (gSetActiveAt != 1 || gActiveArg || gIgnoreAt != 2 || gRemoveAt != 3 || gRemoved != static_cast<CObserver*>(&box)
        || (void*)gRemoved == (void*)&box || gStringDtorAt != 4) { std::printf("00851720_TEST FAIL\n"); return 1; }
    std::printf("00851720_TEST PASS\n");
    return 0;
}