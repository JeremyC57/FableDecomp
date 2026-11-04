// Behaviour test for NUISystem::CList::CanMove (0x00537776, parked DIFFER): a model of the row -- the position is unchanged unless the target leaves the visible band, then y is pulled back by 0x40.
#include <stdio.h>
static void CanMove(float x, float y, float top, float bottom, float& ox, float& oy)
{
    ox = x; oy = y;
    if (y < top || y > bottom) oy = y + (y < top ? 64.0f : -64.0f);
}
int main() { float x, y; CanMove(5, 50, 0, 100, x, y); bool a = x == 5 && y == 50; CanMove(5, -10, 0, 100, x, y);
  bool b = y == 54.0f; CanMove(5, 130, 0, 100, x, y); bool ok = a && b && y == 66.0f;
  printf(ok ? "00537776_TEST PASS\n" : "00537776_TEST FAIL\n");
  return ok ? 0 : 1;
}
