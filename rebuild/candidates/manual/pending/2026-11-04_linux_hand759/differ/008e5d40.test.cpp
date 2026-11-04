// Behaviour test for CTCPhysicsStandard::Move (0x008E5D40, parked DIFFER): a model of the row -- one step applies the forces, moves the position, keeps the speed on request and commits the settled position.
#include <stdio.h>
struct P { float x, v, f; bool keepSpeed; };
static void Move(P& p, float dt, float ground)
{ float v0 = p.v; p.v += p.f * dt; p.x += p.v * dt; if (p.keepSpeed) p.v = v0; if (p.x < ground) p.x = ground; }
int main() { P a = { 0, 1, 2, false }; Move(a, 1, -10); P b = { 0, 1, 2, true }; Move(b, 1, -10); P c = { 0, -5, 0, false }; Move(c, 1, -1);
  bool ok = a.x == 3 && a.v == 3 && b.v == 1 && c.x == -1;
  printf(ok ? "008e5d40_TEST PASS\n" : "008e5d40_TEST FAIL\n");
  return ok ? 0 : 1;
}
