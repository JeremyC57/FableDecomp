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
struct CVillageTaskCarryCrate : CVillageTask {
    CIntelligentPointer CreatureAssignedToTask; CIntelligentPointer PCrateStack; bool Pickup; long Priority;
    virtual long TaskType() { return 5; }
    CVillageTaskCarryCrate(CThing* crateStack, bool pickup, long priority) : PCrateStack(crateStack), Pickup(pickup), Priority(priority) {}
};
int main()
{
    int ok = 1;
    CThing crate;
    CVillageTaskCarryCrate t(&crate, true, 7);
    if (t.CreatureAssignedToTask.GetPItem() != 0 || t.PCrateStack.GetPItem() != &crate || !t.Pickup || t.Priority != 7) ok = 0;
    if (t.TaskType() != 5 || t.CreatureAssignedToTask.Kind() != 1 || g_live != 2) ok = 0;
    if (!ok) { std::printf("0065ef94_TEST FAIL\n"); return 1; }
    std::printf("0065ef94_TEST PASS\n");
    return 0;
}
