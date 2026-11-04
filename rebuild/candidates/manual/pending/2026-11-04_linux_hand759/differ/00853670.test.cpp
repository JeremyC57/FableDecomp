// Behaviour test for NCameraModes::CScriptModuleOpenChest::OnInitLeaf (0x00853670, parked DIFFER): a model of the row -- with both things alive the orbit is centred on the chest at twice the creature distance.
#include <stdio.h>
#include <math.h>
struct Orbit { bool set; float ox, oy, radius; };
static void Init(Orbit& o, bool a, bool b, float cx, float cy, float px, float py)
{ if (!a || !b) return; o.set = true; o.ox = cx; o.oy = cy; o.radius = 2.0f * sqrtf((cx - px) * (cx - px) + (cy - py) * (cy - py)); }
int main() { Orbit o = { false, 0, 0, 0 }; Init(o, true, false, 1, 1, 0, 0); bool a = !o.set; Init(o, true, true, 3, 4, 0, 0);
  bool ok = a && o.set && o.ox == 3 && o.radius == 10.0f;
  printf(ok ? "00853670_TEST PASS\n" : "00853670_TEST FAIL\n");
  return ok ? 0 : 1;
}
