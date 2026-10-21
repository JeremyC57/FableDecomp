// behaviour test for 0x0048FAA0
#include <stdio.h>
class C2DBoxF { public: float X0, Y0, X1, Y1; };
class CBoxStyle { public: int id; };
static long g_layer, g_flags; static const CBoxStyle* g_style; static const C2DBoxF* g_box;
void __fastcall DrawBox(long layer, const CBoxStyle& style, const C2DBoxF& box, long flags)
{ g_layer = layer; g_style = &style; g_box = &box; g_flags = flags; }
class CSubtitleRenderer {
public:
    void DrawBox(long layer, const CBoxStyle& style, const C2DBoxF& box);
};
void CSubtitleRenderer::DrawBox(long layer, const CBoxStyle& style, const C2DBoxF& box)
{
    ::DrawBox(layer, style, box, 0x47);
}
int main()
{
    CSubtitleRenderer r; CBoxStyle s; C2DBoxF b; int ok = 1;
    r.DrawBox(3, s, b);
    ok &= g_layer == 3 && g_style == &s && g_box == &b && g_flags == 0x47;
    printf(ok ? "0048faa0_TEST PASS\n" : "0048faa0_TEST FAIL\n");
    return ok ? 0 : 1;
}