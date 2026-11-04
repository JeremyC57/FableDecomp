// Behaviour test for NCameraModes::CStringAndCageBase::UpdateXYVars (0x00871FD0, parked DIFFER row): a model of the row -- the string length eases a quarter toward its target; the cage eases toward the string length while expanding.
#include <stdio.h>
struct C { float Len, Target, Cage, CageTarget; bool Expanding;
    void Update() { Len += (Target - Len) * 0.25f; float goal = Expanding ? Len : CageTarget; Cage += (goal - Cage) * 0.25f; } };
static int Check() { C c = { 0, 8, 0, 4, false }; c.Update(); int ok = c.Len == 2 && c.Cage == 1; c.Expanding = true; c.Update(); return ok && c.Len == 3.5f && c.Cage == 1.625f; }
int main()
{
    int ok = Check();
    printf(ok ? "00871fd0_TEST PASS\n" : "00871fd0_TEST FAIL\n");
    return ok ? 0 : 1;
}
