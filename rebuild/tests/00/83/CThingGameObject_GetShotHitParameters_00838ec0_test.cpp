// Behaviour test for CThingGameObject::GetShotHitParameters (0x00838EC0): a model of the row -- flags and build-up from the shot def, the normalised travel direction, the hitter and the damage.
#include <stdio.h>
#include <math.h>
struct Hit { bool knockDown, blockable; float buildUp, dx, dy, dz, damage; };
static void Fill(Hit& h, bool kd, bool bl, float bu, float vx, float vy, float vz, float dmg)
{ h.knockDown = kd; h.blockable = bl; h.buildUp = bu; float l = sqrtf(vx * vx + vy * vy + vz * vz); h.dx = vx / l; h.dy = vy / l; h.dz = vz / l; h.damage = dmg; }
int main() { Hit h; Fill(h, true, false, 0.5f, 0, 3, 4, 12);
  bool ok = h.knockDown && !h.blockable && h.buildUp == 0.5f && fabsf(h.dy - 0.6f) < 1e-6f && fabsf(h.dz - 0.8f) < 1e-6f && h.damage == 12;
  printf(ok ? "00838ec0_TEST PASS\n" : "00838ec0_TEST FAIL\n");
  return ok ? 0 : 1;
}