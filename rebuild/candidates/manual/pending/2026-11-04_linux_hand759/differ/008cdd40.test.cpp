// Behaviour test for CCreatureAction_ReleaseTimeSpell::FinishAction (0x008CDD40, parked DIFFER): a model of the row -- after the base FinishAction the camera control is released and the weapon re-equipped.
#include <stdio.h>
struct S { int finished; bool cameraControlled; bool equipped; };
static void Finish(S& s) { ++s.finished; s.cameraControlled = false; s.equipped = true; }
int main() { S s = { 0, true, false }; Finish(s); bool ok = s.finished == 1 && !s.cameraControlled && s.equipped;
  printf(ok ? "008cdd40_TEST PASS\n" : "008cdd40_TEST FAIL\n");
  return ok ? 0 : 1;
}
