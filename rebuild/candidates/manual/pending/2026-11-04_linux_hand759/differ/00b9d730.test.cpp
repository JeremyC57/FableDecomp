// Behaviour test for CEnginePrimitiveManagerStaticMeshesBase::IsInsideFustrum (0x00B9D730, parked DIFFER): a model of the row -- the transformed sphere must overlap the depth range and every side plane.
#include <stdio.h>
struct Pl { float a, b, c, d; };
static bool Inside(float cx, float cy, float cz, float r, float scale, float nearZ, float farZ, const Pl* p)
{ float rr = r * scale; if (cz + rr < nearZ || cz - rr > farZ) return false;
  for (int i = 0; i < 4; ++i) if (p[i].a * cx + p[i].b * cy + p[i].c * cz + p[i].d < -rr) return false; return true; }
int main() { Pl p[4] = { { 1, 0, 0, 10 }, { -1, 0, 0, 10 }, { 0, 1, 0, 10 }, { 0, -1, 0, 10 } };
  bool ok = Inside(0, 0, 5, 1, 1, 1, 100, p) && !Inside(0, 0, 200, 1, 1, 1, 100, p) && !Inside(20, 0, 5, 1, 1, 1, 100, p) && Inside(11, 0, 5, 1, 2, 1, 100, p);
  printf(ok ? "00b9d730_TEST PASS\n" : "00b9d730_TEST FAIL\n");
  return ok ? 0 : 1;
}
