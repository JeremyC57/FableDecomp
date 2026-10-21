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
// behaviour test: CV_KnotholeGladeGatesScript::Main resets the quest flags, binds the two entity scripts (flags 1),
// finishes the scripted-entity setup and spawns WatchForTermination then DoMission in KnotholeGlade
struct CV_KnotholeGladeGatesScript;
static int g_allocTag = 0;
struct CKGG_VillagerOutside { static CEntityScriptBase* Alloc(CV_KnotholeGladeGatesScript&, CScriptDataBase*, const CScriptThing&) { g_allocTag = 1; return 0; } };
struct CKGG_SurpriseBalverine { static CEntityScriptBase* Alloc(CV_KnotholeGladeGatesScript&, CScriptDataBase*, const CScriptThing&) { g_allocTag = 2; return 0; } };
struct CV_KnotholeGladeGatesScript : CQuestScriptBase {
    bool Flag;
    int Ran;
    void ResetMissionState() { Flag = false; Log("reset"); }
    void WatchForTermination() { Ran = 1; }
    void DoMission() { Ran = 2; }
    void Main();
};
void CV_KnotholeGladeGatesScript::Main()
{
    ResetMissionState();
    AddEntityScriptBinding(new CEntityScriptBinding<CV_KnotholeGladeGatesScript>("KGG_VillagerOutside", this, &CKGG_VillagerOutside::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_KnotholeGladeGatesScript>("KGG_SurpriseBalverine", this, &CKGG_SurpriseBalverine::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_KnotholeGladeGatesScript>("WatchForTermination", 0, this, &CV_KnotholeGladeGatesScript::WatchForTermination), "KnotholeGlade");
    AddSpawnedFunction(new CSpawnedFunc<CV_KnotholeGladeGatesScript>("DoMission", 0, this, &CV_KnotholeGladeGatesScript::DoMission), "KnotholeGlade");
}
int main()
{
    CGameScriptInterface game;
    CV_KnotholeGladeGatesScript script;
    script.Game = &game;
    script.Flag = true;
    script.Ran = 0;
    Reset(1000);
    script.Main();
    CHECK("00e9fcd0", g_log == "reset@0;bind KGG_VillagerOutside@0;bind KGG_SurpriseBalverine@0;post@0;game post@0;spawn WatchForTermination KnotholeGlade@0;spawn DoMission KnotholeGlade@0;");
    CHECK("00e9fcd0", !script.Flag && script.BindingCount == 2 && script.SpawnedCount == 2);
    for (int i = 0; i < 2; ++i) {
        CEntityScriptBinding<CV_KnotholeGladeGatesScript>* b = static_cast<CEntityScriptBinding<CV_KnotholeGladeGatesScript>*>(script.Bindings[i]);
        CScriptThing thing;
        b->AllocFunc(*b->Parent, 0, thing);
        CHECK("00e9fcd0", b->Flags == 1 && g_allocTag == i + 1);
        script.Spawned[i]->Run();
        CHECK("00e9fcd0", script.Ran == i + 1);
    }
    std::printf("00e9fcd0_TEST PASS\n");
    return 0;
}