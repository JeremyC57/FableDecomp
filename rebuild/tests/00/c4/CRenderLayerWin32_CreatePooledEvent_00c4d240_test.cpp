// Behaviour test for CRenderLayerWin32::CreatePooledEvent (0x00C4D240): a pooled event while the budget lasts (budget and
// total drop), else a factory event by type; a failed initialise yields 0 and its code; no event -> -5.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Ev { long init; bool active; };
static long Create(Ev* pooled, long* budget, long* total, int type, Ev* made, Ev** out) {
    Ev* e = 0; long r;
    if (*budget > 0) { e = pooled; r = e->init; if (r != 0) e = 0; else { e->active = true; --*budget; if (*total > 0) --*total; } }
    else { if (type >= 0 && type <= 3) e = made; if (e == 0) r = -5; else { r = e->init; if (r == 0) e->active = true; else e = 0; } }
    *out = e; return r; }
int main()
{
    Ev p = { 0, false }, m = { 0, false }, bad = { 7, false };
    long budget = 1, total = 0; Ev* out;
    long r1 = Create(&p, &budget, &total, 0, &m, &out); bool a = r1 == 0 && out == &p && budget == 0 && total == 0 && p.active;
    long r2 = Create(&p, &budget, &total, 2, &m, &out); bool b = r2 == 0 && out == &m && m.active;
    long r3 = Create(&p, &budget, &total, 9, &m, &out); bool c = r3 == -5 && out == 0;
    long r4 = Create(&p, &budget, &total, 1, &bad, &out); bool d = r4 == 7 && out == 0;
    bool ok = a && b && c && d;
    printf(ok ? "00c4d240_TEST PASS\n" : "00c4d240_TEST FAIL\n");
    return ok ? 0 : 1;
}