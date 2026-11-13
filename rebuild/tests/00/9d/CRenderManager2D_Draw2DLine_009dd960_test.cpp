// Behaviour test for 0x009DD960 CRenderManager2D::Draw2DLine(): the row's body copied over test doubles (virtual-to-actual
// screen conversions, the screen RHW float, the queued-line sink); checks both vertices (converted X/Y, zero Z/U/V/specular,
// the RHW, the packed ARGB colour) and the blend mode chosen from the flags and the colour's alpha.
#include <cstdio>
namespace line_test {
struct C2DVector { float X, Y; };
struct CRGBColour {
    unsigned char B, G, R, A;
    unsigned char GetR() const { return R; } unsigned char GetG() const { return G; } unsigned char GetB() const { return B; } unsigned char GetA() const { return A; }
};
struct CTVertexRHWColSpecTex1 { float X, Y, Z, RHW; unsigned long DiffuseColour, SpecularColour; float U1, V1; };
static float GFVXToAX(float x) { return x * 2.0f; }
static float GFVYToAY(float y) { return y * 3.0f; }
static float g_ScreenRHW = 0.5f;
static CTVertexRHWColSpecTex1 g_a, g_b; static long g_mode = -1, g_tex = -1;
struct CRenderManager2D {
    void Add2DLine(const CTVertexRHWColSpecTex1& a, const CTVertexRHWColSpecTex1& b, long mode, long tex) { g_a = a; g_b = b; g_mode = mode; g_tex = tex; }
    bool Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags);
};
bool CRenderManager2D::Draw2DLine(const C2DVector& start, const C2DVector& end, const CRGBColour& colour, unsigned long flags)
{
    unsigned long argb = ((unsigned long)colour.GetA() << 24) | ((unsigned long)colour.GetR() << 16) | ((unsigned long)colour.GetG() << 8) | colour.GetB();
    long mode = 1;
    CTVertexRHWColSpecTex1 v0;
    CTVertexRHWColSpecTex1 v1;
    v0.X = GFVXToAX(start.X);
    v0.Y = GFVYToAY(start.Y);
    v1.X = GFVXToAX(end.X);
    v1.Y = GFVYToAY(end.Y);
    v0.DiffuseColour = argb;
    v1.DiffuseColour = argb;
    v0.SpecularColour = 0;
    v1.SpecularColour = 0;
    v0.Z = 0.0f;
    v1.Z = 0.0f;
    v0.RHW = g_ScreenRHW;
    v1.RHW = g_ScreenRHW;
    v1.U1 = 0.0f;
    v0.U1 = 0.0f;
    v1.V1 = 0.0f;
    v0.V1 = 0.0f;
    if (flags & 1) {
        if (colour.A != 0xff)
            mode = 3;
    } else
        mode = (flags & 2) ? 2 : 1;
    Add2DLine(v0, v1, mode, 0);
    return true;
}
static bool Vert(const CTVertexRHWColSpecTex1& v, float x, float y, unsigned long c)
{
    return v.X == x && v.Y == y && v.Z == 0.0f && v.RHW == 0.5f && v.DiffuseColour == c && v.SpecularColour == 0 && v.U1 == 0.0f && v.V1 == 0.0f;
}
} // namespace line_test
using namespace line_test;
int main()
{
    bool ok = true;
    CRenderManager2D r;
    C2DVector s = { 1.0f, 2.0f }, e = { 4.0f, 5.0f };
    CRGBColour opaque = { 0x10, 0x20, 0x30, 0xff }, faded = { 0x01, 0x02, 0x03, 0x80 };
    ok = ok && r.Draw2DLine(s, e, opaque, 0) && g_mode == 1 && g_tex == 0;
    ok = ok && Vert(g_a, 2.0f, 6.0f, 0xff302010UL) && Vert(g_b, 8.0f, 15.0f, 0xff302010UL);
    ok = ok && r.Draw2DLine(s, e, opaque, 2) && g_mode == 2;
    ok = ok && r.Draw2DLine(s, e, opaque, 1) && g_mode == 1;
    ok = ok && r.Draw2DLine(s, e, faded, 1) && g_mode == 3 && g_a.DiffuseColour == 0x80030201UL;
    ok = ok && r.Draw2DLine(s, e, faded, 3) && g_mode == 3;
    ok = ok && r.Draw2DLine(s, e, faded, 2) && g_mode == 2;
    std::printf(ok ? "009dd960_TEST PASS\n" : "009dd960_TEST FAIL\n");
    return ok ? 0 : 1;
}