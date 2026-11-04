// Behaviour test for CTCFlourishTarget::ProcessActionMovementDelta (0x007E8A40, parked DIFFER): a model of the row -- the delta is re-expressed in the target-relative frame (forward / side components).
#include <stdio.h>
static void Process(float& dx, float& dy, float fx, float fy)
{ float f = dx * fx + dy * fy, s = dx * -fy + dy * fx; dx = f; dy = s; }
int main() { float x = 1, y = 0; Process(x, y, 0, 1); bool a = x == 0 && y == -1; x = 2; y = 0; Process(x, y, 1, 0);
  bool ok = a && x == 2 && y == 0;
  printf(ok ? "007e8a40_TEST PASS\n" : "007e8a40_TEST FAIL\n");
  return ok ? 0 : 1;
}
