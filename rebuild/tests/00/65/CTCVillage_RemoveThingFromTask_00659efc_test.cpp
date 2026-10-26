// behaviour test for 00659efc CTCVillage::RemoveThingFromTask: each task type dispatches to the matching task list
// (0 torch, 1 patrol, 2 crops, 3 sweep, 4 horn, 5 crate) with a copy of the task pointer and the thing
#include <cstdio>
class CThing { public: int id; };
class CVillageTaskTorch; class CVillageTaskHorn; class CVillageTaskPatrol;
class CVillageTaskPickCrops; class CVillageTaskSecuritySweep; class CVillageTaskCarryCrate;
class CVillageTaskBase { public:
    long type;
    virtual void v00() {}
    virtual void v01() {}
    virtual long GetTaskType() { return type; }
};
static int g_copies = 0, g_live = 0;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* m_pItem;
    ~CBaseIntelligentPointer() { --g_live; }
    CVillageTaskBase* GetPItem() const { return (CVillageTaskBase*)m_pItem; }
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00() {}
    CIntelligentPointer(T* item) { m_pItem = item; ++g_live; }
    CIntelligentPointer(const CIntelligentPointer& other) { m_pItem = other.m_pItem; ++g_copies; ++g_live; }
};
template<class T> struct CTaskList { int id; };
static int g_list = -1; static const CThing* g_thing = 0; static void* g_task = 0;
class CTCVillage { public:
    void*                                 __vftable;
    CTaskList<CVillageTaskTorch>          TorchTasks;
    CTaskList<CVillageTaskHorn>           HornTasks;
    CTaskList<CVillageTaskPatrol>         PatrolTasks;
    CTaskList<CVillageTaskPickCrops>      CropTasks;
    CTaskList<CVillageTaskSecuritySweep>  SecuritySweepTasks;
    CTaskList<CVillageTaskCarryCrate>     CrateTasks;
    template<class T> void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing,
                                               CTaskList<T>& tasks) const
    { g_list = tasks.id; g_thing = &thing; g_task = task.m_pItem; }
    void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing);
};
void CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing)
{
    switch (task.GetPItem()->GetTaskType()) {
    case 5:
        RemoveThingFromTask(task, thing, CrateTasks);
        break;
    case 4:
        RemoveThingFromTask(task, thing, HornTasks);
        break;
    case 3:
        RemoveThingFromTask(task, thing, SecuritySweepTasks);
        break;
    case 2:
        RemoveThingFromTask(task, thing, CropTasks);
        break;
    case 1:
        RemoveThingFromTask(task, thing, PatrolTasks);
        break;
    case 0:
        RemoveThingFromTask(task, thing, TorchTasks);
        break;
    }
}
int main()
{
    CTCVillage v; v.TorchTasks.id = 10; v.HornTasks.id = 11; v.PatrolTasks.id = 12; v.CropTasks.id = 13;
    v.SecuritySweepTasks.id = 14; v.CrateTasks.id = 15;
    static const int expect[6] = { 10, 12, 13, 14, 11, 15 };
    CThing thing; thing.id = 1; bool ok = true;
    for (long t = 0; t < 6; ++t) {
        CVillageTaskBase task; task.type = t; g_list = -1;
        CIntelligentPointer<CVillageTaskBase> p(&task);
        v.RemoveThingFromTask(p, thing);
        ok = ok && g_list == expect[t] && g_thing == &thing && g_task == &task;
    }
    CVillageTaskBase odd; odd.type = 9; g_list = -1;
    { CIntelligentPointer<CVillageTaskBase> p(&odd); v.RemoveThingFromTask(p, thing); }
    ok = ok && g_list == -1 && g_copies == 13 && g_live == 0;
    if (!ok) { std::printf("00659efc_TEST FAIL\n"); return 1; }
    std::printf("00659efc_TEST PASS\n");
    return 0;
}