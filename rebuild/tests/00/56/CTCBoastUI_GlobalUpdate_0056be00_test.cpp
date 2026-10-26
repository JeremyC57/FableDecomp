// 0056be00 behaviour test: CTCBoastUI::GlobalUpdate (match.cpp body: quit path and activation path)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class CCharString { public: std::string S; CCharString(const char* t) : S(t) {} };
class CThing { public: std::string ScriptName; };
class CThingFilter_HasScriptName { public: CCharString Name; CThingFilter_HasScriptName(const CCharString& n) : Name(n) {} };
class CThingSearchTools { public:
    std::vector<CThing*> Things;
    bool GetPFirstThing(const CThingFilter_HasScriptName& f, CThing** out) const
    { for (size_t i = 0; i < Things.size(); ++i) if (Things[i]->ScriptName == f.Name.S) { *out = Things[i]; return true; } return false; }
};
class CCameraModeDef { public: long Refs; };
template <class T> class CDefPointer { public: T* Data; CDefPointer() : Data(0) {} ~CDefPointer() { if (Data) --Data->Refs; } };
static CCameraModeDef g_nullDef = { 0 };
static std::string g_lastDefName;
class CGameDefinitionManager { public:
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    long GetDefGlobalIndexFromName(const CCharString& n) { g_lastDefName = n.S; return 77; }
    bool GetDef(long index, CDefPointer<CCameraModeDef>* out) { if (index != 77) return false; out->Data = &g_nullDef; ++g_nullDef.Refs; return true; }
};
struct SCountInfo { long Count; };
static int g_liveModes = 0;
namespace NCameraModes {
    class CBase { public: virtual ~CBase() { --g_liveModes; } virtual long GetModeType() = 0; };
}
template <class T> class CCountedPointer { public:
    T* Data; SCountInfo* Info;
    CCountedPointer(T* d) : Data(d), Info(new SCountInfo) { Info->Count = 1; }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->Count; }
    ~CCountedPointer() { if (Info && --Info->Count == 0) { delete Data; delete Info; } }
};
class CGameCameraManager;
namespace NCameraModes {
    struct CFixed : CBase { long Type; CFixed(long t) : Type(t) { ++g_liveModes; } long GetModeType() { return Type; } };
    struct CSnapToPoint : CBase {
        CGameCameraManager* Manager; CCameraModeDef* Def; CThing* Point;
        CSnapToPoint(CGameCameraManager* m, const CDefPointer<CCameraModeDef>& d, CThing* p) : Manager(m), Def(d.Data), Point(p) { ++g_liveModes; }
        long GetModeType() { return 7; }
    };
}
class CGameCameraManager { public:
    NCameraModes::CBase* CurrentMode;
    std::vector<CCountedPointer<NCameraModes::CBase> > Stack;
    void PushCameraMode(CCountedPointer<NCameraModes::CBase> mode) { Stack.push_back(mode); }
};
class CWorld { public: CThingSearchTools* PThingSearchTools; CGameCameraManager* CameraManager; CThingSearchTools& GetThingSearchTools() { return *PThingSearchTools; } };
class CEngine { public: CWorld* World; };
CEngine* g_pEngine;
static int g_closed, g_cleared, g_reset;
struct CFrontEndManager {
    static CFrontEndManager* GetInstance() { static CFrontEndManager f; return &f; }
    void Close() { ++g_closed; } void ClearMenus() { ++g_cleared; } void ResetInput() { ++g_reset; }
};
struct SListNode { SListNode* Next; SListNode* Prev; };
SListNode* g_BoastList;
static int g_compDeleted;
struct CComponent { virtual ~CComponent() { ++g_compDeleted; } };
static long g_removedMode;
class CPlayer { public: void RemoveMode(long m) { g_removedMode = m; } };
class CPlayerManager { public: CPlayer P; CPlayer* GetMainPlayer() { return &P; } };
static CPlayerManager g_pm;
static int g_activated;
class CTCBoastUI { public:
    bool Active;
    std::vector<CComponent*> Components;
    bool Quitting, AllowedToOpen, ActivationRequested, ActivateLater;
    CPlayerManager* GetPlayerManager() { return &g_pm; }
    void Activate() { ++g_activated; }
    void GlobalUpdate(long);
};
void CTCBoastUI::GlobalUpdate(long)
{
    if (Quitting) {
        for (unsigned long i = 0; i < Components.size(); ++i)
            delete Components[i];
        Components.clear();
        CFrontEndManager::GetInstance()->Close();
        CFrontEndManager::GetInstance()->ClearMenus();
        SListNode* node = g_BoastList->Next;
        while (node != g_BoastList) {
            SListNode* dead = node;
            node = node->Next;
            free(dead);
        }
        g_BoastList->Next = g_BoastList;
        g_BoastList->Prev = g_BoastList;
        CFrontEndManager::GetInstance()->ResetInput();
        Active = false;
        Quitting = false;
        ActivationRequested = false;
        GetPlayerManager()->GetMainPlayer()->RemoveMode(0x20);
    }
    if (ActivationRequested) {
        if (Active) {
            CGameCameraManager* camera = g_pEngine->World->CameraManager;
            if (camera->CurrentMode->GetModeType() != 2) {
                CThing* point = 0;
                if (g_pEngine->World->GetThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName("BoastingCamera"), &point)) {
                    CDefPointer<CCameraModeDef> def;
                    CGameDefinitionManager::Get().GetDef(CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CAMERA_MODE_MODE_NULL"), &def);
                    CCountedPointer<NCameraModes::CBase> mode(new NCameraModes::CSnapToPoint(camera, def, point));
                    camera->PushCameraMode(mode);
                }
            }
        } else if (AllowedToOpen) {
            Activate();
        }
    }
}
int main()
{
    SListNode head; head.Next = head.Prev = &head; g_BoastList = &head;
    CThing cam; cam.ScriptName = "BoastingCamera"; CThing other; other.ScriptName = "X";
    CThingSearchTools tools; tools.Things.push_back(&other); tools.Things.push_back(&cam);
    NCameraModes::CFixed normal(1), boast(2);
    CGameCameraManager cm; cm.CurrentMode = &normal;
    CWorld w; w.PThingSearchTools = &tools; w.CameraManager = &cm; CEngine e; e.World = &w; g_pEngine = &e;
    { // quitting: components deleted, list emptied, front end reset, flags cleared, mode 0x20 removed; no activation afterwards
        CTCBoastUI ui; ui.Active = true; ui.Quitting = true; ui.AllowedToOpen = true; ui.ActivationRequested = true;
        ui.Components.push_back(new CComponent); ui.Components.push_back(new CComponent);
        for (int i = 0; i < 3; ++i) { SListNode* n = (SListNode*)malloc(sizeof(SListNode)); n->Next = head.Next; n->Prev = &head; head.Next = n; }
        g_compDeleted = 0; g_closed = g_cleared = g_reset = 0; g_removedMode = 0; g_activated = 0;
        ui.GlobalUpdate(0);
        CHECK(g_compDeleted == 2 && ui.Components.empty());
        CHECK(g_closed == 1 && g_cleared == 1 && g_reset == 1);
        CHECK(head.Next == &head && head.Prev == &head);
        CHECK(!ui.Active && !ui.Quitting && !ui.ActivationRequested);
        CHECK(g_removedMode == 0x20);
        CHECK(g_activated == 0 && cm.Stack.empty());
    }
    { // activation requested while inactive: Activate only when allowed
        CTCBoastUI ui; ui.Active = false; ui.Quitting = false; ui.AllowedToOpen = false; ui.ActivationRequested = true;
        g_activated = 0; ui.GlobalUpdate(0); CHECK(g_activated == 0);
        ui.AllowedToOpen = true; ui.GlobalUpdate(0); CHECK(g_activated == 1); CHECK(cm.Stack.empty());
    }
    { // active: a snap-to-point mode on the boasting camera is pushed, the def comes from CAMERA_MODE_MODE_NULL
        CTCBoastUI ui; ui.Active = true; ui.Quitting = false; ui.AllowedToOpen = false; ui.ActivationRequested = true;
        ui.GlobalUpdate(0);
        CHECK(cm.Stack.size() == 1);
        NCameraModes::CSnapToPoint* m = (NCameraModes::CSnapToPoint*)cm.Stack[0].Data;
        CHECK(m->Point == &cam && m->Manager == &cm && m->Def == &g_nullDef);
        CHECK(g_lastDefName == "CAMERA_MODE_MODE_NULL");
        CHECK(cm.Stack[0].Info->Count == 1);          // the local `mode` released its reference
        CHECK(g_nullDef.Refs == 0);                   // the def pointer released its temporary
        // already in boast mode (type 2): nothing pushed
        cm.CurrentMode = &boast; ui.GlobalUpdate(0); CHECK(cm.Stack.size() == 1);
        // no boasting camera: nothing pushed
        cm.CurrentMode = &normal; tools.Things.pop_back(); ui.GlobalUpdate(0); CHECK(cm.Stack.size() == 1);
        cm.Stack.clear();
    }
    if (!g_fail) printf("0056be00_TEST PASS\n");
    return g_fail;
}