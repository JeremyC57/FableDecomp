// Run the selected body verbatim on village, light and clock doubles.
#include <assert.h>
#include <stdio.h>
#include <list>
#define FABLE_RETAIL_ASSERT(expression) ((void)0)

class CGameTimeManager {
public:
    bool IsDay() const { return Day; }
    bool Day;
};
class CWorld {
public:
    static long GetFrame() { return Frame; }
    const CGameTimeManager& PeekGameTimeManager() const { return Time; }
    CGameTimeManager Time;
    static long Frame;
};
long CWorld::Frame;

class CMainGameComponent {
public:
    static long GetConstantFPS() { return FPS; }
    static long FPS;
};
long CMainGameComponent::FPS = 30;

class CTCLight {
public:
    CTCLight() : Active(false), Changes(0) {}
    bool IsActive() const { return Active; }
    void SetActive(bool active) { Active = active; ++Changes; }
    bool Active;
    int Changes;
};
class CThing {
public:
    CThing() : Dead(false), HasLight(true) {}
    bool IsDead() const { return Dead; }
    bool GetTC(CTCLight** light) {
        if (!HasLight)
            return false;
        *light = &Light;
        return true;
    }
    bool Dead;
    bool HasLight;
    CTCLight Light;
};

template <class T> class CIntelligentPointer {
public:
    explicit CIntelligentPointer(T* thing) : Thing(thing) {}
    T* Get() const { return Thing; }
private:
    T* Thing;
};

template <class T> class CCountedPointer {
public:
    explicit CCountedPointer(T* thing) : Thing(thing), Owners(new int(1)) {}
    CCountedPointer(const CCountedPointer& other)
      : Thing(other.Thing), Owners(other.Owners) { ++*Owners; }
    ~CCountedPointer() {
        if (--*Owners == 0) {
            delete Thing;
            delete Owners;
        }
    }
    T* Get() const { return Thing; }
private:
    T* Thing;
    int* Owners;
};

class CVillageTaskTorch {
public:
    explicit CVillageTaskTorch(CThing& thing) : Torch(&thing), Taken(false) {}
    CThing* Torch;
    bool Taken;
};

class CTCVillage {
public:
    typedef std::list<CCountedPointer<CVillageTaskTorch> > TTaskList;
    CTCVillage() : TorchTasksUpdateFrame(0), LightActiveStateToChange(true),
        MaxFrameToAssignTask(10), TaskRemovals(0) { World.Time.Day = false; }
    void UpdateTorchTasks();
    long TorchTasksUpdateFrame;
    bool LightActiveStateToChange;
    long MaxFrameToAssignTask;
    int TaskRemovals;
    std::list<CIntelligentPointer<CThing> > VillageTorches;
    std::list<CIntelligentPointer<CThing> > VillagePeople;
    TTaskList TorchTasks;
    CWorld World;

private:
    const CWorld& PeekWorld() const { return World; }
    bool IsThingInTaskList(const CThing& thing, TTaskList& tasks) {
        return Find(thing, tasks) != tasks.end();
    }
    bool IsTaskTaken(const CThing& thing, TTaskList& tasks) {
        TTaskList::iterator pos = Find(thing, tasks);
        assert(pos != tasks.end());
        return pos->Get()->Taken;
    }
    void RemoveThingFromTaskList(const CThing& thing, TTaskList& tasks) {
        TTaskList::iterator pos = Find(thing, tasks);
        assert(pos != tasks.end());
        tasks.erase(pos);
        ++TaskRemovals;
    }
    TTaskList::iterator Find(const CThing& thing, TTaskList& tasks) {
        TTaskList::iterator it = tasks.begin();
        while (it != tasks.end() && it->Get()->Torch != &thing)
            ++it;
        return it;
    }
};

void CTCVillage::UpdateTorchTasks()
{
    static long next_frame_update = CMainGameComponent::GetConstantFPS() * 5;
    if (TorchTasksUpdateFrame < CWorld::GetFrame()) {
        TorchTasksUpdateFrame = CWorld::GetFrame() + next_frame_update;
        bool light_active_state_to_change = !PeekWorld().PeekGameTimeManager().IsDay();
        if (light_active_state_to_change != LightActiveStateToChange) {
            LightActiveStateToChange = light_active_state_to_change;
            MaxFrameToAssignTask = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 60;
        }
        for (std::list<CIntelligentPointer<CThing> >::iterator it = VillageTorches.begin(); it != VillageTorches.end(); ++it) {
            CThing* ptorch = it->Get();
            CTCLight* ptc_light;
            FABLE_RETAIL_ASSERT(ptorch != 0);
            if (!ptorch->IsDead() && ptorch->GetTC(&ptc_light)) {
                if (ptc_light->IsActive() != light_active_state_to_change && !IsThingInTaskList(*ptorch, TorchTasks)) {
                    if (VillagePeople.size() == 0)
                        ptc_light->SetActive(!ptc_light->IsActive());
                    else
                        TorchTasks.push_back(CCountedPointer<CVillageTaskTorch>(new CVillageTaskTorch(*ptorch)));
                } else if (ptc_light->IsActive() == light_active_state_to_change && IsThingInTaskList(*ptorch, TorchTasks)) {
                    RemoveThingFromTaskList(*ptorch, TorchTasks);
                } else if (ptc_light->IsActive() != light_active_state_to_change &&
                           IsThingInTaskList(*ptorch, TorchTasks) && !IsTaskTaken(*ptorch, TorchTasks)) {
                    FABLE_RETAIL_ASSERT(MaxFrameToAssignTask != -1);
                    if (CWorld::GetFrame() > MaxFrameToAssignTask) {
                        ptc_light->SetActive(!ptc_light->IsActive());
                        RemoveThingFromTaskList(*ptorch, TorchTasks);
                    }
                }
            }
        }
    }
}

int main()
{
    for (int mode = 0; mode != 11; ++mode) {
        CThing torch;
        CThing person;
        CTCVillage village;
        village.VillageTorches.push_back(CIntelligentPointer<CThing>(&torch));
        CWorld::Frame = 100;

        if (mode == 0)
            village.TorchTasksUpdateFrame = CWorld::Frame;
        else if (mode == 1)
            torch.Dead = true;
        else if (mode == 2)
            torch.HasLight = false;
        else if (mode == 4)
            village.VillagePeople.push_back(CIntelligentPointer<CThing>(&person));
        else if (mode >= 5 && mode <= 8) {
            village.TorchTasks.push_back(CCountedPointer<CVillageTaskTorch>(new CVillageTaskTorch(torch)));
            village.TorchTasks.front().Get()->Taken = mode == 6;
            torch.Light.Active = mode == 5;
            if (mode == 7)
                village.MaxFrameToAssignTask = CWorld::Frame;
        } else if (mode == 9) {
            village.World.Time.Day = true;
            torch.Light.Active = true;
        } else if (mode == 10) {
            torch.Light.Active = true;
        }

        village.UpdateTorchTasks();

        assert(village.TorchTasksUpdateFrame == (mode == 0 ? 100 : 250));
        assert(village.LightActiveStateToChange == (mode != 9));
        assert(village.MaxFrameToAssignTask == (mode == 7 ? 100 : mode == 9 ? 1900 : 10));
        assert(village.TaskRemovals == (mode == 5 || mode == 8 ? 1 : 0));
        assert(torch.Light.Changes == (mode == 3 || mode == 8 || mode == 9 ? 1 : 0));
        assert(torch.Light.Active == (mode == 3 || mode == 5 || mode == 8 || mode == 10));
        assert(village.TorchTasks.size() == (mode == 4 || mode == 6 || mode == 7 ? 1u : 0u));
        if (!village.TorchTasks.empty())
            assert(village.TorchTasks.front().Get()->Torch == &torch);
    }
    puts("00659625_TEST PASS");
    return 0;
}