// Behaviour test for 0x00597006 NUISystem::CFrontEndManager::RefreshAvailableProfilesForDelete: the delete screen's old
// profile list (if any) is closed with a type-1 action, a new list is created and added, one button per profile name at
// y = 0, 30, 60 ... gets a shared type-0xD7 action carrying that name (select + delete), is added to the list, then the
// screen is re-initialised and the profile count returned; every counted pointer is released.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_log[64]; static int g_logN = 0;
static void Log(int v) { if (g_logN < 64) g_log[g_logN++] = v; }
class CCharString { public: const char* PStringData; CCharString(const char* t, long = -1) : PStringData(t) {} ~CCharString() {} };
class CWideString { public: void* PStringData; };
struct CCPPointerInfo { long RefCount; void (__fastcall* DeleteFunc)(void* data); void* Data; };
static int g_liveInfos = 0;
namespace NUISystem {
class CComponent; class CList; class CActionParam; class CActionParamString;
static CActionParam* g_lastParam = 0;
struct CActionParamList { void* _M_node; void push_back(CActionParam* const& x) { g_lastParam = x; _M_node = (void*)x; } };
struct CActionParamListOwned { void* _M_node; ~CActionParamListOwned() {} };
namespace N_0042bed4 { struct CAction {
    long ActionType; CActionParamListOwned UIActionParams;
    CAction() : ActionType(0) { UIActionParams._M_node = 0; }
    CAction(const CAction& o) : ActionType(o.ActionType) { UIActionParams._M_node = 0; }
}; }
class CAction { public: long ActionType; CActionParamList UIActionParams; void* ThingActionParams; bool Valid;
    CAction() : ActionType(0), ThingActionParams(0), Valid(true) { UIActionParams._M_node = 0; } };
class CActionParamString { public: void* __vftable; long ActionParamType; CWideString Value;
    CActionParamString(const CWideString& v) : __vftable(0), ActionParamType(3), Value(v) {} };
}
namespace NUISystem { class CComponent; class CAction; }
static void __fastcall DeleteComponent(void* p);
static void __fastcall DeleteAction(void* p);
template <class T> struct SDeleter;
template <> struct SDeleter<NUISystem::CComponent> { static void (__fastcall* Get())(void*) { return DeleteComponent; } };
template <> struct SDeleter<NUISystem::CAction> { static void (__fastcall* Get())(void*) { return DeleteAction; } };
template <class T> class CCountedPointer {
public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer(T* data) : Data(data), Info(0)
    { if (data) { Info = new CCPPointerInfo; Info->RefCount = 1; Info->DeleteFunc = SDeleter<T>::Get(); Info->Data = data; ++g_liveInfos; } }
    ~CCountedPointer() { Release(); }
    void Release() { if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; --g_liveInfos; } Data = 0; Info = 0; }
    T* operator->() const { return Data; }
};
struct CListPointer {
    NUISystem::CList* Data; CCPPointerInfo* Info;
    CListPointer(const CCountedPointer<NUISystem::CComponent>& other) : Data(0), Info(0) { ShareData((NUISystem::CList*)other.Data, other.Info); }
    ~CListPointer() { Release(); }
    void Release() { if (Info) { if (--Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; --g_liveInfos; } } Data = 0; Info = 0; }
    void ShareData(NUISystem::CList* data, CCPPointerInfo* info) { if (Info != info) { Release(); Data = data; Info = info; if (info) ++info->RefCount; } }
    NUISystem::CList* operator->() const { return Data; }
};
namespace NUISystem {
static int g_liveComponents = 0;
class CComponent {
public:
    int Id; long LastAction; float Y; const CWideString* Text; CAction* Select; CAction* Delete; int Children; int Initialised;
    CComponent(int id) : Id(id), LastAction(-1), Y(-1), Text(0), Select(0), Delete(0), Children(0), Initialised(0) { ++g_liveComponents; }
    virtual ~CComponent() { --g_liveComponents; }
    virtual CComponent* FindChild(CCharString name);
    virtual void Initialise() { ++Initialised; Log(900 + Id); }
    virtual void AddChild(CCountedPointer<CComponent>& child) { ++Children; Log(100 + child->Id); }
    virtual void AddChildAfterFocus(CCountedPointer<CComponent>& child) { ++Children; Log(200 + child->Id); }
    virtual void SetAction(N_0042bed4::CAction action) { LastAction = action.ActionType; Log(300 + Id); }
    virtual void SetSelectAction(CCountedPointer<CAction>& a) { Select = a.Data; }
    virtual void SetDeleteAction(CCountedPointer<CAction>& a) { Delete = a.Data; }
};
class CList : public CComponent { public: CList(int id) : CComponent(id) {} };
static CComponent* g_oldList = 0;
CComponent* CComponent::FindChild(CCharString name)
{ return strcmp(name.PStringData, "UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE") == 0 ? g_oldList : 0; }
static int g_nextId = 10;
class CManager { public: CComponent* CreateComponent(const CCharString& def, bool b)
{ Log(strcmp(def.PStringData, "UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE") == 0 && !b ? 1 : -1); return new CList(g_nextId++); } };
enum FRONTEND_SCREEN { FRONTEND_SCREEN_DELETE_PROFILE = 21 };
static CComponent* g_screen = 0; static int g_key = -1;
struct CScreenMap { void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    CComponent*& operator[](const FRONTEND_SCREEN& key) { g_key = key; return g_screen; } };
static CWideString g_names[3]; static unsigned long g_nameCount = 3; static int g_namesFreed = 0;
struct CWideStringVector {
    CWideString* _M_start; CWideString* _M_finish; CWideString* _M_end_of_storage;
    CWideStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CWideStringVector() { ++g_namesFreed; }
    unsigned long size() const { return _M_finish - _M_start; }
    CWideString& operator[](unsigned long n) { return _M_start[n]; }
};
struct SButtonRecord { const CWideString* text; float y; int id; };
static SButtonRecord g_buttons[8];
class CFrontEndManager : public CManager {
public:
    static CFrontEndManager* __fastcall GetInstance();
    void GetProfileNames(CWideStringVector& names) { names._M_start = g_names; names._M_finish = names._M_end_of_storage = g_names + g_nameCount; }
    CComponent* CreateFrontEndButtonComponent(CWideString& text, float y, const CCharString& frameDef, const CCharString& buttonDef, bool flag)
    {
        CComponent* c = new CComponent(g_nextId++);
        int n = c->Id;
        if (strcmp(frameDef.PStringData, "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST") != 0 || buttonDef.PStringData[0] != 0 || flag) c->Id = -1;
        c->Text = &text; c->Y = y; (void)n;
        return c;
    }
    unsigned long RefreshAvailableProfilesForDelete();
    unsigned char _pad00[0x54];
    CScreenMap m_mapScreens;
};
static CFrontEndManager g_manager;
CFrontEndManager* __fastcall CFrontEndManager::GetInstance() { return &g_manager; }
}
static void __fastcall DeleteComponent(void* p) { delete (NUISystem::CComponent*)p; }
static void __fastcall DeleteAction(void* p) { delete (NUISystem::CAction*)p; }
namespace NUISystem {
unsigned long CFrontEndManager::RefreshAvailableProfilesForDelete()
{
    CWideStringVector names;
    GetProfileNames(names);
    CFrontEndManager* manager = GetInstance();
    CComponent* screen = m_mapScreens[FRONTEND_SCREEN_DELETE_PROFILE];
    CComponent* oldList = screen->FindChild("UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE");
    if (oldList) {
        N_0042bed4::CAction close;
        close.ActionType = 1;
        oldList->SetAction(close);
    }
    CCountedPointer<CComponent> listComponent(manager->CreateComponent("UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE", false));
    CListPointer list(listComponent);
    screen->AddChild(listComponent);
    float y = 0.0f;
    const unsigned long count = names.size();
    for (unsigned long i = 0; i < count; ++i) {
        CCountedPointer<CComponent> button(CreateFrontEndButtonComponent(names[i], y, "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST", "", false));
        CCountedPointer<CAction> action(new CAction());
        CActionParam* param = (CActionParam*)new CActionParamString(names[i]);
        action->ActionType = 0xd7;
        action->UIActionParams.push_back(param);
        g_buttons[i].text = button->Text; g_buttons[i].y = button->Y; g_buttons[i].id = button->Id;
        button->SetSelectAction(action);
        button->SetDeleteAction(action);
        if (button->Select != action.Data || button->Delete != action.Data || action->ActionType != 0xd7
            || ((CActionParamString*)g_lastParam)->Value.PStringData != names[i].PStringData) g_buttons[i].y = -99.0f;
        list->AddChildAfterFocus(button);
        y += 30.0f;
    }
    screen->Initialise();
    return count;
}
}
int main()
{
    using namespace NUISystem;
    int i;
    for (i = 0; i < 3; ++i) g_names[i].PStringData = (void*)(g_names + i);
    CComponent screen(1), old(2);
    g_screen = &screen; g_oldList = &old;
    unsigned long n = g_manager.RefreshAvailableProfilesForDelete();
    bool ok = n == 3 && g_key == 21 && old.LastAction == 1 && screen.Children == 1 && screen.Initialised == 1 && g_namesFreed == 1;
    for (i = 0; ok && i < 3; ++i) ok = g_buttons[i].text == &g_names[i] && g_buttons[i].y == (float)(30 * i) && g_buttons[i].id >= 0;
    // log: old list closed, list created, list added, three buttons added to the list (ids 11..13), screen initialised
    int expect[] = { 302, 1, 110, 211, 212, 213, 901 };
    ok = ok && g_logN == 7;
    for (i = 0; ok && i < 7; ++i) ok = g_log[i] == expect[i];
    ok = ok && g_liveInfos == 0 && g_liveComponents == 2;
    g_oldList = 0; g_nameCount = 0; g_logN = 0;
    n = g_manager.RefreshAvailableProfilesForDelete();
    ok = ok && n == 0 && g_logN == 3 && g_log[0] == 1 && g_log[1] == 114 && g_log[2] == 901 && g_liveInfos == 0 && g_namesFreed == 2;
    if (!ok) { printf("00597006_TEST FAIL\n"); return 1; }
    printf("00597006_TEST PASS\n");
    return 0;
}
