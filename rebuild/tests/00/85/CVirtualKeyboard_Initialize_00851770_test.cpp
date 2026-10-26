// Behaviour test for 0x00851770 CVirtualKeyboard::Initialize: edit-box set-up sequence, wrong type / missing child
// leave the keyboard untouched; result always 0.
#include <cstdio>
#include <cstring>
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public: char text[40]; CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; } };
class CWideString { public: unsigned short c; CWideString(const unsigned short* t) : c(t[0]) { } };
class CUserProfileManager { public: CWideString GetAutoSaveDisplayName() { static const unsigned short n[2] = { 'P', 0 }; return CWideString(n); } };
static CUserProfileManager g_profiles;
struct CTBaseSingleton_CUserProfileManager { static CUserProfileManager& Get() { return g_profiles; } };
struct CObserver {
    virtual ~CObserver() { }
    virtual void ObserveEvent(long event) { char b[16]; std::sprintf(b, "observe%lx", event); Log(b); }
};
struct CComponent {
    long type; CComponent* child; char lastName[40];
    CComponent() : type(0), child(0) { lastName[0] = 0; }
    virtual ~CComponent() { }
    virtual CComponent* GetChildByName(CCharString name) { std::strcpy(lastName, name.text); return child; }
    virtual long GetComponentType() { return type; }
    virtual void SetText(const CWideString& t) { if (t.c == 'P') Log("text"); }
    virtual void UpdateDisplayText() { Log("update"); }
    virtual void SetActive(bool a) { if (a) Log("active"); }
    virtual void SetForbiddenCharacters(const CWideString& c) { if (c.c == '.') Log("forbid"); }
};
struct CEditBox : CComponent, CObserver { bool DeleteAllOnKeyPress; CEditBox() : DeleteAllOnKeyPress(false) { } };
struct CFrontEndManager {
    CObserver* added;
    static CFrontEndManager* GetInstance() { static CFrontEndManager m; return &m; }
    virtual ~CFrontEndManager() { }
    virtual void AddObserver(CObserver* o) { added = o; Log("add"); }
};
static const unsigned short g_dot[2] = { '.', 0 };
struct CVirtualKeyboard {
    CComponent* PEntryScreen; CEditBox* PEntryBox;
    long Initialize()
    {
        CComponent* box = PEntryScreen->GetChildByName(CCharString("UI_NEW_PROFILE_EDIT_BOX", -1));
        if (box && box->GetComponentType() == 0x25) {
            PEntryBox = static_cast<CEditBox*>(box);
            PEntryBox->SetText(CTBaseSingleton_CUserProfileManager::Get().GetAutoSaveDisplayName());
            PEntryBox->UpdateDisplayText();
            PEntryBox->SetForbiddenCharacters(g_dot);
            PEntryBox->DeleteAllOnKeyPress = true;
            CFrontEndManager::GetInstance()->AddObserver(PEntryBox);
            PEntryBox->ObserveEvent(0x21);
            PEntryBox->ObserveEvent(0x22);
            PEntryBox->SetActive(true);
        }
        return 0;
    }
};
int main()
{
    CComponent screen; CEditBox box; box.type = 0x25; screen.child = &box;
    CVirtualKeyboard k; k.PEntryScreen = &screen; k.PEntryBox = 0;
    g_log[0] = 0;
    bool ok = k.Initialize() == 0 && k.PEntryBox == &box && box.DeleteAllOnKeyPress
        && CFrontEndManager::GetInstance()->added == static_cast<CObserver*>(&box)
        && std::strcmp(screen.lastName, "UI_NEW_PROFILE_EDIT_BOX") == 0
        && std::strcmp(g_log, "text;update;forbid;add;observe21;observe22;active;") == 0;
    box.type = 3; k.PEntryBox = 0; g_log[0] = 0;
    ok = ok && k.Initialize() == 0 && k.PEntryBox == 0 && g_log[0] == 0;
    screen.child = 0;
    ok = ok && k.Initialize() == 0 && k.PEntryBox == 0;
    if (!ok) { std::printf("00851770_TEST FAIL\n"); return 1; }
    std::printf("00851770_TEST PASS\n");
    return 0;
}