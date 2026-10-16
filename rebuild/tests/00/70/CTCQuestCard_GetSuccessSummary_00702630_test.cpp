#include <cstdio>
// Behaviour model of CTCQuestCard::GetSuccessSummary (0x00702630): text of the def's SuccessSummary tag or the empty string
struct CEntry { long refs; long tag; int destroyed; };
struct CEntryPtr { CEntry* Data; CEntryPtr() : Data(0) {} CEntryPtr(const CEntryPtr& o) : Data(o.Data) { if (Data) ++Data->refs; } ~CEntryPtr() { if (Data && --Data->refs == 0) ++Data->destroyed; } };
static CEntry g_entry = { 1, 0, 0 };
static CEntryPtr GetPGroupEntry(unsigned long tag, bool) { CEntryPtr p; p.Data = 0; if (tag == 7) { ++g_entry.refs; g_entry.tag = 7; p.Data = &g_entry; } return p; }
struct CQuestCardDef { long SuccessSummary; };
static long GetSuccessSummary(const CQuestCardDef* def)
{
    if (def->SuccessSummary > 0) {
        CEntryPtr entry = GetPGroupEntry(def->SuccessSummary, true);
        if (entry.Data) return entry.Data->tag * 100;
    }
    return -1;   // the empty wide string
}
int main()
{
    CQuestCardDef a = { 7 }, b = { 0 }, c = { 9 };
    if (GetSuccessSummary(&a) != 700 || g_entry.refs != 1 || GetSuccessSummary(&b) != -1 || GetSuccessSummary(&c) != -1)
    { std::printf("00702630_TEST FAIL\n"); return 1; }
    std::printf("00702630_TEST PASS\n");
    return 0;
}