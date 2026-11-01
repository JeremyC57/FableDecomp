// Behaviour test for 0x007DCA90 CTCTavernGameCardBlackjack::~CTCTavernGameCardBlackjack (implicit): Bets' block, then every
// hand of PlayersCards and its array, then DealersCards' block are freed, then ~CTCTavernGameCardBase runs.
#include <cstdio>
#include <cstdlib>
#include <new>
static const void* g_log[16]; static int g_n = 0; static int g_baseAt = -1;
static void Free(void* p) { if (p) { g_log[g_n++] = p; std::free(p); } }
template <class T> struct Vec {
    T* start; T* finish; T* eos;
    Vec() : start(0), finish(0), eos(0) { }
    ~Vec() { for (T* p = start; p != finish; ++p) p->~T(); if (start != 0) Free(start); }
};
class CCard;
struct CTCTavernGameCardBase { virtual ~CTCTavernGameCardBase() { g_baseAt = g_n; } };
struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase {
    Vec<CCard*> DealersCards; Vec<Vec<CCard*> > PlayersCards; Vec<long> Bets;
};
int main()
{
    CTCTavernGameCardBlackjack* g = new CTCTavernGameCardBlackjack;
    g->Bets.start = (long*)std::malloc(8); g->Bets.finish = g->Bets.start + 2;
    g->DealersCards.start = (CCard**)std::malloc(16); g->DealersCards.finish = g->DealersCards.start;
    g->PlayersCards.start = (Vec<CCard*>*)std::malloc(2 * sizeof(Vec<CCard*>)); g->PlayersCards.finish = g->PlayersCards.start + 2;
    new (g->PlayersCards.start) Vec<CCard*>(); new (g->PlayersCards.start + 1) Vec<CCard*>();
    g->PlayersCards.start[1].start = (CCard**)std::malloc(8); g->PlayersCards.start[1].finish = g->PlayersCards.start[1].start;
    const void* bets = g->Bets.start; const void* hand1 = g->PlayersCards.start[1].start;
    const void* hands = g->PlayersCards.start; const void* dealers = g->DealersCards.start;
    delete g;
    if (g_n != 4 || g_log[0] != bets || g_log[1] != hand1 || g_log[2] != hands || g_log[3] != dealers || g_baseAt != 4) {
        std::printf("007dca90_TEST FAIL\n"); return 1;
    }
    std::printf("007dca90_TEST PASS\n");
    return 0;
}