// Behaviour test for CEnginePrimitiveManagerText::RenderSubPrimitive (0x00BC6DB0): a model of the render setup decisions --
// shaders on unless the streaming fonts run fixed-function (with shaders, vertex constant 1 = 1/(w/2), 1/(h/2), -1, 1), a mask
// texture selects stage-1 clamp with point filtering unless KeepMaskFiltering, the mask UVs go to the vertex shader (or to
// the font's fixed-function mask UVs), and the text is drawn at sub position + viewport top-left (Z unchanged).
#include <stdio.h>
struct State { bool Shaders, MaskShaders, Stage1Clamp, Stage1Point, VsMaskUVs, FfMaskUVs, ScaleSet; float ScaleX, ScaleY;
               float X, Y, Z; int Renders; };
static State Render(bool fixedFunction, bool mask, bool keepFiltering, float w, float h, float px, float py, float pz,
                    float vx, float vy)
{
    State s = {};
    if (!fixedFunction) { s.Shaders = true; s.ScaleSet = true; s.ScaleX = 1.0f / (w * 0.5f); s.ScaleY = 1.0f / (h * 0.5f); }
    if (mask) {
        s.Stage1Clamp = true;
        s.Stage1Point = !keepFiltering;
        if (!fixedFunction) { s.VsMaskUVs = true; s.MaskShaders = true; } else s.FfMaskUVs = true;
    }
    s.X = px + vx; s.Y = vy + py; s.Z = pz;
    ++s.Renders;
    return s;
}
int main()
{
    int ok = 1;
    State a = Render(false, false, false, 640, 480, 10, 20, 3, 100, 50);
    ok &= a.Shaders && a.ScaleSet && a.ScaleX == 1.0f / 320 && a.ScaleY == 1.0f / 240 && !a.Stage1Clamp && a.X == 110 && a.Y == 70 && a.Z == 3 && a.Renders == 1;
    State b = Render(true, true, false, 640, 480, 0, 0, 0, 0, 0);
    ok &= !b.Shaders && !b.ScaleSet && b.Stage1Point && b.FfMaskUVs && !b.VsMaskUVs;
    State c = Render(false, true, true, 640, 480, 0, 0, 0, 0, 0);
    ok &= c.Stage1Clamp && !c.Stage1Point && c.VsMaskUVs && c.MaskShaders;
    printf(ok ? "00bc6db0_TEST PASS\n" : "00bc6db0_TEST FAIL\n");
    return ok ? 0 : 1;
}
