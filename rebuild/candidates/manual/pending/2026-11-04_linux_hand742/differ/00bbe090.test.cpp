// Behaviour test for CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderSingleSpriteTrail (0x00BBE090): a model of the pass
// choice (6 with displacement on, 7: displacement shaders, ADD + SRC_ALPHA/INV_SRC_ALPHA for 6, no premultiply; otherwise the
// trail's blend op (1 SUBTRACT, 2 REVSUBTRACT, else ADD) and blend mode (3 ONE/ONE, 4 ONE/INV_SRC_COLOR, both premultiplied;
// else SRC_ALPHA/INV_SRC_ALPHA), point lights when flag 0x80 else 100% self illumination, the point-light shader index clamped
// to the shader count - 1) and the frame UV constant (TL * 1/32767, frame size / graphic size).
#include <cstdio>
#include <cmath>
struct Result { int op, src, dst; bool premultiply, displacementShaders; unsigned long shader; long selfIllum; float c[4]; };
enum { ONE = 2, INV_SRC_COLOR = 4, SRC_ALPHA = 5, INV_SRC_ALPHA = 6, ADD = 1, SUB = 2, REVSUB = 3 };
static bool Render(bool frameOk, int pass, bool dispOn, int blendOp, int blendMode, unsigned flags, unsigned long lights,
                   unsigned long shaderCount, unsigned tlx, unsigned tly, unsigned fw, unsigned fh, unsigned w, unsigned h, Result& r)
{
    if (!frameOk) return false;
    r.selfIllum = -1;
    if ((pass == 6 && dispOn) || pass == 7) {
        r.premultiply = false; r.displacementShaders = true;
        if (pass == 6) { r.op = ADD; r.src = SRC_ALPHA; r.dst = INV_SRC_ALPHA; }
    } else {
        r.displacementShaders = false;
        r.op = blendOp == 1 ? SUB : blendOp == 2 ? REVSUB : ADD;
        if (blendMode == 3) { r.src = ONE; r.dst = ONE; r.premultiply = true; }
        else if (blendMode == 4) { r.src = ONE; r.dst = INV_SRC_COLOR; r.premultiply = true; }
        else { r.src = SRC_ALPHA; r.dst = INV_SRC_ALPHA; r.premultiply = false; }
        unsigned long l = 0;
        if (flags & 0x80) l = lights; else r.selfIllum = 100;
        r.shader = l > shaderCount - 1 ? shaderCount - 1 : l;
    }
    r.c[0] = tlx * (1.0f / 32767.0f); r.c[1] = tly * (1.0f / 32767.0f); r.c[2] = (float)fw / w; r.c[3] = (float)fh / h;
    return true;
}
int main()
{
    Result r; bool ok = !Render(false, 0, false, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, r);
    ok = ok && Render(true, 6, true, 2, 3, 0x80, 9, 4, 32767, 0, 16, 8, 64, 32, r) && !r.premultiply && r.displacementShaders
        && r.op == ADD && r.src == SRC_ALPHA && std::fabs(r.c[0] - 1.0f) < 1e-6f && std::fabs(r.c[2] - 0.25f) < 1e-6f;
    ok = ok && Render(true, 6, false, 2, 3, 0x80, 9, 4, 0, 0, 1, 1, 1, 1, r) && r.premultiply && r.op == REVSUB && r.dst == ONE
        && r.shader == 3 && r.selfIllum == -1;
    ok = ok && Render(true, 0, false, 1, 4, 0, 9, 4, 0, 0, 1, 1, 1, 1, r) && r.premultiply && r.op == SUB && r.dst == INV_SRC_COLOR
        && r.shader == 0 && r.selfIllum == 100;
    ok = ok && Render(true, 0, false, 0, 0, 0x80, 2, 4, 0, 0, 1, 1, 1, 1, r) && !r.premultiply && r.op == ADD && r.shader == 2;
    printf(ok ? "00bbe090_TEST PASS\n" : "00bbe090_TEST FAIL\n");
    return ok ? 0 : 1;
}
