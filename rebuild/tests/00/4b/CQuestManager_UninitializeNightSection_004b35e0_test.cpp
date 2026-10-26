#include <cstdio>
#include <cstring>
#include <cstdlib>
// behaviour test: CQuestManager::UninitializeNightSection deletes the things of every "NightOnly" section once, then clears the flag
class CCharStringData { public: const char* m_pData; unsigned long m_length; };
static long Compare_00411570(const char* a, const char* b) { return std::strcmp(a, b); }
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(CCharStringData* d) : PStringData(d) {}
    CCharString(const CCharString& o) : PStringData(o.PStringData) {}
    bool SameText(const CCharStringData& other) const
    {
        if (PStringData->m_length != other.m_length)
            return false;
        return Compare_00411570(PStringData->m_pData, other.m_pData) ? false : true;
    }
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return SameText(*other.PStringData);
    }
};
static CCharStringData g_dayData = { "DayOnly", 7 }, g_nightData = { "NightOnly", 9 }, g_noneData = { "", 0 };
static CCharString g_DayOnlySuffix(&g_dayData), g_NightOnlySuffix(&g_nightData);
struct CSectionNode { unsigned char Color; CSectionNode* Parent; CSectionNode* Left; CSectionNode* Right; CCharString Value;
    CSectionNode() : Value(0) {} };
struct CSectionSet {
    CSectionNode* Head; unsigned long Count;
    CSectionSet() { Head = new CSectionNode; Head->Parent = 0; Head->Left = Head->Right = Head; Count = 0; }
    ~CSectionSet() { delete Head; }
    static CSectionNode* Increment(CSectionNode* node)
    {
        if (node->Right != 0) { node = node->Right; while (node->Left != 0) node = node->Left; }
        else { CSectionNode* y = node->Parent; while (node == y->Right) { node = y; y = y->Parent; } if (node->Right != y) node = y; }
        return node;
    }
};
static const char* g_deleted[8]; static int g_deletedCount = 0;
class CThingManager { public: void DeleteThingsWithSectionName(const CCharString& s) { g_deleted[g_deletedCount++] = s.PStringData->m_pData; } };
static CThingManager g_things;
class CWorld { public: CThingManager* GetThingManager() { return &g_things; } };
static CCharStringData g_names[3] = { { "A_DayOnly", 9 }, { "B_NightOnly", 11 }, { "C", 1 } };
static CSectionNode g_nodes[3];
class CQuestManager { public:
    CWorld* World; bool DayActive; bool NightActive; int builds;
    void BuildLoadSections(CSectionSet* sections)
    {
        ++builds;
        for (int i = 0; i < 3; ++i) { g_nodes[i].Value = CCharString(&g_names[i]); g_nodes[i].Left = g_nodes[i].Right = 0; }
        CSectionNode* h = sections->Head;          // B at the root, A left, C right
        h->Parent = &g_nodes[1]; h->Left = &g_nodes[0]; h->Right = &g_nodes[2];
        g_nodes[1].Parent = h; g_nodes[1].Left = &g_nodes[0]; g_nodes[1].Right = &g_nodes[2];
        g_nodes[0].Parent = &g_nodes[1]; g_nodes[2].Parent = &g_nodes[1];
        sections->Count = 3;
    }
    CCharString GetDayOrNightOnlyStringFromFullSectionName(const CCharString& section)
    {
        const char* t = section.PStringData->m_pData;
        if (std::strstr(t, "NightOnly")) return CCharString(&g_nightData);
        if (std::strstr(t, "DayOnly")) return CCharString(&g_dayData);
        return CCharString(&g_noneData);
    }
    void UninitializeNightSection();
};
void CQuestManager::UninitializeNightSection()
{
    if (!NightActive)
        return;
    CSectionSet sections;
    BuildLoadSections(&sections);
    for (CSectionNode* node = sections.Head->Left; node != sections.Head; node = CSectionSet::Increment(node)) {
        CCharString section(node->Value);
        bool nightOnly = GetDayOrNightOnlyStringFromFullSectionName(section) == g_NightOnlySuffix;
        if (nightOnly)
            World->GetThingManager()->DeleteThingsWithSectionName(section);
    }
    NightActive = false;
}

int main()
{
    CWorld world; CQuestManager q; q.World = &world; q.builds = 0; q.DayActive = false; q.NightActive = false;
    q.UninitializeNightSection();               // inactive: nothing
    bool ok = q.builds == 0 && g_deletedCount == 0;
    q.NightActive = true;
    q.UninitializeNightSection();
    ok = ok && q.builds == 1 && g_deletedCount == 1 && std::strstr(g_deleted[0], "NightOnly") != 0 && !q.NightActive;
    if (!ok) { std::printf("004b35e0_TEST FAIL\n"); return 1; }
    std::printf("004b35e0_TEST PASS\n");
    return 0;
}