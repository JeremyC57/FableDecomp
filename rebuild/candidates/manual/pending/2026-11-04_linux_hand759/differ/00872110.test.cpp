// Behaviour test for NCameraModes::CStringAndCageBase::UpdateAutoGoBehind (0x00872110, parked DIFFER): a model of the row -- the go-to point swings an eleventh of the way towards directly behind the thing, keeping its XY distance.
#include <stdio.h>
#include <math.h>
static void Update(float& gx, float& gy, float fx, float fy, float dirx, float diry)
{
    float vx = gx - fx, vy = gy - fy, d = sqrtf(vx * vx + vy * vy);
    float bx = -dirx * d, by = -diry * d;
    float nx = vx + (bx - vx) / 11.0f, ny = vy + (by - vy) / 11.0f, n = sqrtf(nx * nx + ny * ny);
    gx = fx + nx / n * d; gy = fy + ny / n * d;
}
int main() { float x = 0, y = 10; Update(x, y, 0, 0, 1, 0); float d = sqrtf(x * x + y * y);
  float x2 = -10, y2 = 0; Update(x2, y2, 0, 0, 1, 0);
  bool ok = fabsf(d - 10) < 1e-4f && x < 0 && y < 10 && fabsf(x2 + 10) < 1e-4f && fabsf(y2) < 1e-4f;
  printf(ok ? "00872110_TEST PASS\n" : "00872110_TEST FAIL\n");
  return ok ? 0 : 1;
}
