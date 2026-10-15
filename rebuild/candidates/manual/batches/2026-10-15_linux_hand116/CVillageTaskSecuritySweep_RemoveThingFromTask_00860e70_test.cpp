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
struct CVillageTaskSecuritySweep {
    virtual long TaskType() { return 2; }
    void* m_pVillage; CIntelligentPointer CreatureAssignedToTask;
    void RemoveThingFromTask(const CThing& thing, bool& stillValid)
    {
        if (CreatureAssignedToTask.GetPItem() == &thing) {
            CreatureAssignedToTask = CIntelligentPointer(0);
            stillValid = false;
        }
    }
};
int main()
{
    int ok = 1;
    CThing a, b;
    CVillageTaskSecuritySweep t; t.m_pVillage = 0; t.CreatureAssignedToTask.SetPItem(&a);
    bool valid = true;
    t.RemoveThingFromTask(b, valid);
    if (!valid || t.CreatureAssignedToTask.GetPItem() != &a) ok = 0;
    t.RemoveThingFromTask(a, valid);
    if (valid || t.CreatureAssignedToTask.GetPItem() != 0) ok = 0;
    if (!ok) { std::printf("00860e70_TEST FAIL\n"); return 1; }
    std::printf("00860e70_TEST PASS\n");
    return 0;
}
