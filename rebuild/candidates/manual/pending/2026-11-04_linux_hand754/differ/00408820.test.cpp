// Behaviour test for CUserProfileManager::ResetAssignedInputsWASD (0x00408820): a model of the row.
#include <stdio.h>

struct Def { int controls; int refs; bool deleted; };
static int g_applied, g_resetDefault;
struct Manager { int passive, aggressive; };
static void Reset(Manager& m, bool pad, bool noCaps, bool haveManager, Def* wasd, Def* wasdNoCaps)
{
    if (pad) { g_resetDefault++; return; }
    Def* scheme = 0;
    if (haveManager) {
        scheme = noCaps ? wasdNoCaps : wasd; scheme->refs++;
        m.passive = scheme->controls; m.aggressive = scheme->controls;
    }
    if (scheme && --scheme->refs == 0) scheme->deleted = true;
    g_applied++;
}
static int run()
{
    Def a = { 1, 1, false }, b = { 2, 1, false };
    Manager m = { 0, 0 };
    Reset(m, false, false, true, &a, &b); int ok = m.passive == 1 && m.aggressive == 1 && a.refs == 1 && g_applied == 1;
    Reset(m, false, true, true, &a, &b); ok = ok && m.passive == 2 && g_applied == 2;
    Reset(m, true, false, true, &a, &b); ok = ok && g_resetDefault == 1 && g_applied == 2;
    Reset(m, false, false, false, &a, &b); ok = ok && g_applied == 3 && m.passive == 2;
    return ok;
}
int main()
{
    int ok = run();
    printf(ok ? "00408820_TEST PASS\n" : "00408820_TEST FAIL\n");
    return ok ? 0 : 1;
}
