// Behaviour test for 004b3810: body copied from src/004b3810.cpp (CQuestManager::SetCreatureGenerationEnabledDuringScript), callees stubbed.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CCharString { public: void* PStringData; };
struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };
static int g_find = 0; static const CCharString* g_findValue = 0; static CCharString* g_findResult = 0;
static CCharString* __fastcall FindString_004b8e40(CCharString* first, CCharString* last, const CCharString& value, const SRandomAccessIteratorTag& tag)
{
    ++g_find; g_findValue = &value; (void)tag;
    for (CCharString* it = first; it != last; ++it)
        if (it->PStringData == value.PStringData) return g_findResult = it;
    return g_findResult = last;
}
static int g_pushBack = 0, g_erase = 0; static const CCharString* g_pushed = 0; static CCharString* g_erased = 0;
struct CCharStringVector {
    CCharString* m_pFirst; CCharString* m_pLast; CCharString* m_pEnd;
    void push_back(const CCharString& value) { ++g_pushBack; g_pushed = &value; }
    CCharString* erase(CCharString* where) { ++g_erase; g_erased = where; return where; }
};
class CQuestManager { public:
    unsigned char     _pad_0x00[0x40];
    CCharStringVector CreatureGenerationDisabledQuests;   // +0x40
    void SetCreatureGenerationEnabledDuringScript(const CCharString& script, bool enabled);
};
void CQuestManager::SetCreatureGenerationEnabledDuringScript(const CCharString& script, bool enabled)
{
    CCharString* last = CreatureGenerationDisabledQuests.m_pLast;
    CCharStringVector* quests = &CreatureGenerationDisabledQuests;
    CCharString* it = FindString_004b8e40(quests->m_pFirst, last, script, SRandomAccessIteratorTag());
    if (it == last) {
        if (!enabled)
            quests->push_back(script);
    } else if (enabled) {
        quests->erase(it);
    }
}
int main()
{
    CQuestManager qm; CCharString list[3]; list[0].PStringData = (void*)1; list[1].PStringData = (void*)2; list[2].PStringData = (void*)3;
    qm.CreatureGenerationDisabledQuests.m_pFirst = list; qm.CreatureGenerationDisabledQuests.m_pLast = list + 3; qm.CreatureGenerationDisabledQuests.m_pEnd = list + 3;
    CCharString absent; absent.PStringData = (void*)9;
    // absent + disable -> push_back
    qm.SetCreatureGenerationEnabledDuringScript(absent, false);
    CHECK(g_find == 1 && g_findValue == &absent && g_pushBack == 1 && g_pushed == &absent && g_erase == 0);
    // absent + enable -> nothing
    qm.SetCreatureGenerationEnabledDuringScript(absent, true);
    CHECK(g_find == 2 && g_pushBack == 1 && g_erase == 0);
    // present + enable -> erase at the found slot
    qm.SetCreatureGenerationEnabledDuringScript(list[1], true);
    CHECK(g_find == 3 && g_erase == 1 && g_erased == list + 1 && g_pushBack == 1);
    // present + disable -> nothing
    qm.SetCreatureGenerationEnabledDuringScript(list[2], false);
    CHECK(g_find == 4 && g_erase == 1 && g_pushBack == 1);
    if (fails == 0) printf("004b3810_TEST PASS\n"); else printf("004b3810_TEST FAIL (%d)\n", fails);
    return fails;
}