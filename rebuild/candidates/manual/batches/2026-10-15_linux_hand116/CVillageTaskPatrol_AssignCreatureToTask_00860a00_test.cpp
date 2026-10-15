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
struct CVillageTaskPatrol {
    virtual long TaskType() { return 1; }
    void* m_pVillage; CIntelligentPointer PatrolStart; CIntelligentPointer CreatureAssignedToTask; bool TaskLocked;
    CVillageTaskPatrol() : m_pVillage(0), TaskLocked(true) {}
    void AssignCreatureToTask(const CThing& creature)
    {
        TaskLocked = false;
        CreatureAssignedToTask = CIntelligentPointer(const_cast<CThing*>(&creature));
    }
};
int main()
{
    int ok = 1;
    CThing creature;
    CVillageTaskPatrol t;
    int live = g_live;
    t.AssignCreatureToTask(creature);
    if (t.TaskLocked || t.CreatureAssignedToTask.GetPItem() != &creature || t.PatrolStart.GetPItem() != 0 || g_live != live) ok = 0;
    if (!ok) { std::printf("00860a00_TEST FAIL\n"); return 1; }
    std::printf("00860a00_TEST PASS\n");
    return 0;
}
