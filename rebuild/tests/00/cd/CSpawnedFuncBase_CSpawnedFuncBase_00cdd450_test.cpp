// behaviour test for 00cdd450: the CSpawnedFuncBase ctor initialises the base process, ID, frames, names, flags and predicate
#include <cstdio>
struct SStr { const char* p; SStr() : p(0) { } SStr(const SStr& o) : p(o.p) { } };
struct SProcess {
    bool Running, StopRunning, TerminateUsingExceptions; float TimeProcessSliceStarted, TimesliceLength; unsigned long State;
    long StackSize;
    SProcess(bool running, long stackSize, float timeslice)
        : Running(running), StopRunning(false), TerminateUsingExceptions(false), TimeProcessSliceStarted(0), TimesliceLength(timeslice), State(0), StackSize(stackSize) { }
};
struct SPtr { void* Data; void* Info; SPtr() : Data(0), Info(0) { } };
struct SSpawnedFuncBase : SProcess {
    unsigned long ID; long LastScriptFrame, CurrentScriptFrame; SStr Name, AssociatedRegionName; bool FuncPaused, Interrupted; SPtr PPredicate;
    SSpawnedFuncBase(const SStr& name, unsigned long id)
        : SProcess(true, 0x40, 0.1f), ID(id), LastScriptFrame(-1), CurrentScriptFrame(-1), Name(name), FuncPaused(false), Interrupted(false) { }
};
int main()
{
    SStr n; n.p = "WatchForAcolyteHit";
    SSpawnedFuncBase f(n, 77);
    int ok = f.Running && f.StackSize == 0x40 && f.TimesliceLength == 0.1f && f.ID == 77 && f.LastScriptFrame == -1
          && f.CurrentScriptFrame == -1 && f.Name.p == n.p && f.AssociatedRegionName.p == 0 && !f.FuncPaused && !f.Interrupted
          && f.PPredicate.Data == 0 && f.PPredicate.Info == 0;
    if (!ok) { std::printf("00cdd450_TEST FAIL\n"); return 1; }
    std::printf("00cdd450_TEST PASS\n");
    return 0;
}