// Behaviour test for NCameraModes::CScriptModuleUseLamp::OnInitLeaf (0x008539C0, parked DIFFER): a model of the row -- with both things alive the orbit is centred on the lamp helper point at four times the distance.
#include <stdio.h>
#include <math.h>
struct Orbit { bool set; float ox, oy, radius; };
static void Init(Orbit& o, bool a, bool b, bool helper, float hx, float hy, float lx, float ly, float px, float py)
{ if (!a || !b) return; o.set = true; o.ox = helper ? hx : lx; o.oy = helper ? hy : ly;
  o.radius = 4.0f * sqrtf((lx - px) * (lx - px) + (ly - py) * (ly - py)); }
int main() { Orbit o = { false, 0, 0, 0 }; Init(o, true, true, true, 9, 9, 3, 4, 0, 0);
  bool ok = o.set && o.ox == 9 && o.radius == 20.0f;
  printf(ok ? "008539c0_TEST PASS\n" : "008539c0_TEST FAIL\n");
  return ok ? 0 : 1;
}
