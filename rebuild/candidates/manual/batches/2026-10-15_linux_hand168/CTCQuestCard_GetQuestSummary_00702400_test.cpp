#include <cstdio>
// Behaviour model of CTCQuestCard::GetQuestSummary (0x00702400): text of the def's QuestSummary tag or the empty string
struct CEntry { long refs; long tag; int destroyed; };
struct CEntryPtr { CEntry* Data; CEntryPtr() : Data(0) {} CEntryPtr(const CEntryPtr& o) : Data(o.Data) { if (Data) ++Data->refs; } ~CEntryPtr() { if (Data && --Data->refs == 0) ++Data->destroyed; } };
static CEntry g_entry = { 1, 0, 0 };
static CEntryPtr GetPGroupEntry(unsigned long tag, bool) { CEntryPtr p; p.Data = 0; if (tag == 7) { ++g_entry.refs; g_entry.tag = 7; p.Data = &g_entry; } return p; }
struct CQuestCardDef { long QuestSummary; };
static long GetQuestSummary(const CQuestCardDef* def)
{
    if (def->QuestSummary > 0) {
        CEntryPtr entry = GetPGroupEntry(def->QuestSummary, true);
        if (entry.Data) return entry.Data->tag * 100;
    }
    return -1;   // the empty wide string
}
int main()
{
    CQuestCardDef a = { 7 }, b = { 0 }, c = { 9 };
    if (GetQuestSummary(&a) != 700 || g_entry.refs != 1 || GetQuestSummary(&b) != -1 || GetQuestSummary(&c) != -1)
    { std::printf("00702400_TEST FAIL\n"); return 1; }
    std::printf("00702400_TEST PASS\n");
    return 0;
}
