#include <cstdio>
#include <cstring>
struct CCharString {
    const char* p;
    CCharString(const char* t, long = -1) : p(t) {}
    CCharString(const CCharString& o) : p(o.p) {}
    bool is(const char* t) const { return std::strcmp(p, t) == 0; }
};
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { if (!g_fail) std::printf("check failed at line %d\n", line); g_fail = 1; } }
#define CHECK(c) check((c), __LINE__)
struct CSpawnedFuncBase {
    CCharString Name; unsigned long Id;
    CSpawnedFuncBase(const CCharString& name, unsigned long id) : Name(name), Id(id) {}
    virtual ~CSpawnedFuncBase() {}
    virtual void SuspendableProcess() = 0;
};
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();
    T* PClass;
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
struct CScriptThing;
struct CScriptDataBase;
struct CEntityScriptBase;
template <class T> struct CCountedPointer { T* Data; void* Info; };
struct CEntityScriptBindingBase {
    virtual ~CEntityScriptBindingBase() {}
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef void* (*TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString EntityScriptName; T* PParentScript; TAllocFunc ClassAllocFunc; unsigned long Flags;
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};
static int g_gamePost = 0, g_post = 0, g_order = 0, g_ran = 0;
struct CGameScriptInterface { void PostAddScriptedEntities() const { g_gamePost = ++g_order; } };
static const char* g_bindings[8]; static int g_nb = 0;
static const char* g_spawned[8]; static int g_ns = 0;
static CSpawnedFuncBase* g_funcs[8];
namespace NScript {
struct CScriptBase {
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& region)
    {
        if (!region.is("")) g_fail = 1;
        g_funcs[g_ns] = func; g_spawned[g_ns++] = func->Name.p;
    }
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding)
    {
        g_bindings[g_nb++] = binding->GetEntityScriptName().p;
        delete binding;
    }
    void PostAddScriptedEntities() { g_post = ++g_order; }
};
struct CV_BodyGuardScript : CScriptBase {
    struct CBodyGuard { static void* Alloc(CV_BodyGuardScript&, CScriptDataBase*, const CScriptThing&) { return 0; } };
    struct CHenchmanComment { static void* Alloc(CV_BodyGuardScript&, CScriptDataBase*, const CScriptThing&) { return 0; } };
    struct CHenchmanRegionEntranceComment { static void* Alloc(CV_BodyGuardScript&, CScriptDataBase*, const CScriptThing&) { return 0; } };
    struct CHenchmanRegionExitComment { static void* Alloc(CV_BodyGuardScript&, CScriptDataBase*, const CScriptThing&) { return 0; } };
    struct CBodyGuardRespawn { static void* Alloc(CV_BodyGuardScript&, CScriptDataBase*, const CScriptThing&) { return 0; } };
    CGameScriptInterface* Game;
    void HenchmanDeductWages() { g_ran |= 1; }
    void HenchmanCommentary() { g_ran |= 2; }
    void FisticuffsWatch() { g_ran |= 4; }
    void CheckForHenchmanFiredOverride() { g_ran |= 8; }
    void Main();
};

void CV_BodyGuardScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BodyGuardScript>("BodyGuard", this, &CBodyGuard::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BodyGuardScript>("HenchmanComment", this, &CHenchmanComment::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BodyGuardScript>("HenchmanRegionEntranceComment", this, &CHenchmanRegionEntranceComment::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BodyGuardScript>("HenchmanRegionExitComment", this, &CHenchmanRegionExitComment::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BodyGuardScript>("M_BodyGuardRespawn", this, &CBodyGuardRespawn::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_BodyGuardScript>("HenchmanDeductWages", 0, this, &CV_BodyGuardScript::HenchmanDeductWages), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BodyGuardScript>("HenchmanCommentary", 0, this, &CV_BodyGuardScript::HenchmanCommentary), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BodyGuardScript>("FisticuffsWatch", 0, this, &CV_BodyGuardScript::FisticuffsWatch), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BodyGuardScript>("CheckForHenchmanFiredOverride", 0, this, &CV_BodyGuardScript::CheckForHenchmanFiredOverride), CCharString(""));
}

}
int main()
{
    CGameScriptInterface g; NScript::CV_BodyGuardScript s; s.Game = &g;
    s.Main();
    CHECK(g_nb == 5 && std::strcmp(g_bindings[0], "BodyGuard") == 0 && std::strcmp(g_bindings[4], "M_BodyGuardRespawn") == 0);
    CHECK(g_post == 1 && g_gamePost == 2);
    CHECK(g_ns == 4 && std::strcmp(g_spawned[0], "HenchmanDeductWages") == 0 && std::strcmp(g_spawned[3], "CheckForHenchmanFiredOverride") == 0);
    for (int i = 0; i < g_ns; ++i) { g_funcs[i]->SuspendableProcess(); delete g_funcs[i]; }
    CHECK(g_ran == 15);
    if (g_fail) { std::printf("00e5d4f0_TEST FAIL\n"); return 1; }
    std::printf("00e5d4f0_TEST PASS\n");
    return 0;
}