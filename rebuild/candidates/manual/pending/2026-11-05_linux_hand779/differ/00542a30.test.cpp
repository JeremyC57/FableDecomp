// Behaviour test for NUISystem::CIconText::Draw (0x00542A30): a standalone model of the draw decision -- an invisible icon text
// (alpha 0 or zoom.X <= 0) still draws for one frame (NoRenderNextFrame set) and is released on the next; a text whose
// position is at or beyond the back-buffer width (both x and y are tested against the width) is released; CENTER / RIGHT
// alignment shift x left by half / all of Size.X (no scale); x and y are rounded to nearest; the drop shadow sits at +2,+2
// on the next layer; an unavailable text swaps the colours; with a parent handle and a child index the two text primitives
// are child primitives, otherwise they are persistent and one sprite per embedded icon is placed at position + offset with
// size scale * icon size. Release clears two children (parent handle) or the text, shadow and icon handles.
#include <cstdio>
#include <cmath>
#include <vector>
enum { LEFT, CENTER, RIGHT };
static float RoundNear(float v) { float f = std::floor(v); float d = v - f; if (d > 0.5f || (d == 0.5f && ((long)f & 1))) f += 1.0f; return f; }   // round to nearest, ties to even (the x87 default)
struct Icon { float sizeX, sizeY, offX, offY; };
struct Box { float l, t, r, b; };
struct Out { int action; float x, y, sx, sy; bool textBlack; int childIndex; std::vector<Box> boxes; };   // 1 child draw, 2 persistent, 3 clear children, 4 clear handles
static Out Draw(unsigned char alpha, float zoomX, unsigned char& noRender, float x, float y, int align, float scaleX, float scaleY,
                float sizeX, bool unavailable, bool parent, bool hasIndex, int index, float bbw, const std::vector<Icon>& icons)
{
    Out o; o.action = 0; o.x = o.y = o.sx = o.sy = 0; o.textBlack = false; o.childIndex = index;
    if (alpha <= 0 || zoomX <= 0.0f) {
        if (noRender == 1) { if (parent) { o.action = 3; o.childIndex += 2; } else o.action = 4; return o; }
        noRender = 1;
    } else noRender = 0;
    if (x >= bbw || y >= bbw) { o.action = 4; return o; }
    if (align == CENTER) x -= sizeX * 0.5f; else if (align == RIGHT) x -= sizeX;
    x = RoundNear(x); y = RoundNear(y);
    o.x = x; o.y = y; o.sx = x + 2.0f; o.sy = y + 2.0f; o.textBlack = unavailable;
    if (parent && hasIndex) { o.action = 1; o.childIndex += 2; return o; }
    o.action = 2;
    for (size_t i = 0; i < icons.size(); ++i) {
        Box b; b.l = x + icons[i].offX; b.t = y + icons[i].offY; b.r = b.l + scaleX * icons[i].sizeX; b.b = b.t + scaleY * icons[i].sizeY;
        o.boxes.push_back(b);
    }
    return o;
}
int main()
{
    int fail = 0;
    unsigned char nr = 0;
    std::vector<Icon> none, two;
    Icon a = { 10, 8, 1, 2 }, b = { 4, 4, 30, -1 }; two.push_back(a); two.push_back(b);
    Out r1 = Draw(255, 1.0f, nr, 10.4f, 20.6f, LEFT, 2.0f, 3.0f, 50.0f, false, false, false, 0, 640, two);
    if (r1.action != 2 || r1.x != 10.0f || r1.y != 21.0f || r1.sx != 12.0f || r1.sy != 23.0f || nr != 0 || r1.boxes.size() != 2) fail = 1;
    else if (r1.boxes[0].l != 11.0f || r1.boxes[0].t != 23.0f || r1.boxes[0].r != 31.0f || r1.boxes[0].b != 47.0f || r1.boxes[1].l != 40.0f || r1.boxes[1].b != 32.0f) fail = 2;
    Out r2 = Draw(255, 1.0f, nr, 100.0f, 0.0f, CENTER, 2.0f, 2.0f, 30.0f, true, true, true, 5, 640, two);
    if (r2.action != 1 || r2.x != 85.0f || !r2.textBlack || r2.childIndex != 7 || !r2.boxes.empty()) fail = 3;   // centre: no scale on the shift
    Out r3 = Draw(255, 1.0f, nr, 100.0f, 0.0f, RIGHT, 1.0f, 1.0f, 30.0f, false, true, false, 5, 640, none);
    if (r3.action != 2 || r3.x != 70.0f || r3.boxes.size() != 0) fail = 4;
    Out r4 = Draw(0, 1.0f, nr, 100.0f, 0.0f, LEFT, 1.0f, 1.0f, 30.0f, false, false, false, 0, 640, none);
    if (r4.action != 2 || nr != 1) fail = 5;                          // first invisible frame still draws
    Out r5 = Draw(0, 1.0f, nr, 100.0f, 0.0f, LEFT, 1.0f, 1.0f, 30.0f, false, true, true, 3, 640, none);
    if (r5.action != 3 || r5.childIndex != 5) fail = 6;               // second invisible frame clears two children
    nr = 0;
    Out r6 = Draw(255, 1.0f, nr, 10.0f, 640.0f, LEFT, 1.0f, 1.0f, 30.0f, false, false, false, 0, 640, none);
    if (r6.action != 4) fail = 7;                                     // y tested against the width too
    if (fail) { std::printf("00542a30_TEST FAIL %d\n", fail); return 1; }
    std::printf("00542a30_TEST PASS\n");
    return 0;
}
