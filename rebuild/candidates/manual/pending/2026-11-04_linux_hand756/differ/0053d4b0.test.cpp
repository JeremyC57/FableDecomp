// Behaviour test for NUISystem::CMenuEntry::Update (0x0053D4B0): a model of the row -- full clickable update while the menu accepts
// input, else only the state animation; the on-visible actions fire once when the entry has just become visible.
#include <stdio.h>
struct Entry { bool Visible, WasVisible; int Full, Anim, Fired; };
static void Update(Entry& e, bool acceptsInput)
{
    bool was = e.WasVisible;
    if (acceptsInput) ++e.Full; else ++e.Anim;
    if (e.Visible && !was) ++e.Fired;
    e.WasVisible = e.Visible;
}
int main()
{
    Entry e = { false, false, 0, 0, 0 };
    Update(e, true); e.Visible = true; Update(e, false); Update(e, true);
    int ok = e.Full == 2 && e.Anim == 1 && e.Fired == 1;
    printf(ok ? "0053d4b0_TEST PASS\n" : "0053d4b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
