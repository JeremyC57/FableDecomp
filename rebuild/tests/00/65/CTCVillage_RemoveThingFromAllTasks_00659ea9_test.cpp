#include <cstdio>
class CThing { public: int id; };
template<class T> struct CTaskList { int removed; const CThing* last; };
class CVillageTaskTorch; class CVillageTaskHorn; class CVillageTaskPatrol; class CVillageTaskPickCrops; class CVillageTaskSecuritySweep; class CVillageTaskCarryCrate;
static int g_order = 0;
class CTCVillage { public:
    CTaskList<CVillageTaskTorch> TorchTasks; CTaskList<CVillageTaskHorn> HornTasks; CTaskList<CVillageTaskPatrol> PatrolTasks;
    CTaskList<CVillageTaskPickCrops> CropTasks; CTaskList<CVillageTaskSecuritySweep> SecuritySweepTasks; CTaskList<CVillageTaskCarryCrate> CrateTasks;
    template<class T> void RemoveThingFromTaskList(const CThing& thing, CTaskList<T>& tasks) { tasks.removed = ++g_order; tasks.last = &thing; }
    void RemoveThingFromAllTasks(const CThing& thing)
    {
        RemoveThingFromTaskList(thing, TorchTasks);
        RemoveThingFromTaskList(thing, HornTasks);
        RemoveThingFromTaskList(thing, PatrolTasks);
        RemoveThingFromTaskList(thing, CropTasks);
        RemoveThingFromTaskList(thing, SecuritySweepTasks);
        RemoveThingFromTaskList(thing, CrateTasks);
    }
};
int main()
{
    CTCVillage v; CThing t;
    v.RemoveThingFromAllTasks(t);
    if (v.TorchTasks.removed != 1 || v.HornTasks.removed != 2 || v.PatrolTasks.removed != 3 || v.CropTasks.removed != 4
        || v.SecuritySweepTasks.removed != 5 || v.CrateTasks.removed != 6 || v.CrateTasks.last != &t || v.TorchTasks.last != &t) { std::printf("00659ea9_TEST FAIL\n"); return 1; }
    std::printf("00659ea9_TEST PASS\n");
    return 0;
}