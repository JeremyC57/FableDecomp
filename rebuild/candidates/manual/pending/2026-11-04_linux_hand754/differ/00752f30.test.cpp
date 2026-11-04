// Behaviour test for CTCLook::UpdateHeadCreatureAnim (0x00752F30): a model of the row.
#include <stdio.h>

struct Anim { bool present; float ratio, previous; };
struct Look { float angles[4]; int entryH, entryV; };
static void Update(Look& l, int type, Anim& anim, bool canFind, float frame)
{
    int entry = type == 11 ? l.entryH : type == 12 ? l.entryV : -2;
    if (entry == -2 || entry == -1) return;
    if (!anim.present && canFind) { anim.present = true; anim.ratio = 0.5f; }
    if (anim.present) { float prev = anim.ratio; anim.ratio = frame; anim.previous = prev; }
    else { for (int i = 0; i < 4; ++i) l.angles[i] = 0; }
}
static int run()
{
    Look l = { { 1, 1, 1, 1 }, 3, -1 }; Anim a = { false, 0, 0 };
    Update(l, 11, a, true, 0.75f); int ok = a.present && a.ratio == 0.75f && a.previous == 0.5f;
    Anim b = { false, 0, 0 }; Update(l, 12, b, true, 0.75f); ok = ok && !b.present;
    Update(l, 11, b, false, 0.3f); ok = ok && l.angles[0] == 0 && !b.present;
    return ok;
}
int main()
{
    int ok = run();
    printf(ok ? "00752f30_TEST PASS\n" : "00752f30_TEST FAIL\n");
    return ok ? 0 : 1;
}
