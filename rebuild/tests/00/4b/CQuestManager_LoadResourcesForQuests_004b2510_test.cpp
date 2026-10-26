// Behaviour test for 004b2510: src/004b2510.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CQuestManager::LoadResourcesForQuests(const vector<CCharString>&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CQuestManager::LoadResourcesForQuests(const vector<CCharString>& questSections) -- gather every quest section name (plus its "%DayOnly" / "%NightOnly" variants while day / night is active) into a set, then have the world map load the things in those sections and update its nav maps
// Leaf: LoadResourcesForQuests
// Shape: the three set inserts drop their pair<iterator,bool> results; a same-TU noinline stand-in body for the insert
//   0x004AC380 (as the landed UpdateHiddenMaterials 0x00706510) lets VC7.1 pack the three results into one 8-byte frame
//   slot like retail's link-time code generation (declaration only: three slots, 0x34 frame).
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);               // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
extern CCharString g_DayOnlySuffix;                  // 0x013BAC18 "DayOnly"
extern CCharString g_NightOnlySuffix;                // 0x013BAC1C "NightOnly"

struct CSectionNode { unsigned char Color; CSectionNode* Parent; CSectionNode* Left; CSectionNode* Right; CCharString Value; };
struct CSectionInsertResult { CSectionNode* Position; bool Inserted; CSectionInsertResult() {} };
struct CSectionSet {                                 // STLport set<CCharString> (12 bytes)
    CSectionNode* Head;
    unsigned long Count;
    unsigned long Compare;
    CSectionSet()
    {
        Head = 0;
        Head = (CSectionNode*)malloc(20);
        Count = 0;
        Head->Color = 0;
        Head->Parent = 0;
        Head->Left = Head;
        Head->Right = Head;
    }
    CSectionInsertResult Insert(const CCharString& value);   // 0x004AC380
    void Erase(CSectionNode* node);                  // 0x004ABA70
    ~CSectionSet()
    {
        if (Count) {
            Erase(Head->Parent);
            Head->Left = Head;
            Head->Parent = 0;
            Head->Right = Head;
            Count = 0;
        }
        if (Head)
            free(Head);
    }
};
struct CCharStringVector {                           // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const CCharString& operator[](unsigned int i) const { return _M_start[i]; }
};
class CWorldMap { public:
    void LoadThingsWithSectionNames(const CSectionSet& sections, bool a2);   // 0x00500EB0
    void UpdateNavMaps(const CSectionSet& sections, bool a2);                // 0x00501990
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                // slot 12 (+0x30)
};
class CQuestManager { public:
    unsigned char _pad00[0x7c];
    CWorld*       World;                             // +0x7c
    unsigned char _pad80[0x0c];
    bool          DayActive;                         // +0x8c
    bool          NightActive;                       // +0x8d
    void LoadResourcesForQuests(const CCharStringVector& questSections);
};

CSectionNode* InsertNode_004ac380(CSectionSet* set, void* key, bool* inserted);
#pragma auto_inline(off)
// Same-TU stand-in for set<CCharString>::insert 0x004AC380 (pair returned by hidden pointer), as the landed
// UpdateHiddenMaterials 0x00706510: with a body VC7.1 knows the result pointer does not escape and packs the three
// dropped pairs into one frame slot as retail's link-time code generation did. Only LoadResourcesForQuests is graded.
__declspec(noinline) CSectionInsertResult CSectionSet::Insert(const CCharString& value)
{
    CSectionInsertResult result;
    bool inserted;
    result.Position = InsertNode_004ac380(this, value.PStringData, &inserted);
    result.Inserted = inserted;
    return result;
}
#pragma auto_inline(on)
void CQuestManager::LoadResourcesForQuests(const CCharStringVector& questSections)
{
    CSectionSet sections;
    for (unsigned int i = 0; i < questSections.size(); ++i) {
        const CCharString& section = questSections[i];
        sections.Insert(section);
        if (DayActive) {
            CCharString dayName = section + "%" + g_DayOnlySuffix;
            sections.Insert(dayName);
        }
        if (NightActive) {
            CCharString nightName = section + "%" + g_NightOnlySuffix;
            sections.Insert(nightName);
        }
    }
    World->GetWorldMap()->LoadThingsWithSectionNames(sections, false);
    World->GetWorldMap()->UpdateNavMaps(sections, false);
}

// ---- stubs ----
static char g_pool[64][64]; static int g_poolN = 0, g_dtors = 0;
CCharString::~CCharString() { ++g_dtors; }
static CCharString Make(const char* a, const char* b) { char* p = g_pool[g_poolN++ % 64]; strcpy(p, a); strcat(p, b); CCharString s; s.PStringData = p; return s; }
CCharString __fastcall operator+(const CCharString& left, const char* right) { return Make((const char*)left.PStringData, right); }
CCharString __fastcall operator+(const CCharString& left, const CCharString& right) { return Make((const char*)left.PStringData, (const char*)right.PStringData); }
CCharString g_DayOnlySuffix = { (void*)"DayOnly" };
CCharString g_NightOnlySuffix = { (void*)"NightOnly" };
static char g_ins[16][64]; static int g_insN = 0, g_erases = 0, g_loads = 0, g_navs = 0, g_maps = 0;
static const CSectionSet* g_loadSet = 0; static const CSectionSet* g_navSet = 0; static unsigned long g_loadCount = 0;
CSectionNode* InsertNode_004ac380(CSectionSet* set, void* key, bool* inserted) { strcpy(g_ins[g_insN++ % 16], (const char*)key); ++set->Count; *inserted = true; return set->Head; }
void CSectionSet::Erase(CSectionNode* node) { ++g_erases; }
void CWorldMap::LoadThingsWithSectionNames(const CSectionSet& sections, bool a2) { ++g_loads; g_loadSet = &sections; g_loadCount = sections.Count; CHECK(a2 == false); }
void CWorldMap::UpdateNavMaps(const CSectionSet& sections, bool a2) { ++g_navs; g_navSet = &sections; CHECK(a2 == false); CHECK(g_loads == 1); }
static CWorldMap g_map;
struct TWorld : CWorld { virtual CWorldMap* GetWorldMap() { ++g_maps; return &g_map; } };
void CWorld::v00() {} void CWorld::v01() {} void CWorld::v02() {} void CWorld::v03() {} void CWorld::v04() {} void CWorld::v05() {}
void CWorld::v06() {} void CWorld::v07() {} void CWorld::v08() {} void CWorld::v09() {} void CWorld::v10() {} void CWorld::v11() {}
CWorldMap* CWorld::GetWorldMap() { return 0; }
static void Reset() { g_insN = g_erases = g_loads = g_navs = g_maps = 0; g_loadSet = g_navSet = 0; g_loadCount = 0; }
int main()
{
    static CQuestManager qm; static TWorld world; qm.World = &world;
    static CCharString names[2] = { { (void*)"Q1" }, { (void*)"Q2" } };
    CCharStringVector v; v._M_start = names; v._M_finish = names + 2; v._M_end_of_storage = names + 2;
    // neither day nor night: the plain sections only
    Reset(); qm.DayActive = false; qm.NightActive = false;
    qm.LoadResourcesForQuests(v);
    CHECK(g_insN == 2); CHECK(strcmp(g_ins[0], "Q1") == 0 && strcmp(g_ins[1], "Q2") == 0);
    CHECK(g_maps == 2 && g_loads == 1 && g_navs == 1); CHECK(g_loadSet == g_navSet && g_loadCount == 2);
    CHECK(g_erases == 1);   // the set destructor erases the populated tree
    // day active: each section plus its %DayOnly variant
    Reset(); qm.DayActive = true;
    qm.LoadResourcesForQuests(v);
    CHECK(g_insN == 4); CHECK(strcmp(g_ins[0], "Q1") == 0 && strcmp(g_ins[1], "Q1%DayOnly") == 0);
    CHECK(strcmp(g_ins[2], "Q2") == 0 && strcmp(g_ins[3], "Q2%DayOnly") == 0); CHECK(g_loadCount == 4);
    // day and night: three names per section, in order
    Reset(); qm.NightActive = true;
    qm.LoadResourcesForQuests(v);
    CHECK(g_insN == 6); CHECK(strcmp(g_ins[1], "Q1%DayOnly") == 0 && strcmp(g_ins[2], "Q1%NightOnly") == 0 && strcmp(g_ins[5], "Q2%NightOnly") == 0);
    // night only, empty vector: nothing inserted, maps still updated, no erase
    Reset(); qm.DayActive = false; v._M_finish = names;
    qm.LoadResourcesForQuests(v);
    CHECK(g_insN == 0 && g_loads == 1 && g_navs == 1 && g_erases == 0 && g_loadCount == 0);
    if (fails == 0) printf("004b2510_TEST PASS\n"); else printf("004b2510_TEST FAIL\n");
    return fails;
}