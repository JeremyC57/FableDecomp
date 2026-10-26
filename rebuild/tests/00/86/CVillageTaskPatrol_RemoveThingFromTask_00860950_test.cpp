// Behaviour test for 0x00860950 CVillageTaskPatrol::RemoveThingFromTask: start-node hit clears the start and asks for
// removal; any other thing clears the assigned creature; the task is always unlocked.
#include <cstdio>
class CBaseObject { public: int id; };
class CThing : public CBaseObject { public: };
class CThingTrackNode : public CThing { public: };
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
class CVillageTaskPatrol { public:
    CIntelligentPointer<const CThingTrackNode> PatrolStart;
    CIntelligentPointer<const CThing> CreatureAssignedToTask;
    bool TaskLocked;
    CVillageTaskPatrol() : PatrolStart(0), CreatureAssignedToTask(0), TaskLocked(true) { }
    void RemoveThingFromTask(const CThing& thing, bool& removeTask)
    {
        TaskLocked = false;
        if (PatrolStart.GetPItem() == &thing) { PatrolStart = CIntelligentPointer<const CThingTrackNode>(0); removeTask = true; }
        else { CreatureAssignedToTask = CIntelligentPointer<const CThing>(0); removeTask = false; }
    }
};
int main()
{
    CThingTrackNode node; CThing creature;
    CVillageTaskPatrol t; t.PatrolStart.SetPItem(&node); t.CreatureAssignedToTask.SetPItem(&creature);
    bool remove = false;
    t.RemoveThingFromTask(creature, remove);
    bool ok = !remove && !t.TaskLocked && t.CreatureAssignedToTask.GetPItem() == 0 && t.PatrolStart.GetPItem() == &node;
    t.TaskLocked = true; t.CreatureAssignedToTask.SetPItem(&creature);
    t.RemoveThingFromTask(node, remove);
    ok = ok && remove && !t.TaskLocked && t.PatrolStart.GetPItem() == 0 && t.CreatureAssignedToTask.GetPItem() == &creature;
    if (!ok) { std::printf("00860950_TEST FAIL\n"); return 1; }
    std::printf("00860950_TEST PASS\n");
    return 0;
}