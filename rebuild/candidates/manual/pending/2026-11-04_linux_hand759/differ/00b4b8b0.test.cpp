// Behaviour test for CEngineShadowRenderer::CalcLocalWorldToBaseShadowSpace (0x00B4B8B0, parked DIFFER): a model of the row -- up / side axes orthogonal to the direction, normalised, scaled by 2/width and 2/height.
#include <stdio.h>
#include <math.h>
static void Basis(const float d[3], float w, float h, float side[3], float up[3])
{
    float a[3] = { 0, 0, 0 }; if (fabsf(d[0]) > fabsf(d[1])) a[1] = 1; else a[0] = 1;
    side[0] = a[1] * d[2] - a[2] * d[1]; side[1] = a[2] * d[0] - a[0] * d[2]; side[2] = a[0] * d[1] - a[1] * d[0];
    float l = sqrtf(side[0] * side[0] + side[1] * side[1] + side[2] * side[2]); for (int i = 0; i < 3; ++i) side[i] /= l;
    up[0] = side[1] * d[2] - side[2] * d[1]; up[1] = side[2] * d[0] - side[0] * d[2]; up[2] = side[0] * d[1] - side[1] * d[0];
    l = sqrtf(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]);
    for (int i = 0; i < 3; ++i) { up[i] = up[i] / l * (2.0f / h); side[i] *= 2.0f / w; }
}
int main() { float d[3] = { 0, 0, -1 }, s[3], u[3]; Basis(d, 4, 8, s, u);
  float dotS = s[0] * d[0] + s[1] * d[1] + s[2] * d[2], dotU = u[0] * d[0] + u[1] * d[1] + u[2] * d[2];
  bool ok = fabsf(dotS) < 1e-6f && fabsf(dotU) < 1e-6f && fabsf(sqrtf(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]) - 0.5f) < 1e-5f
      && fabsf(sqrtf(u[0] * u[0] + u[1] * u[1] + u[2] * u[2]) - 0.25f) < 1e-5f;
  printf(ok ? "00b4b8b0_TEST PASS\n" : "00b4b8b0_TEST FAIL\n");
  return ok ? 0 : 1;
}
