#include <cstdio>
struct Thing { Thing* next; bool dead; bool renderUpdated; int calls; };
struct LL { Thing* head; Thing* tail; };
struct Rec { bool render; };
static void Render(Rec* recs, LL* lists)
{
    for (long i = 1; i < 16; ++i) {
        if (recs[i].render) {
            Thing* next = lists[i].head;
            while (next) { Thing* t = next; next = t->next; if (!t->dead && t->renderUpdated) t->calls += 2; }
        }
    }
}
int main()
{
    Thing a = { 0, false, true, 0 }, b = { &a, true, true, 0 }, c = { 0, false, true, 0 }, z = { 0, false, true, 0 };
    Rec recs[16] = { { true } }; recs[3].render = true;
    LL lists[16] = { { &z, &z } }; lists[3].head = &b; lists[4].head = &c;
    Render(recs, lists);
    bool ok = a.calls == 2 && b.calls == 0 && c.calls == 0 && z.calls == 0;
    if (!ok) { std::printf("0051ebd0_TEST FAIL\n"); return 1; }
    std::printf("0051ebd0_TEST PASS\n");
    return 0;
}