#include <cstdio>

// Model of 0x00404E10 NUISystem::CIME::Callback_DrawFans: convert the fan centre once, then for each rim vertex i
// (1..count-1) draw the triangle (centre, v[i], v[i+1], wrapping the last back to v[1]) with v[i]'s RGB and the
// composition alpha, all in converted coordinates. Coordinate conversion and the 2D render manager are stubs.
struct C2DVector {
    float X, Y;
    C2DVector() { }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct STriangle {
    C2DVector P[3];
    unsigned char R, G, B, A;
    unsigned long Flags;
};
struct CRenderManager2D {
    STriangle Drawn[8];
    int Count;
    bool Draw2DTriangle(const C2DVector& p0, const C2DVector& p1, const C2DVector& p2, const CRGBColour& colour, unsigned long flags)
    {
        STriangle& t = Drawn[Count++];
        t.P[0] = p0; t.P[1] = p1; t.P[2] = p2;
        t.R = colour.R; t.G = colour.G; t.B = colour.B; t.A = colour.A;
        t.Flags = flags;
        return true;
    }
};
struct CDisplayManager {
    CRenderManager2D* RenderManager2D;
};
struct CSystemManager {
    CDisplayManager* DisplayManager;
};
static CRenderManager2D g_render;
static CDisplayManager g_displayManager = { &g_render };
static CSystemManager g_systemManager = { &g_displayManager };
static CSystemManager* GFGetSystemManager() { return &g_systemManager; }
static void GFACoordToVCoord(const C2DVector* in, C2DVector* out)   // stand-in: scale by 2
{
    float x = in->X * 2.0f, y = in->Y * 2.0f;
    out->X = x;
    out->Y = y;
}
static unsigned char g_imeCompositionAlpha_013750b0 = 0x80;
struct IMEUI_VERTEX {
    float sx, sy, sz, rhw;
    unsigned int color;
    unsigned int specular;
    float tu, tv;
};

static void Callback_DrawFans(const IMEUI_VERTEX* vertices, unsigned int count)
{
    C2DVector points[3];
    points[0].X = vertices[0].sx;
    points[0].Y = vertices[0].sy;
    GFACoordToVCoord(&points[0], &points[0]);
    for (unsigned int i = 1; i < count; ++i) {
        unsigned int next = (i == count - 1) ? 1 : i + 1;
        points[1].X = vertices[i].sx;
        points[1].Y = vertices[i].sy;
        points[2].X = vertices[next].sx;
        points[2].Y = vertices[next].sy;
        GFACoordToVCoord(&points[1], &points[1]);
        GFACoordToVCoord(&points[2], &points[2]);
        unsigned int color = vertices[i].color;
        unsigned char b = (unsigned char)color;
        color >>= 8;
        unsigned char g = (unsigned char)color;
        color >>= 8;
        unsigned char r = (unsigned char)color;
        CRGBColour colour(r, g, b, g_imeCompositionAlpha_013750b0);
        CDisplayManager* displayManager = GFGetSystemManager()->DisplayManager;
        displayManager->RenderManager2D->Draw2DTriangle(points[0], points[1], points[2], colour, 1);
    }
}

static int Fail()
{
    std::printf("00404e10_TEST FAIL\n");
    return 1;
}

int main()
{
    IMEUI_VERTEX v[4];
    const float xs[4] = { 10.0f, 20.0f, 30.0f, 40.0f };
    const float ys[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
    const unsigned int colors[4] = { 0xFF000000u, 0xFF112233u, 0x00445566u, 0x7F778899u };
    for (int i = 0; i < 4; ++i) {
        v[i].sx = xs[i]; v[i].sy = ys[i]; v[i].sz = 0.0f; v[i].rhw = 1.0f;
        v[i].color = colors[i]; v[i].specular = 0; v[i].tu = v[i].tv = 0.0f;
    }
    g_render.Count = 0;
    Callback_DrawFans(v, 4);
    if (g_render.Count != 3)
        return Fail();
    const int rimA[3] = { 1, 2, 3 };
    const int rimB[3] = { 2, 3, 1 };
    for (int t = 0; t < 3; ++t) {
        const STriangle& tri = g_render.Drawn[t];
        if (tri.P[0].X != 20.0f || tri.P[0].Y != 2.0f || tri.P[1].X != xs[rimA[t]] * 2.0f || tri.P[1].Y != ys[rimA[t]] * 2.0f ||
            tri.P[2].X != xs[rimB[t]] * 2.0f || tri.P[2].Y != ys[rimB[t]] * 2.0f || tri.Flags != 1 || tri.A != 0x80)
            return Fail();
        unsigned int c = colors[rimA[t]];
        if (tri.R != ((c >> 16) & 0xFF) || tri.G != ((c >> 8) & 0xFF) || tri.B != (c & 0xFF))
            return Fail();
    }
    g_render.Count = 0;
    Callback_DrawFans(v, 1);                       // centre only: nothing drawn
    Callback_DrawFans(v, 0);
    if (g_render.Count != 0)
        return Fail();
    std::printf("00404e10_TEST PASS\n");
    return 0;
}