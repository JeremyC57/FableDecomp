// Behaviour test for NPlayerGui::CDrawFishingGame::Draw (0x00649E8B, parked DIFFER): a model of the row -- draws only while the main player hero is alive and fishing is active.
#include <stdio.h>
static int Draw(bool heroAlive, bool active, int& drawn) { if (heroAlive && active) ++drawn; return drawn; }
int main() { int d = 0; Draw(true, true, d); Draw(false, true, d); Draw(true, false, d);
  bool ok = d == 1;
  printf(ok ? "00649e8b_TEST PASS\n" : "00649e8b_TEST FAIL\n");
  return ok ? 0 : 1;
}
