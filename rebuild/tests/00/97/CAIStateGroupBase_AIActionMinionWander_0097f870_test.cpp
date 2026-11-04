// Behaviour test for CAIStateGroupBase::AIAction_MinionWander (0x0097F870): a model of the row -- a wander position inside the building (ten tries) or near the creature is walked to; with none the brain yields.
#include <stdio.h>
static int Wander(bool building, int insideOnTry, bool nearOk, bool& walked)
{ walked = false;
  if (building) { for (int i = 0; i < 10; ++i) if (i == insideOnTry) { walked = true; return i; } return -1; }
  if (nearOk) { walked = true; return 0; } return -1; }
int main() { bool w; bool ok = Wander(true, 3, false, w) == 3 && w && Wander(true, 12, false, w) == -1 && !w && Wander(false, 0, true, w) == 0 && w;
  printf(ok ? "0097f870_TEST PASS\n" : "0097f870_TEST FAIL\n");
  return ok ? 0 : 1;
}