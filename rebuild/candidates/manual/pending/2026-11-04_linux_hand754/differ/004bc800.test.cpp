// Behaviour test for NGraphicAppearance::CTCGraphicAppearance::GetPositionMatrix (0x004BC800): a model of the row.
#include <stdio.h>

struct M { float pos, orient, scale; };
static M GetPositionMatrix(float renderPos, float orient, float scale, float renderSizeX)
{
    M m; m.pos = renderPos; m.orient = orient; m.scale = scale * renderSizeX * 0.01f; return m;
}
static int run()
{
    M m = GetPositionMatrix(3.0f, 0.5f, 2.0f, 50.0f);
    return m.pos == 3.0f && m.orient == 0.5f && m.scale > 0.999f && m.scale < 1.001f;
}
int main()
{
    int ok = run();
    printf(ok ? "004bc800_TEST PASS\n" : "004bc800_TEST FAIL\n");
    return ok ? 0 : 1;
}
