#include <cstdio>
#include <math.h>
// Behaviour test for CConsole::DrawHistoryScrollBar @ 0x009EAD60: at the right edge of the history window (window
// coordinates carry a 3000 offset) the up arrow is drawn while lines after the visible ones exist (finish != end), the
// down arrow while lines before them exist (start != begin), and the bar box always spans the visible fraction of the
// history between the window top and the bottom margin. Same arithmetic as the reconstruction (implicit double->float
// conversions of the virtual pixel / screen scales).
static int g_failures = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++g_failures; } } while (0)
static bool Near(float a, float b) { return fabs(a - b) < 0.001f; }

struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour { unsigned char B, G, R, A; };

struct CRenderManager2D {
    int Triangles;
    C2DVector TriangleVerts[4][3];
    unsigned long TriangleFlags[4];
    CRGBColour TriangleColour[4];
    int Boxes;
    C2DBoxF Box;
    long BoxFlags;
    CRGBColour BoxColour;
    bool Draw2DTriangle(const C2DVector& v0, const C2DVector& v1, const C2DVector& v2, const CRGBColour& colour,
                        unsigned long flags)
    {
        TriangleVerts[Triangles][0] = v0;
        TriangleVerts[Triangles][1] = v1;
        TriangleVerts[Triangles][2] = v2;
        TriangleColour[Triangles] = colour;
        TriangleFlags[Triangles] = flags;
        ++Triangles;
        return true;
    }
    bool Draw2DBox(const C2DBoxF& box, const CRGBColour& colour, long flags)
    {
        Box = box;
        BoxColour = colour;
        BoxFlags = flags;
        ++Boxes;
        return true;
    }
};
struct CDisplayManager {
    CRenderManager2D* PRenderManager2D;
};
struct CSystemManager {
    CDisplayManager* PDisplayManager;
};
static CRenderManager2D g_renderer;
static CDisplayManager g_display;
static CSystemManager g_system;
static int g_systemCalls = 0;
CSystemManager* GFGetSystemManager()
{
    ++g_systemCalls;
    return &g_system;
}
double g_virtualPixelScaleX = 1.5;
double g_virtualPixelScaleY = 2.0;
double g_virtualScreenWidth = 640.0;
double g_virtualScreenHeight = 480.0;

struct CHistoryList {
    struct Node { Node* Next; Node* Prev; };
    Node* Sentinel;
    Node* begin() const { return Sentinel->Next; }
    Node* end() const { return Sentinel; }
    unsigned int size() const
    {
        unsigned int n = 0;
        for (Node* p = Sentinel->Next; p != Sentinel; p = p->Next)
            ++n;
        return n;
    }
};
inline int Distance(CHistoryList::Node* first, CHistoryList::Node* last)
{
    int n = 0;
    CHistoryList::Node* it = first;
    for (; it != last; it = it->Next)
        ++n;
    return n;
}

struct CConsole {
    virtual void v00() { }
    CHistoryList        CommandLineHistory;
    CHistoryList::Node* VisibleHistoryStart;
    CHistoryList::Node* VisibleHistoryFinish;
    CRGBColour          ScrollBarColour;
    void DrawHistoryScrollBar(const C2DBoxF& window);
};

void CConsole::DrawHistoryScrollBar(const C2DBoxF& window)
{
    float bottomOffset = g_virtualPixelScaleY * -2.0;
    float barWidth = g_virtualPixelScaleX * 3.0;
    C2DVector arrowOffset(-(g_virtualPixelScaleX * 2.0 + barWidth), 3000.0f);
    float arrowHeight = g_virtualScreenHeight * 0.015f;
    float arrowWidth = g_virtualScreenWidth * 0.02f;
    float right = arrowOffset.X + window.BRX - 3000.0f;
    C2DVector up[3];
    float middle = right - arrowWidth * 0.5f;
    up[0].X = middle;
    float top = window.TLY + 3000.0f;
    up[0].Y = top;
    up[1].X = right;
    up[1].Y = top + arrowHeight;
    up[2].X = right - arrowWidth;
    up[2].Y = top + arrowHeight;
    C2DVector down[3];
    down[0].X = middle;
    float bottom = bottomOffset + window.BRY;
    down[0].Y = bottom;
    down[1].X = right - arrowWidth;
    down[1].Y = bottom - arrowHeight;
    down[2].X = right;
    down[2].Y = bottom - arrowHeight;
    CRenderManager2D* renderer = GFGetSystemManager()->PDisplayManager->PRenderManager2D;
    if (VisibleHistoryFinish != CommandLineHistory.end())
        renderer->Draw2DTriangle(up[0], up[1], up[2], ScrollBarColour, 0);
    if (VisibleHistoryStart != CommandLineHistory.begin())
        renderer->Draw2DTriangle(down[0], down[1], down[2], ScrollBarColour, 0);
    float barBottom = bottomOffset + window.BRY;
    float barHeight = barBottom - (window.TLY + 3000.0f);
    float startFraction = (float)Distance(CommandLineHistory.begin(), VisibleHistoryStart) / (float)CommandLineHistory.size();
    float finishFraction = (float)Distance(CommandLineHistory.begin(), VisibleHistoryFinish) / (float)CommandLineHistory.size();
    C2DBoxF bar;
    bar.TLX = window.BRX - 3000.0f - barWidth;
    bar.TLY = barBottom - finishFraction * barHeight;
    bar.BRX = window.BRX - 3000.0f;
    bar.BRY = barBottom - startFraction * barHeight;
    renderer->Draw2DBox(bar, ScrollBarColour, 1);
}

static CHistoryList::Node g_sentinel;
static CHistoryList::Node g_nodes[10];
static void BuildHistory()
{
    CHistoryList::Node* prev = &g_sentinel;
    for (int i = 0; i < 10; ++i) {
        prev->Next = &g_nodes[i];
        g_nodes[i].Prev = prev;
        prev = &g_nodes[i];
    }
    prev->Next = &g_sentinel;
    g_sentinel.Prev = prev;
}
static void ResetRenderer()
{
    g_renderer.Triangles = 0;
    g_renderer.Boxes = 0;
}

int main()
{
    g_display.PRenderManager2D = &g_renderer;
    g_system.PDisplayManager = &g_display;
    BuildHistory();
    CConsole console;
    console.CommandLineHistory.Sentinel = &g_sentinel;
    console.ScrollBarColour.B = 1; console.ScrollBarColour.G = 2; console.ScrollBarColour.R = 3; console.ScrollBarColour.A = 4;
    C2DBoxF window;
    window.TLX = 0.0f; window.TLY = -2980.0f; window.BRX = 3600.0f; window.BRY = 400.0f;

    // expected geometry: bottom margin -4, bar width 4.5, arrow offset -(3 + 4.5), arrow 12.8 x 7.2
    const float right = 592.5f, middle = 586.1f, top = 20.0f, bottom = 396.0f, height = 7.2f, width = 12.8f;

    // 1. lines 2..6 visible out of 10: both arrows, bar from 6/10 to 2/10 of the 376-pixel track
    ResetRenderer();
    console.VisibleHistoryStart = &g_nodes[2];
    console.VisibleHistoryFinish = &g_nodes[6];
    console.DrawHistoryScrollBar(window);
    CHECK(g_systemCalls == 1);
    CHECK(g_renderer.Triangles == 2 && g_renderer.Boxes == 1);
    CHECK(Near(g_renderer.TriangleVerts[0][0].X, middle) && Near(g_renderer.TriangleVerts[0][0].Y, top));
    CHECK(Near(g_renderer.TriangleVerts[0][1].X, right) && Near(g_renderer.TriangleVerts[0][1].Y, top + height));
    CHECK(Near(g_renderer.TriangleVerts[0][2].X, right - width) && Near(g_renderer.TriangleVerts[0][2].Y, top + height));
    CHECK(Near(g_renderer.TriangleVerts[1][0].X, middle) && Near(g_renderer.TriangleVerts[1][0].Y, bottom));
    CHECK(Near(g_renderer.TriangleVerts[1][1].X, right - width) && Near(g_renderer.TriangleVerts[1][1].Y, bottom - height));
    CHECK(Near(g_renderer.TriangleVerts[1][2].X, right) && Near(g_renderer.TriangleVerts[1][2].Y, bottom - height));
    CHECK(g_renderer.TriangleFlags[0] == 0 && g_renderer.TriangleFlags[1] == 0);
    CHECK(g_renderer.TriangleColour[1].R == 3 && g_renderer.TriangleColour[1].A == 4);
    CHECK(Near(g_renderer.Box.TLX, 595.5f) && Near(g_renderer.Box.BRX, 600.0f));
    CHECK(Near(g_renderer.Box.TLY, 396.0f - 0.6f * 376.0f) && Near(g_renderer.Box.BRY, 396.0f - 0.2f * 376.0f));
    CHECK(g_renderer.BoxFlags == 1 && g_renderer.BoxColour.G == 2);

    // 2. the whole history visible: no arrows, the bar fills the track
    ResetRenderer();
    console.VisibleHistoryStart = &g_nodes[0];
    console.VisibleHistoryFinish = &g_sentinel;
    console.DrawHistoryScrollBar(window);
    CHECK(g_renderer.Triangles == 0 && g_renderer.Boxes == 1);
    CHECK(Near(g_renderer.Box.TLY, top) && Near(g_renderer.Box.BRY, bottom));

    // 3. scrolled to the newest lines (finish at the end): only the down arrow
    ResetRenderer();
    console.VisibleHistoryStart = &g_nodes[5];
    console.VisibleHistoryFinish = &g_sentinel;
    console.DrawHistoryScrollBar(window);
    CHECK(g_renderer.Triangles == 1);
    CHECK(Near(g_renderer.TriangleVerts[0][0].Y, bottom) && Near(g_renderer.TriangleVerts[0][1].X, right - width));
    CHECK(Near(g_renderer.Box.TLY, top) && Near(g_renderer.Box.BRY, 396.0f - 0.5f * 376.0f));

    // 4. scrolled to the oldest lines (start at the beginning): only the up arrow; scales follow the globals
    ResetRenderer();
    g_virtualPixelScaleY = 1.0;
    g_virtualScreenHeight = 1000.0;
    console.VisibleHistoryStart = &g_nodes[0];
    console.VisibleHistoryFinish = &g_nodes[3];
    console.DrawHistoryScrollBar(window);
    CHECK(g_renderer.Triangles == 1);
    CHECK(Near(g_renderer.TriangleVerts[0][0].Y, top) && Near(g_renderer.TriangleVerts[0][1].Y, top + 15.0f));
    CHECK(Near(g_renderer.Box.BRY, 398.0f) && Near(g_renderer.Box.TLY, 398.0f - 0.3f * 378.0f));

    if (g_failures == 0)
        printf("009ead60_TEST PASS\n");
    return g_failures;
}