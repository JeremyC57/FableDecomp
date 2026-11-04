// Behaviour test for NUISystem::CText::Draw (0x0054EF00): a standalone model of the draw decision -- an invisible text (alpha 0
// or zoom.X <= 0) still draws for one frame (NoRenderNextFrame set) and is released on the next; an off-screen text
// (x >= back-buffer width or y >= height) is released; CENTER / RIGHT alignment shift x left by half / all of scale * Size.X;
// x and y are rounded to nearest; the drop shadow sits at +2,+2; an unavailable text swaps colours (text black with the alpha,
// shadow in the text colour); with a parent handle and a child index both primitives are child primitives (index advanced twice),
// otherwise the two persistent handles are set. Release clears two children (parent handle) or both handles.
#include <cstdio>
#include <cmath>
enum { LEFT, CENTER, RIGHT };
struct Out { int action; float x, y, sx, sy; bool textBlack; int childIndex; };   // action 0 none, 1 child draw, 2 persistent, 3 clear child, 4 clear handles
static Out Draw(unsigned char alpha, float zoomX, unsigned char& noRender, float x, float y, int align, float scale, float sizeX,
                bool unavailable, bool parent, bool hasIndex, int index, float bbw, float bbh)
{
    Out o = { 0, 0, 0, 0, 0, false, index };
    bool release = false;
    if (alpha <= 0 || zoomX <= 0.0f) { if (noRender == 1) release = true; else noRender = 1; }
    else noRender = 0;
    if (!release && (x >= bbw || y >= bbh)) release = true;
    if (release) {
        if (parent) { o.action = 3; o.childIndex += 2; } else o.action = 4;
        return o;
    }
    if (align == CENTER) x -= scale * sizeX * 0.5f; else if (align == RIGHT) x -= scale * sizeX;
    x = (float)std::nearbyint(x); y = (float)std::nearbyint(y);
    o.x = x; o.y = y; o.sx = x + 2.0f; o.sy = y + 2.0f; o.textBlack = unavailable;
    if (parent && hasIndex) { o.action = 1; o.childIndex += 2; } else o.action = 2;
    return o;
}
int main()
{
    int fail = 0;
    unsigned char nr = 0;
    Out a = Draw(255, 1.0f, nr, 10.4f, 20.6f, LEFT, 1.0f, 50.0f, false, false, false, 0, 640, 480);
    if (a.action != 2 || a.x != 10.0f || a.y != 21.0f || a.sx != 12.0f || a.sy != 23.0f || nr != 0) fail = 1;
    Out b = Draw(255, 1.0f, nr, 100.0f, 0.0f, CENTER, 2.0f, 30.0f, true, true, true, 5, 640, 480);
    if (b.action != 1 || b.x != 70.0f || !b.textBlack || b.childIndex != 7) fail = 2;
    Out c = Draw(255, 1.0f, nr, 100.0f, 0.0f, RIGHT, 1.0f, 30.0f, false, true, false, 5, 640, 480);
    if (c.action != 2 || c.x != 70.0f) fail = 3;
    Out d = Draw(0, 1.0f, nr, 100.0f, 0.0f, LEFT, 1.0f, 30.0f, false, false, false, 0, 640, 480);
    if (d.action != 2 || nr != 1) fail = 4;                          // first invisible frame still draws
    Out e = Draw(0, 1.0f, nr, 100.0f, 0.0f, LEFT, 1.0f, 30.0f, false, true, true, 3, 640, 480);
    if (e.action != 3 || e.childIndex != 5) fail = 5;                // second invisible frame clears two children
    nr = 0;
    Out f = Draw(255, 1.0f, nr, 640.0f, 0.0f, LEFT, 1.0f, 30.0f, false, false, false, 0, 640, 480);
    if (f.action != 4) fail = 6;                                     // off screen: clear both handles
    if (fail) { std::printf("0054ef00_TEST FAIL %d\n", fail); return 1; }
    std::printf("0054ef00_TEST PASS\n");
    return 0;
}
