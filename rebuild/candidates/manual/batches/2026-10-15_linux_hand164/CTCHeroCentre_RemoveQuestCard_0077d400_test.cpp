#include <cstdio>
// Behaviour model of CTCHeroCentre::RemoveQuestCard (0x0077D400): erase the first live matching card, keep order.
struct Thing { bool dead; };
struct Vec { Thing* items[8]; int n;
    void erase(int pos) { for (int i = pos + 1; i < n; ++i) items[i - 1] = items[i]; --n; } };
static void RemoveQuestCard(Vec& v, Thing* card)
{
    for (int i = 0; i != v.n; ++i) {
        Thing* t = v.items[i];
        if (t && !t->dead && card == t) { v.erase(i); return; }
    }
}
int main()
{
    Thing a = { false }, b = { false }, c = { true };
    Vec v; v.n = 4; v.items[0] = &a; v.items[1] = 0; v.items[2] = &b; v.items[3] = &c;
    RemoveQuestCard(v, &b);
    if (v.n != 3 || v.items[0] != &a || v.items[1] != 0 || v.items[2] != &c) { std::printf("0077d400_TEST FAIL\n"); return 1; }
    RemoveQuestCard(v, &c);
    if (v.n != 3) { std::printf("0077d400_TEST FAIL\n"); return 1; }
    RemoveQuestCard(v, &a);
    if (v.n != 2 || v.items[0] != 0) { std::printf("0077d400_TEST FAIL\n"); return 1; }
    std::printf("0077d400_TEST PASS\n");
    return 0;
}
