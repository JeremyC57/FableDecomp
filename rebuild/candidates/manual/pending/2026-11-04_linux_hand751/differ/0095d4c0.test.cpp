// Behaviour test for CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList (0x0095D4C0, parked DIFFER row): a model of the row -- collect the preferred fire points that are not occupied.
#include <stdio.h>
#include <vector>
struct Thing { bool Preferred, Occupied; };
static void Get(Thing* all, int n, std::vector<Thing*>& out) { for (int i = 0; i < n; ++i) if (all[i].Preferred && !all[i].Occupied) out.push_back(&all[i]); }
static int Check() { Thing t[3] = { { true, false }, { true, true }, { false, false } }; std::vector<Thing*> out; Get(t, 3, out); return out.size() == 1 && out[0] == &t[0]; }
int main()
{
    int ok = Check();
    printf(ok ? "0095d4c0_TEST PASS\n" : "0095d4c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
