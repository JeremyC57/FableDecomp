#include <cstdio>
#include <cstring>
struct CThing { int id; };
static int g_sets, g_live;
struct CBaseIntelligentPointer {
    virtual long Kind() { return 0; }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_live; }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; ++g_sets; }
    ~CBaseIntelligentPointer() { --g_live; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual long Kind() { return 1; }
    CIntelligentPointer() {}
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CVillageTask { virtual long TaskType() { return 0; } void* m_pVillage; CVillageTask() : m_pVillage(0) {} };
struct CVillageTaskPatrol : CVillageTask {
    CIntelligentPointer PatrolStart; CIntelligentPointer CreatureAssignedToTask; bool TaskLocked;
    virtual long TaskType() { return 1; }
    CVillageTaskPatrol(CThing* start) : PatrolStart(start), TaskLocked(false) {}
};
int main()
{
    int ok = 1;
    CThing node;
    CVillageTaskPatrol t(&node);
    if (t.PatrolStart.GetPItem() != &node || t.CreatureAssignedToTask.GetPItem() != 0 || t.TaskLocked || t.TaskType() != 1) ok = 0;
    if (!ok) { std::printf("0065ee66_TEST FAIL\n"); return 1; }
    std::printf("0065ee66_TEST PASS\n");
    return 0;
}