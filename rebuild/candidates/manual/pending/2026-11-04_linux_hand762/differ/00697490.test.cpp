// Behaviour test for CGameCameraManager::IsPosCameraInsideThing (0x00697490, parked DIFFER): a model of the row -- the camera is inside the capsule from the thing up to 1.2x the eye height with 1.8x the collision radius.
#include <stdio.h>
#include <math.h>
static bool Inside(float tz, float eye, float radius, float px, float py, float pz)
{
    float top = tz + (eye - tz) * 1.2f, r = radius * 1.8f;
    float cz = pz < tz ? tz : (pz > top ? top : pz);
    return px * px + py * py + (pz - cz) * (pz - cz) < r * r;
}
int main() { bool ok = Inside(0, 2, 1, 0.5f, 0, 1) && !Inside(0, 2, 1, 3, 0, 1) && Inside(0, 2, 1, 0, 0, 2.3f) && !Inside(0, 2, 1, 0, 0, 5);
  printf(ok ? "00697490_TEST PASS\n" : "00697490_TEST FAIL\n");
  return ok ? 0 : 1;
}
