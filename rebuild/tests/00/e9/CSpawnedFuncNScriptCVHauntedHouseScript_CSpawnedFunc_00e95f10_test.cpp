// behaviour test for 00e95f10: CSpawnedFunc<T> stores the base fields plus the bound object and member callback, and the callback runs
#include <cstdio>
struct SStr { const char* p; SStr() : p(0) { } SStr(const SStr& o) : p(o.p) { } };
struct SSpawnedFuncBase {
    bool Running; unsigned long ID; long LastScriptFrame, CurrentScriptFrame; SStr Name, AssociatedRegionName; bool FuncPaused, Interrupted;
    SSpawnedFuncBase(const SStr& name, unsigned long id)
        : Running(true), ID(id), LastScriptFrame(-1), CurrentScriptFrame(-1), Name(name), FuncPaused(false), Interrupted(false) { }
};
struct SScript { int calls; void DoMission() { ++calls; } };
template <class T> struct SSpawnedFunc : SSpawnedFuncBase {
    void (T::*Func)(); T* PClass;
    SSpawnedFunc(const SStr& name, unsigned long id, T* pclass, void (T::*func)()) : SSpawnedFuncBase(name, id), Func(func), PClass(pclass) { }
    void Run() { (PClass->*Func)(); }
};
int main()
{
    SStr n; n.p = "DoMission";
    SScript s; s.calls = 0;
    SSpawnedFunc<SScript> f(n, 9, &s, &SScript::DoMission);
    f.Run();
    int ok = f.ID == 9 && f.PClass == &s && f.Func == &SScript::DoMission && s.calls == 1 && f.Name.p == n.p && f.LastScriptFrame == -1 && !f.Interrupted;
    if (!ok) { std::printf("00e95f10_TEST FAIL\n"); return 1; }
    std::printf("00e95f10_TEST PASS\n");
    return 0;
}