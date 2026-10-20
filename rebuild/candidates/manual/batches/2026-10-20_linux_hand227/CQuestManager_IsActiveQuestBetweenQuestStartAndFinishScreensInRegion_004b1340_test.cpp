// Behaviour test for CQuestManager::IsActiveQuestBetweenQuestStartAndFinishScreensInRegion (0x004B1340): an empty
// region is never matched; otherwise true only for a live, non-vignette quest card of a between-screens quest whose
// region name equals the given one (shared block, or equal length and text); the region temporaries are released.
#include <cstdio>
#include <cstring>

static int g_live = 0;
static long Compare_00411570(const char* a, const char* b) { return std::strcmp(a, b); }
struct CCharStringData { const char* Data; unsigned long Length;
    bool IsEqual(const CCharStringData& o) const { if (Length != o.Length) return false; return Compare_00411570(Data, o.Data) == 0; } };
struct CCharString {
    CCharStringData* PStringData;
    CCharString(CCharStringData* d) : PStringData(d) { ++g_live; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_live; }
    ~CCharString() { --g_live; }
    bool operator!=(const char* t) const { return PStringData && std::strcmp(PStringData->Data, t) != 0; }
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = PStringData; CCharStringData* b = other.PStringData;
        if (a == b) return true;
        if (a == 0 || b == 0) return false;
        return a->IsEqual(*b);
    }
};
struct CTCQuestCard { bool vignette; CCharStringData* region;
    bool IsVignette() const { return vignette; } CCharString GetRegionName() { return CCharString(region); } };
struct CThing { bool dead; CTCQuestCard* tc; bool HasTC(long k) const { return k == 0x6c && tc != 0; } };
static CThing* g_cards[3];
template <class T> struct CRetailVector { T* _M_start; T* _M_finish;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); } T& operator[](unsigned long n) { return *(_M_start + n); } };
struct CQuestManager {
    CRetailVector<int> BetweenStartAndEndScreenQuests;
    CThing* GetActiveQuestCardFromScriptName(int i) { return g_cards[i]; }
    bool IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(const CCharString& region);
};
bool CQuestManager::IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(const CCharString& region)
{
    if (region != "") {
        for (unsigned long i = 0; i < BetweenStartAndEndScreenQuests.size(); ++i) {
            CThing* card = GetActiveQuestCardFromScriptName(BetweenStartAndEndScreenQuests[i]);
            if (card && !card->dead) {
                CTCQuestCard* tc = 0;
                if (card->HasTC(0x6c))
                    tc = card->tc;
                if (!tc->IsVignette()) {
                    if (tc->GetRegionName() == region)
                        return true;
                }
            }
        }
    }
    return false;
}
int main()
{
    int bad = 0;
    CCharStringData oak = { "Oakvale", 7 }, oak2 = { "Oakvale", 7 }, bow = { "Bowerstone", 10 }, empty = { "", 0 };
    CTCQuestCard vig = { true, &oak }, other = { false, &bow }, match = { false, &oak2 };
    CThing c0 = { false, &vig }, c1 = { false, &other }, c2 = { false, &match };
    g_cards[0] = &c0; g_cards[1] = &c1; g_cards[2] = &c2;
    int idx[3] = { 0, 1, 2 };
    CQuestManager m; m.BetweenStartAndEndScreenQuests._M_start = idx; m.BetweenStartAndEndScreenQuests._M_finish = idx + 3;
    CCharString rOak(&oak), rBow(&bow), rEmpty(&empty);
    int base = g_live;
    if (!m.IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(rOak)) bad = 1;      // card 2 (equal text)
    if (!m.IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(rBow)) bad = 1;      // card 1 (same block)
    if (m.IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(rEmpty)) bad = 1;
    c2.dead = true;
    if (m.IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(rOak)) bad = 1;       // only the vignette left
    if (g_live != base) bad = 1;
    if (bad) { std::printf("004b1340_TEST FAIL\n"); return 1; }
    std::printf("004b1340_TEST PASS\n");
    return 0;
}
