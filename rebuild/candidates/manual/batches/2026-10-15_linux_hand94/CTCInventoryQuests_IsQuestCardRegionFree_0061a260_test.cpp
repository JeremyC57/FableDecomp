#include <cstdio>
#include <cstring>
// Behaviour test for 0x0061A260: !IsActiveQuestStartedInRegion(GetString(card->PDef->RegionName)), def held during the query.
static int g_destroyed = 0; static long g_refDuring = -1;
struct CCharString { char m_text[16]; };
struct CDefStringTable { CCharString GetString(long id) { CCharString s; std::sprintf(s.m_text, "R%ld", id); return s; } };
static CDefStringTable g_DefStringTable;
struct CQuestCardDef;
static CQuestCardDef* g_def = 0;
struct CQuestCardDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long refCount;
    long RegionName;
    void AddRef() { ++refCount; }
    void Release() { if (--refCount == 0) Destroy(); }
};
struct CQuestManager {
    const char* activeRegion;
    bool IsActiveQuestStartedInRegion(const CCharString& region) { g_refDuring = g_def->refCount; return std::strcmp(region.m_text, activeRegion) == 0; }
};
static CQuestManager* g_pQuestManager = 0;
struct CQuestCardDefPointer {
    CQuestCardDef* Object;
    CQuestCardDefPointer(CQuestCardDef* p) : Object(p) {}
    CQuestCardDefPointer(const CQuestCardDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CQuestCardDefPointer() { Object->Release(); }
    CQuestCardDef* operator->() const { return Object; }
};
struct CTCBase { virtual void v00() {} void* ParentThing; };
struct CTCQuestCard : CTCBase { CQuestCardDefPointer PDef; CTCQuestCard(CQuestCardDef* d) : PDef(d) {} };
struct CTCInventoryQuests { static unsigned char IsQuestCardRegionFree(CTCQuestCard* card); };
unsigned char CTCInventoryQuests::IsQuestCardRegionFree(CTCQuestCard* card)
{
    bool started;
    {
        CQuestCardDefPointer def(card->PDef);
        CCharString region = g_DefStringTable.GetString(def->RegionName);
        started = g_pQuestManager->IsActiveQuestStartedInRegion(region);
    }
    return !started;
}
int main()
{
    CQuestCardDef def; def.refCount = 1; def.RegionName = 7; g_def = &def;
    CQuestManager qm; qm.activeRegion = "R7"; g_pQuestManager = &qm;
    CTCQuestCard card(&def);
    if (CTCInventoryQuests::IsQuestCardRegionFree(&card) != 0 || g_refDuring != 2 || def.refCount != 1) { std::printf("0061a260_TEST FAIL\n"); return 1; }
    qm.activeRegion = "R3";
    if (CTCInventoryQuests::IsQuestCardRegionFree(&card) != 1 || def.refCount != 1 || g_destroyed != 0) { std::printf("0061a260_TEST FAIL\n"); return 1; }
    std::printf("0061a260_TEST PASS\n");
    return 0;
}
