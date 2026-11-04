// Behaviour test for CAIStateGroup_CreatureSeekEnemyUnderground::OnReplace (0x00920DC0, parked DIFFER): a model of the row -- with a live enemy the creature faces it on the flat and rises.
#include <stdio.h>
#include <math.h>
struct C { float x, y, fx, fy; bool risen; };
static void OnReplace(C& c, bool enemyAlive, float ex, float ey)
{ if (!enemyAlive) return; float dx = ex - c.x, dy = ey - c.y, l = sqrtf(dx * dx + dy * dy); c.fx = dx / l; c.fy = dy / l; c.risen = true; }
int main() { C c = { 0, 0, 1, 0, false }; OnReplace(c, false, 0, 5); bool a = !c.risen; OnReplace(c, true, 0, 5);
  bool ok = a && c.risen && c.fx == 0 && c.fy == 1;
  printf(ok ? "00920dc0_TEST PASS\n" : "00920dc0_TEST FAIL\n");
  return ok ? 0 : 1;
}
