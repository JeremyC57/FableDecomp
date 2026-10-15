// Behaviour test for 0x004b1110 (CQuestManager::IsBetweenQuestStartAndFinishScreensIgnoringVignettes): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Card { bool dead; bool hasTC; bool vignette; };
static bool IsBetween(Card** cards, unsigned long n)
{ for (unsigned long i = 0; i < n; ++i) { Card* c = cards[i]; if (c && !c->dead && c->hasTC) { if (!c->vignette) return true; } } return false; }

int main()
{
    Card v = { false, true, true }, d = { true, true, false }, nt = { false, false, false }, ok = { false, true, false };
    Card* l1[4] = { 0, &v, &d, &nt }; CHECK(!IsBetween(l1, 4));
    Card* l2[3] = { &v, &ok, 0 }; CHECK(IsBetween(l2, 3)); CHECK(!IsBetween(l2, 0));
    if (g_fail) { std::printf("004b1110_TEST FAIL\n"); return 1; }
    std::printf("004b1110_TEST PASS\n");
    return 0;
}