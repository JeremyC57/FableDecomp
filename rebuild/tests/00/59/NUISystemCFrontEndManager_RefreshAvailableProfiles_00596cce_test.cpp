// Behaviour test for NUISystem::CFrontEndManager::RefreshAvailableProfiles (0x00596CCE): the profile menu's old list
// (if any) is told to go away, a fresh list is created and added, a "new profile" button (action 0x125, y 0) comes
// first, then one button per profile at y = 50, 80, 110 ... whose action is 0x124 when the profile's signature
// verifies and 0xDB otherwise, carrying the profile name; the menu is re-initialised and the profile count returned.
// Every counted object is released by the end.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int g_live = 0;
static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }

class CWideString
{
public:
    const char* Text;
    CWideString(const char* t = "") : Text(t) { ++g_live; }
    CWideString(const CWideString& o) : Text(o.Text) { ++g_live; }
    ~CWideString() { --g_live; }
};

struct CRefBase
{
    long Count;
    CRefBase() : Count(0) { ++g_live; }
    virtual ~CRefBase() { --g_live; }
};

template <class T> class CCountedPointer
{
public:
    T* Data;
    CCountedPointer(T* p) : Data(p) { if (Data) ++Data->Count; }
    template <class U> CCountedPointer(const CCountedPointer<U>& o) : Data((T*)o.Data) { if (Data) ++Data->Count; }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data) { if (Data) ++Data->Count; }
    ~CCountedPointer() { if (Data && --Data->Count == 0) delete Data; }
    T* operator->() const { return Data; }
};

struct CActionParam : CRefBase {};
struct CActionParamString : CActionParam
{
    CWideString Value;
    CActionParamString(const CWideString& v) : Value(v) {}
};

struct CAction : CRefBase
{
    long ActionType;
    CActionParam* Params[4];
    int ParamCount;
    CAction() : ActionType(0), ParamCount(0) {}
    ~CAction() { for (int i = 0; i < ParamCount; ++i) delete Params[i]; }
};

static char g_buttons[512];
static float g_ys[8];
static int g_buttonCount = 0;

struct CComponent : CRefBase
{
    const char* Name;
    float Y;
    CCountedPointer<CAction>* Actions[2];
    int Children;
    CComponent(const char* n, float y) : Name(n), Y(y), Children(0) {}
    CComponent* FindChild(const char* name);
    void SetActionParameter(long first) { char b[64]; sprintf(b, "remove:%s:%ld", Name, first); Log(b); }
    void AddChild(const CCountedPointer<CComponent>&) { ++Children; Log("addchild"); }
    void AddAction(const CCountedPointer<CAction>& a)
    {
        char b[96];
        CAction* act = a.Data;
        sprintf(b, "%s@%g=%lx", Name, Y, act->ActionType);
        if (act->ParamCount) { strcat(b, ":"); strcat(b, ((CActionParamString*)act->Params[0])->Value.Text); }
        strcat(g_buttons, b); strcat(g_buttons, "|");
    }
    void AddActionOnLeftClicked(const CCountedPointer<CAction>&) { Log("click"); }
    void AddChildDynamically(const CCountedPointer<CComponent>&) { ++Children; }
    void Initialise() { Log("init"); }
};

static CComponent* g_oldList = 0;
CComponent* CComponent::FindChild(const char* name) { return strcmp(name, "UI_FRONTEND_LIST_FOR_PROFILES") == 0 ? g_oldList : 0; }

static CComponent* g_createdList = 0;
static CComponent* CreateComponent(const char* name, bool) { g_createdList = new CComponent(name, -1.0f); return g_createdList; }
static CComponent* CreateFrontEndButtonComponent(const CWideString& text, float y, const char*, const char*, bool)
{
    if (g_buttonCount < 8) g_ys[g_buttonCount] = y;
    ++g_buttonCount;
    return new CComponent(text.Text, y);
}
static bool VerifySignatureCached(const CWideString& dir) { return dir.Text[0] == 'G'; }
static CWideString GetProfileDir(const CWideString& name) { return name; }

static CWideString* g_profiles = 0;
static long g_profileCount = 0;

// The reconstructed body, in the same statement order as src/00596cce.cpp.
static long RefreshAvailableProfiles(CComponent* menu)
{
    long size = g_profileCount;
    CComponent* oldList = menu->FindChild("UI_FRONTEND_LIST_FOR_PROFILES");
    if (oldList)
        oldList->SetActionParameter(1);
    CCountedPointer<CComponent> newList(CreateComponent("UI_FRONTEND_LIST_FOR_PROFILES", false));
    CCountedPointer<CComponent> list(newList);
    menu->AddChild(newList);

    CWideString newProfileText("NEW");
    CCountedPointer<CComponent> newProfileButton(
        CreateFrontEndButtonComponent(newProfileText, 0.0f, "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST", "", true));
    CCountedPointer<CAction> newProfileAction(new CAction);
    newProfileAction->ActionType = 0x125;
    newProfileButton->AddAction(newProfileAction);
    newProfileButton->AddActionOnLeftClicked(newProfileAction);
    list->AddChildDynamically(newProfileButton);

    float y = 50.0f;
    long count = size;
    for (long i = 0; i != count; ++i)
    {
        CCountedPointer<CComponent> button(
            CreateFrontEndButtonComponent(g_profiles[i], y, "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST", "", false));
        CCountedPointer<CAction> action(new CAction);
        bool valid = VerifySignatureCached(GetProfileDir(g_profiles[i]));
        action->ActionType = valid ? 0x124 : 0xdb;
        CActionParam* parameter = new CActionParamString(g_profiles[i]);
        action->Params[action->ParamCount++] = parameter;
        button->AddAction(action);
        button->AddActionOnLeftClicked(action);
        list->AddChildDynamically(button);
        y += 30.0f;
    }
    menu->Initialise();
    return count;
}

static int Run(int withOldList, long n, const char* expectButtons, int expectChildren)
{
    static CWideString names[3] = { CWideString("Good1"), CWideString("bad"), CWideString("Good2") };
    g_profiles = names;
    g_profileCount = n;
    g_log[0] = 0; g_buttons[0] = 0; g_buttonCount = 0;
    int baseline = g_live;
    CComponent* menu = new CComponent("menu", 0.0f);
    CComponent* old = withOldList ? new CComponent("oldlist", 0.0f) : 0;
    g_oldList = old;
    long r = RefreshAvailableProfiles(menu);
    int ok = 1;
    if (r != n) { printf("count %ld\n", r); ok = 0; }
    if (strcmp(g_buttons, expectButtons) != 0) { printf("buttons %s\n", g_buttons); ok = 0; }
    if (g_createdList == 0 || g_createdList == old) { printf("no fresh list\n"); ok = 0; }
    if (withOldList != (strstr(g_log, "remove:oldlist:1") != 0)) { printf("old list %s\n", g_log); ok = 0; }
    if (strstr(g_log, "init") == 0) { printf("menu not initialised\n"); ok = 0; }
    if (g_buttonCount != n + 1) { printf("button count %d\n", g_buttonCount); ok = 0; }
    for (int k = 0; k < g_buttonCount && k < 8; ++k)
    {
        float want = k == 0 ? 0.0f : 50.0f + 30.0f * (k - 1);
        if (g_ys[k] != want) { printf("y[%d] %g\n", k, g_ys[k]); ok = 0; }
    }
    if (menu->Children != 1) { printf("menu children %d\n", menu->Children); ok = 0; }
    delete menu;
    delete old;
    if (g_live != baseline) { printf("leak %d\n", g_live - baseline); ok = 0; }
    (void)expectChildren;
    return ok;
}

int main()
{
    int ok = 1;
    ok &= Run(1, 3, "NEW@0=125|Good1@50=124:Good1|bad@80=db:bad|Good2@110=124:Good2|", 4);
    ok &= Run(0, 0, "NEW@0=125|", 1);
    ok &= Run(0, 1, "NEW@0=125|Good1@50=124:Good1|", 2);
    if (ok)
        printf("FSE2_00596CCE_TEST PASS\n");
    else
        printf("FSE2_00596CCE_TEST did not pass\n");
    return ok ? 0 : 1;
}
