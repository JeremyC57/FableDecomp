// Behaviour test for NCameraModes::CRotateAround::Update (0x0087BD70, parked DIFFER): a model of the row -- Theta turns by speed x delta (inverted on request) and the camera sits on the circle around the focal point at the clamped height.
#include <stdio.h>
#include <math.h>
struct Cam { float theta, x, y, z; };
static void Update(Cam& c, float speed, float delta, bool invert, float r, float fz, float off, float lo, float hi)
{ c.theta += (invert ? -speed : speed) * delta; c.x = r * cosf(c.theta); c.y = r * sinf(c.theta);
  float z = fz + off; c.z = z < lo ? lo : (z > hi ? hi : z); }
int main() { Cam c = { 0, 0, 0, 0 }; Update(c, 1.0f, 0.5f, false, 2, 0, 5, 0, 3);
  bool ok = fabsf(c.theta - 0.5f) < 1e-6f && fabsf(c.x * c.x + c.y * c.y - 4) < 1e-4f && c.z == 3;
  printf(ok ? "0087bd70_TEST PASS\n" : "0087bd70_TEST FAIL\n");
  return ok ? 0 : 1;
}
