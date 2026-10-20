#include <cstdio>
#include <cstring>

static int g_live;
struct CCharString {
    char buf[64];
    CCharString(const char* text, long = -1) { std::strcpy(buf, text); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(buf, o.buf); ++g_live; }
    ~CCharString() { --g_live; }
};
static CCharString operator+(const CCharString& left, const CCharString& right)
{
    CCharString r(left); std::strcat(r.buf, right.buf); return r;
}
static bool g_killedByHero;
struct CScriptThing {
    bool MsgIsKilledBy(const CCharString& killer) const { return g_killedByHero && std::strcmp(killer.buf, "SCRIPT_NAME_HERO") == 0; }
};
struct CSpawnedFuncBase {
    char Name[64]; unsigned long Id;
    CSpawnedFuncBase(const CCharString& name, unsigned long id) : Id(id) { std::strcpy(Name, name.buf); }
    virtual ~CSpawnedFuncBase() {}
    virtual void SuspendableProcess() = 0;
};
template <class T> struct CSpawnedFunc0 : CSpawnedFuncBase {
    void (T::*Func)();
    T* PClass;
    CSpawnedFunc0(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
static CSpawnedFuncBase* g_spawned; static char g_region[16]; static int g_ended;
struct CScriptBase {
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName) { g_spawned = func; std::strcpy(g_region, regionName.buf); }
};
struct CV_FishingCompetitionScript : CScriptBase {
    bool MissionFailed;
    void EndCompetition() { ++g_ended; }
};
struct CFishCompOrganiser {
    CScriptThing Me;
    CV_FishingCompetitionScript* ParentClass;
    void OnPredicateFail()
    {
        bool killed = Me.MsgIsKilledBy("SCRIPT_NAME_HERO");
        if (killed) {
            ParentClass->MissionFailed = true;
            ParentClass->AddSpawnedFunction(
                new CSpawnedFunc0<CV_FishingCompetitionScript>(CCharString("ParentClass.") + CCharString("EndCompetition"), 0,
                                                               ParentClass, &CV_FishingCompetitionScript::EndCompetition),
                CCharString("", -1));
        }
    }
};
int main()
{
    CV_FishingCompetitionScript parent; parent.MissionFailed = false;
    CFishCompOrganiser org; org.ParentClass = &parent;
    org.OnPredicateFail();                               // not killed by the hero: nothing
    if (parent.MissionFailed || g_spawned) { std::printf("00e7f030_TEST FAIL\n"); return 1; }
    g_killedByHero = true;
    org.OnPredicateFail();
    if (!parent.MissionFailed || !g_spawned || std::strcmp(g_spawned->Name, "ParentClass.EndCompetition") != 0 ||
        g_spawned->Id != 0 || g_region[0] != 0 || g_live != 0) {
        std::printf("00e7f030_TEST FAIL\n"); return 1;
    }
    g_spawned->SuspendableProcess();
    if (g_ended != 1) { std::printf("00e7f030_TEST FAIL\n"); return 1; }
    delete g_spawned;
    std::printf("00e7f030_TEST PASS\n");
    return 0;
}