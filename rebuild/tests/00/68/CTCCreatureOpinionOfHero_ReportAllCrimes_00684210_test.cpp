// Behaviour test for CTCCreatureOpinionOfHero::ReportAllCrimes (0x00684210): every unhandled crime in the known-deed lists
// and the pending list is told to the informed creature, or reported to the village; linked deeds (0x35) report the link.
#include <cstdio>

struct COpinionDeed { long Type; bool Decriminalised; bool IsCrime; long LinkedHandle; };
static COpinionDeed g_deeds[8];
static int g_informed[16], g_reported[16], g_nInformed = 0, g_nReported = 0;
static bool g_handled[16];

static bool IsCrime(const COpinionDeed* deed)
{
    if (deed->Type < 2) return false;
    if (deed->Type > 9) return deed->Type == 0x35 && deed->LinkedHandle != -1;
    return deed->IsCrime && !deed->Decriminalised;
}
struct SListNode { SListNode* next; long data; };
struct CLongList { SListNode* node; };
struct CTCVillage {
    bool IsCrimeHandled(long h) { return g_handled[h]; }
    void ReportCrime(long h) { g_reported[g_nReported++] = (int)h; }
};
struct CLog { COpinionDeed* GetDeedFromHandle(long h) { return h < 8 ? &g_deeds[h] : 0; } };
struct COpinion {
    bool hasVillage, hasGuard, hasHero;
    CLongList Pending;
    CLongList Known[92];
    void InformOfDeed(long h, bool) { g_informed[g_nInformed++] = (int)h; }
    void ReportList(CLog* log, CTCVillage* village, COpinion* informed, CLongList& deeds)
    {
        SListNode* end = deeds.node;
        SListNode* it = end->next;
        while (it != end) {
            long handle = it->data;
            it = it->next;
            COpinionDeed* deed = log->GetDeedFromHandle(handle);
            if (deed && IsCrime(deed)) {
                if (deed->Type == 0x35 && deed->LinkedHandle != -1) handle = deed->LinkedHandle;
                if (!village->IsCrimeHandled(handle)) {
                    if (informed) informed->InformOfDeed(handle, false);
                    else village->ReportCrime(handle);
                }
            }
        }
    }
    void ReportAllCrimes(COpinion* informed, CTCVillage* village, CLog* log)
    {
        if (!hasVillage || !hasGuard || !hasHero) return;
        for (int i = 0; i < 92; ++i) ReportList(log, village, informed, Known[i]);
        ReportList(log, village, informed, Pending);
    }
};

static SListNode g_sentinels[93];
static SListNode g_nodes[8];

int main()
{
    static COpinion op;
    op.hasVillage = op.hasGuard = op.hasHero = true;
    for (int i = 0; i < 92; ++i) { g_sentinels[i].next = &g_sentinels[i]; op.Known[i].node = &g_sentinels[i]; }
    g_sentinels[92].next = &g_sentinels[92]; op.Pending.node = &g_sentinels[92];
    // deeds: 1 crime, 2 decriminalised crime, 3 linked (0x35 -> 6), 4 not a crime, 5 handled crime
    g_deeds[1].Type = 3; g_deeds[1].IsCrime = true;
    g_deeds[2].Type = 4; g_deeds[2].IsCrime = true; g_deeds[2].Decriminalised = true;
    g_deeds[3].Type = 0x35; g_deeds[3].LinkedHandle = 6;
    g_deeds[4].Type = 1;
    g_deeds[5].Type = 9; g_deeds[5].IsCrime = true; g_handled[5] = true;
    // known[0]: 1, 2 ; known[91]: 3 ; pending: 4, 5
    g_nodes[0].data = 1; g_nodes[0].next = &g_nodes[1]; g_nodes[1].data = 2; g_nodes[1].next = &g_sentinels[0]; g_sentinels[0].next = &g_nodes[0];
    g_nodes[2].data = 3; g_nodes[2].next = &g_sentinels[91]; g_sentinels[91].next = &g_nodes[2];
    g_nodes[3].data = 4; g_nodes[3].next = &g_nodes[4]; g_nodes[4].data = 5; g_nodes[4].next = &g_sentinels[92]; g_sentinels[92].next = &g_nodes[3];
    CTCVillage village; CLog log; COpinion other;
    op.ReportAllCrimes(0, &village, &log);
    if (g_nReported != 2 || g_reported[0] != 1 || g_reported[1] != 6 || g_nInformed != 0) { std::printf("00684210_TEST FAIL\n"); return 1; }
    op.ReportAllCrimes(&other, &village, &log);
    if (g_nReported != 2 || g_nInformed != 2 || g_informed[0] != 1 || g_informed[1] != 6) { std::printf("00684210_TEST FAIL\n"); return 1; }
    op.hasGuard = false;
    op.ReportAllCrimes(0, &village, &log);
    if (g_nReported != 2) { std::printf("00684210_TEST FAIL\n"); return 1; }
    std::printf("00684210_TEST PASS\n");
    return 0;
}