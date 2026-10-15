// Behaviour test for 0x004b1080 (CQuestManager::IsAnyQuestActive): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Def { long refs; bool vignette; };
struct TC { Def* def; long number; };
struct Card { TC* tc; };
static bool AnyActive(Card* cards, int n)
{ for (int i = 0; i < n; ++i) { TC* tc = cards[i].tc; if (tc) { Def* d = tc->def; if (d) ++d->refs; bool r = d && !d->vignette && tc->number <= -1; if (d) --d->refs; if (r) return true; } } return false; }

int main()
{
    Def dv = { 1, true }, dn = { 1, false }; TC t1 = { &dv, -1 }, t2 = { &dn, 4 }, t3 = { &dn, -1 }, t4 = { 0, -1 };
    Card a[3] = { { &t1 }, { &t2 }, { &t4 } }; CHECK(!AnyActive(a, 3));
    Card b[2] = { { 0 }, { &t3 } }; CHECK(AnyActive(b, 2)); CHECK(dv.refs == 1 && dn.refs == 1);
    if (g_fail) { std::printf("004b1080_TEST FAIL\n"); return 1; }
    std::printf("004b1080_TEST PASS\n");
    return 0;
}
