#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
static int g_frame = 0;
static int g_termAt = 1000;
static void (*g_hook)(int frame) = 0;
static void Log(const std::string& s) { char b[16]; std::sprintf(b, "@%d", g_frame); g_log += s; g_log += b; g_log += ";"; }
static void Reset(int termAt) { g_log = ""; g_frame = 0; g_termAt = termAt; g_hook = 0; }
struct CCharString {
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
};
struct CScriptThing {
    std::string Name;
    CScriptThing() { }
    CScriptThing(const std::string& name) : Name(name) { }
};
struct CScriptBase {
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
};
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s [%s]\n", addr, what, g_log.c_str()); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
struct CScriptDataBase { };
struct CEntityScriptBase { };
struct CEntityScriptBindingBase {
    std::string Name;
    unsigned long Flags;
    virtual ~CEntityScriptBindingBase() { }
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CEntityScriptBase* (*TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    T* Parent;
    TAllocFunc AllocFunc;
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : Parent(parent), AllocFunc(alloc) { Name = name.S; Flags = flags; }
};
struct CSpawnedFuncBase {
    std::string Name;
    virtual ~CSpawnedFuncBase() { }
    virtual void Run() = 0;
};
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    T* PClass;
    void (T::*Func)();
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)()) : PClass(pclass), Func(func) { Name = name.S; (void)id; }
    void Run() { (PClass->*Func)(); }
};
struct CGameScriptInterface {
    void NewScriptFrame() { ++g_frame; }
    bool IsRegionLoaded(const CCharString& name) { return g_frame >= 2 && name.S.size() > 0; }
    void PostAddScriptedEntities() { Log("game post"); }
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;
    CEntityScriptBindingBase* Bindings[8];
    int BindingCount;
    CSpawnedFuncBase* Spawned[8];
    std::string SpawnedRegion[8];
    int SpawnedCount;
    CQuestScriptBase() : BindingCount(0), SpawnedCount(0) { }
    ~CQuestScriptBase()
    {
        for (int i = 0; i < BindingCount; ++i) delete Bindings[i];
        for (int i = 0; i < SpawnedCount; ++i) delete Spawned[i];
    }
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding) { Log("bind " + binding->Name); Bindings[BindingCount++] = binding; }
    void PostAddScriptedEntities() { Log("post"); }
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& region)
    { Log("spawn " + func->Name + " " + region.S); SpawnedRegion[SpawnedCount] = region.S; Spawned[SpawnedCount++] = func; }
};
// behaviour test: CV_FishingCompetitionScript::Main waits for the Fisherman region, binds the three entity scripts in
// retail order with flags 0 and their own Alloc functions, then finishes the scripted-entity setup on both sides
struct CV_FishingCompetitionScript;
static int g_allocTag = 0;
struct CBountyHuntActivationChecker { static CEntityScriptBase* Alloc(CV_FishingCompetitionScript&, CScriptDataBase*, const CScriptThing&) { g_allocTag = 1; return 0; } };
struct CFishCompCompetitor { static CEntityScriptBase* Alloc(CV_FishingCompetitionScript&, CScriptDataBase*, const CScriptThing&) { g_allocTag = 2; return 0; } };
struct CFishCompOrganiser { static CEntityScriptBase* Alloc(CV_FishingCompetitionScript&, CScriptDataBase*, const CScriptThing&) { g_allocTag = 3; return 0; } };
struct CV_FishingCompetitionScript : CQuestScriptBase {
    void Main();
};
void CV_FishingCompetitionScript::Main()
{
    while (!Game->IsRegionLoaded("Fisherman")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FishingCompetitionScript>("BountyHuntActivationChecker", this, &CBountyHuntActivationChecker::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FishingCompetitionScript>("FishCompCompetitor", this, &CFishCompCompetitor::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FishingCompetitionScript>("FishCompOrganiser", this, &CFishCompOrganiser::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
}
int main()
{
    CGameScriptInterface game;
    {
        CV_FishingCompetitionScript script;
        script.Game = &game;
        Reset(1000);
        script.Main();
        CHECK("00e7e370", g_log == "bind BountyHuntActivationChecker@2;bind FishCompCompetitor@2;bind FishCompOrganiser@2;post@2;game post@2;");
        CHECK("00e7e370", script.BindingCount == 3);
        for (int i = 0; i < 3; ++i) {
            CEntityScriptBinding<CV_FishingCompetitionScript>* b = static_cast<CEntityScriptBinding<CV_FishingCompetitionScript>*>(script.Bindings[i]);
            CScriptThing thing;
            b->AllocFunc(*b->Parent, 0, thing);
            CHECK("00e7e370", b->Flags == 0 && b->Parent == &script && g_allocTag == i + 1);
        }
    }
    {
        CV_FishingCompetitionScript script;
        script.Game = &game;
        Reset(1);
        script.Main();
        CHECK("00e7e370", g_log == "" && script.BindingCount == 0);
    }
    std::printf("00e7e370_TEST PASS\n");
    return 0;
}
