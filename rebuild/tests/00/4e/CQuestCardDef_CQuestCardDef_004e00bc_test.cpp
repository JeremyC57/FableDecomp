// Behaviour test for 004e00bc: body copied from src/004e00bc.cpp (CQuestCardDef ctor), callees stubbed.
#include <new>
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_baseCtor = 0, g_mapCtor = 0;
struct CDefBase_0044c0c0 { virtual void v00() {} unsigned char _base[0x24]; CDefBase_0044c0c0() { ++g_baseCtor; } };
template <class T>
struct CAllocProxy {                            // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    T* p;
    CAllocProxy(T* v) : p(v) { }
};
template <class T>
struct CRetailVector {                          // STLport _Vector_base: _M_start / _M_finish / _M_end_of_storage
    T* first;
    T* last;
    CAllocProxy<T> end;
    CRetailVector() : first(0), last(0), end(0) { }
};
struct CDefString { long m_index; CDefString() : m_index(-1) { } };
struct CPrerequisiteMap { long m_words[4]; CPrerequisiteMap() { ++g_mapCtor; m_words[0] = 5; } };
struct CQuestCardDef : CDefBase_0044c0c0 {
    long QuestName; long QuestSummary; long QuestObjective; long SuccessSummary; CDefString RegionName; CDefString TeleporterRegionName;
    long InventoryCategory; long RenownReward; long GoldReward; CRetailVector<long> RewardObjects; bool IsCore; bool IsVignette; bool IsExclusive;
    bool MakeVignetteRouteAppearOnMinimap; CPrerequisiteMap Prerequisites;
    virtual void v00() {}
    CQuestCardDef();
};
CQuestCardDef::CQuestCardDef()
    : CDefBase_0044c0c0()
{
}
int main()
{
    unsigned char raw[sizeof(CQuestCardDef)]; memset(raw, 0xAA, sizeof(raw));
    CQuestCardDef* def = new (raw) CQuestCardDef();
    CHECK(g_baseCtor == 1 && g_mapCtor == 1 && def->Prerequisites.m_words[0] == 5);
    CHECK(def->RegionName.m_index == -1 && def->TeleporterRegionName.m_index == -1);
    CHECK(def->RewardObjects.first == 0 && def->RewardObjects.last == 0 && def->RewardObjects.end.p == 0);
    CHECK(def->GoldReward == (long)0xAAAAAAAA && *(unsigned char*)&def->IsCore == 0xAA);   // bytes left as the allocator gave them
    if (fails == 0) printf("004e00bc_TEST PASS\n"); else printf("004e00bc_TEST FAIL\n");
    return fails;
}