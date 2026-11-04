// Behaviour test for CAIStateGroup_HoverHornetQueen::Spawn (0x0095B820, parked DIFFER): a model of the row -- every live picnic hornet within the box becomes a helper with the queen's persistence.
#include <stdio.h>
#include <vector>
struct H { int def; bool alive; float x, y; bool helper, persistent; };
static int Spawn(std::vector<H>& hs, int def, float qx, float qy, bool persistent)
{ int n = 0; for (size_t i = 0; i < hs.size(); ++i) { H& h = hs[i];
    if (h.def == def && h.alive && h.x > qx - 60 && h.x < qx + 60 && h.y > qy - 60 && h.y < qy + 60) { h.persistent = persistent; h.helper = true; ++n; } }
  return n; }
int main() { std::vector<H> v; H a = { 1, true, 10, 10, false, false }, b = { 1, false, 0, 0, false, false }, c = { 1, true, 100, 0, false, false };
  v.push_back(a); v.push_back(b); v.push_back(c);
  bool ok = Spawn(v, 1, 0, 0, true) == 1 && v[0].helper && v[0].persistent && !v[1].helper && !v[2].helper;
  printf(ok ? "0095b820_TEST PASS\n" : "0095b820_TEST FAIL\n");
  return ok ? 0 : 1;
}
