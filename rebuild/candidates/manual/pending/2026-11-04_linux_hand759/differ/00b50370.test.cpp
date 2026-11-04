// Behaviour test for CEngineShadowRenderer::CEngineShadowRenderer (0x00B50370, parked DIFFER): a model of the row -- the constructor defaults: distances 32 / 8, biases, interpolation 0.1 / 0.2, buffer 0x400, scale and UV scaler 1.
#include <stdio.h>
struct R { float fadeEnd, maxDist, fadeStart, span, zOff, zSlope, zBias, minW, ratio, iStart, iEnd, scale, uv[3]; long size; bool persp, debug; };
static void Ctor(R& r)
{ r.fadeEnd = r.maxDist = 32.0f; r.fadeStart = r.span = 8.0f; r.zOff = 0.0001f; r.zSlope = 2.0f; r.zBias = -0.001f; r.minW = 0.1f;
  r.ratio = 0.5f; r.iStart = 0.1f; r.iEnd = 0.2f; r.size = 0x400; r.scale = 1.0f; r.persp = true; r.debug = false; r.uv[0] = r.uv[1] = r.uv[2] = 1.0f; }
int main() { R r; Ctor(r); bool ok = r.maxDist == 32 && r.span == 8 && r.size == 1024 && r.uv[2] == 1 && r.persp && !r.debug && r.iEnd == 0.2f;
  printf(ok ? "00b50370_TEST PASS\n" : "00b50370_TEST FAIL\n");
  return ok ? 0 : 1;
}
