// Behaviour test for 0x00684050 CTCCreatureOpinionOfHero::DecriminaliseAllKnownCrimes: crime deeds (types 2..9 flagged as
// crimes and not yet decriminalised, or type 0x35 with a linked deed) in KnownDeeds[1..9] and PendingDeeds are cleared,
// a linked deed's partner too.
#include <cstdio>

struct COpinionDeed { long Type; bool Decriminalised; bool IsCrime; long LinkedHandle; };
static COpinionDeed g_deeds[8];
static int g_cleared[16]; static int g_nCleared = 0;
struct Log {
    COpinionDeed* GetDeedFromHandle(long h) { return h >= 0 && h < 8 ? &g_deeds[h] : 0; }
    void DecriminaliseDeed(long h) { g_cleared[g_nCleared++] = (int)h; }
};
struct Node { Node* next; Node* prev; long data; };
struct List { Node* node; };
static bool IsCrime(const COpinionDeed* d)
{
    if (d->Type < 2) return false;
    if (d->Type > 9) return d->Type == 0x35 && d->LinkedHandle != -1;
    return d->IsCrime && !d->Decriminalised;
}
static void DecriminaliseList(Log* log, List& l)
{
    Node* end = l.node;
    for (Node* it = end->next; it != end; it = it->next) {
        long handle = it->data;
        COpinionDeed* deed = log->GetDeedFromHandle(handle);
        if (deed && IsCrime(deed)) {
            if (deed->Type == 0x35) {
                long linked = deed->LinkedHandle;
                if (linked != -1) { long first = handle; handle = linked; log->DecriminaliseDeed(first); }
            }
            log->DecriminaliseDeed(handle);
        }
    }
}
int main()
{
    COpinionDeed d0 = { 3, false, true, -1 }, d1 = { 3, true, true, -1 }, d2 = { 1, false, true, -1 },
                 d3 = { 0x35, false, false, 5 }, d4 = { 0x35, false, false, -1 }, d5 = { 12, false, true, -1 };
    g_deeds[0] = d0; g_deeds[1] = d1; g_deeds[2] = d2; g_deeds[3] = d3; g_deeds[4] = d4; g_deeds[5] = d5;
    Node s1, s2, a, b, c, e, f;
    s1.next = &a; a.next = &b; b.next = &c; c.next = &s1; a.data = 0; b.data = 1; c.data = 3;
    s2.next = &e; e.next = &f; f.next = &s2; e.data = 4; f.data = 2;
    List known[10]; List pending; Node empty; empty.next = &empty;
    for (int i = 0; i < 10; ++i) known[i].node = &empty;
    known[4].node = &s1; pending.node = &s2;
    Log log;
    for (int i = 1; i < 10; ++i) DecriminaliseList(&log, known[i]);
    DecriminaliseList(&log, pending);
    bool ok = g_nCleared == 3 && g_cleared[0] == 0 && g_cleared[1] == 3 && g_cleared[2] == 5;
    if (!ok) { std::printf("00684050_TEST FAIL\n"); return 1; }
    std::printf("00684050_TEST PASS\n");
    return 0;
}
