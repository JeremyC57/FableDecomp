// Behaviour test for CTCTavernGameCardBlackjack::GetHandValue @ 0x007DA230: visible cards summed, aces 11 -> 1 while bust.
#include <cstdio>
class CCard { public: long Value; bool Turning; bool FaceDown; };
struct CVector { CCard** first; CCard** last; };
static long GetHandValue(CVector* cards, bool includeFaceDown)
{
    long total = 0, aces = 0;
    for (CCard** it = cards->first; it != cards->last; ++it)
        if (includeFaceDown || (*it)->Turning || !(*it)->FaceDown) { long v = (*it)->Value; if (v == 11) ++aces; total += v; }
    while (total > 21 && aces > 0) { total -= 10; --aces; }
    return total;
}
int main()
{
    CCard ace1 = { 11, false, false }, ace2 = { 11, false, false }, nine = { 9, false, false }, hidden = { 10, false, true }, turning = { 5, true, true };
    CCard* h1[3] = { &ace1, &ace2, &nine }; CVector v1; v1.first = h1; v1.last = h1 + 3;
    if (GetHandValue(&v1, false) != 21) { std::printf("007da230_TEST FAIL\n"); return 1; }
    CCard* h2[3] = { &nine, &hidden, &turning }; CVector v2; v2.first = h2; v2.last = h2 + 3;
    if (GetHandValue(&v2, false) != 14 || GetHandValue(&v2, true) != 24) { std::printf("007da230_TEST FAIL\n"); return 1; }
    CVector e; e.first = h1; e.last = h1; if (GetHandValue(&e, true) != 0) { std::printf("007da230_TEST FAIL\n"); return 1; }
    std::printf("007da230_TEST PASS\n");
    return 0;
}