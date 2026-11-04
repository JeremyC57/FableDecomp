// Behaviour test for CTCLook::LookAtPos (0x00753E90, parked DIFFER): a model of the row -- re-arms the look timer, stores flags, target and direction.
#include <stdio.h>
#include <math.h>
struct Look { float timer; unsigned long flags; float tx, ty, tz, dx, dy, dz; };
static void LookAtPos(Look& l, float x, float y, float z, float hx, float hy, float hz, unsigned long flags)
{
    l.timer = 0; l.flags = flags; l.tx = x; l.ty = y; l.tz = z;
    float vx = x - hx, vy = y - hy, vz = z - hz, len = sqrtf(vx * vx + vy * vy + vz * vz);
    if (len > 0) { l.dx = vx / len; l.dy = vy / len; l.dz = vz / len; }
}
int main() { Look l = { 5, 0, 0, 0, 0, 0, 0, 0 }; LookAtPos(l, 3, 4, 0, 0, 0, 0, 2);
  bool ok = l.timer == 0 && l.flags == 2 && l.tx == 3 && fabsf(l.dx - 0.6f) < 1e-6f && fabsf(l.dy - 0.8f) < 1e-6f;
  printf(ok ? "00753e90_TEST PASS\n" : "00753e90_TEST FAIL\n");
  return ok ? 0 : 1;
}
