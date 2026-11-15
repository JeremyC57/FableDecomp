// Behaviour test for 0x009A25D0 CStreamingFontBank::Render: a standalone model of the row's layout logic with a small glyph table.
// Checked: flag 8 centres the block VERTICALLY (cursor_y = floor(line_y - extents.Y / 2), cursor_x untouched: retail 0x009A2676..
// 0x009A268F); at each line start flag 2 right-aligns and flag 4 centres horizontally from line_x; '\n' returns to line_x and moves
// down by the font height; 0xA0 is drawn as a space; a space only advances (rounded half away from zero, signed char); a missing
// glyph is skipped; every glyph emits a quad (x0,y1)(x0,y0)(x1,y0)(x1,y1) with the -0.5 pixel offset and the cell UVs; the vertex
// buffer is flushed when more than 0xFC vertices are queued (64 quads) and the rest at the end, triangle count = vertices / 2.
#include <cstdio>
#include <cmath>
#include <vector>
struct Glyph { unsigned short cell; signed char ox, oy; unsigned char w, h, adv; };
struct Quad { float x0, y0, x1, y1, u, v; };
static const float kAdjScale = 64.0f, kBaseline = 2.0f; // one adjustment record: Scale 64 (weight 1.0), Baseline 2
static const Glyph* Lookup(unsigned short c)
{
    static const Glyph a = { 37, 1, 3, 8, 10, 9 }, sp = { 0, 0, 0, 0, 0, 5 };
    if (c == 'a') return &a;
    if (c == 0x20) return &sp;
    return 0;
}
static float ExtentX(const unsigned short* p)
{
    float x = 0;
    while (*p && *p != '\n') { unsigned short c = *p++; if (c == 0xa0) c = 0x20; const Glyph* g = Lookup(c); if (g) x += g->adv; }
    return x;
}
static signed char Round(float v) { return (signed char)(long)(v < 0.0f ? v - 0.5f : v + 0.5f); }
static std::vector<Quad> quads; static std::vector<long> flushes;
static void Render(const unsigned short* text, float px, float py, float scale, unsigned long flags, float blockH, float fontH)
{
    float line_x = px, line_y = py, cursor_x = line_x, cursor_y = line_y;
    if (flags & 8)
        cursor_y = (float)std::floor(line_y - blockH * 0.5f);
    unsigned long count = 0; long new_line = 1; const unsigned short* p = text;
    while (*p) {
        if (new_line) {
            if (flags & 6) {
                float ex = ExtentX(p);
                if (flags & 2) cursor_x = (float)std::floor(line_x - ex);
                if (flags & 4) cursor_x = (float)std::floor(line_x - ex * 0.5f);
            }
            new_line = 0;
        }
        unsigned short c = *p++;
        if (c == '\n') { cursor_x = line_x; cursor_y += fontH; new_line = 1; continue; }
        if (c == 0xa0) c = 0x20;
        const Glyph* g = Lookup(c);
        if (!g) continue;
        float ws = kAdjScale * (1.0f / 64.0f);
        if (c == 0x20) { cursor_x += (float)Round(g->adv * ws); continue; }
        float gs = scale * ws;
        Quad q;
        q.u = (float)(g->cell % 16) / 16.0f; q.v = (float)(g->cell / 16) / 8.0f;
        q.x0 = (cursor_x - line_x) * scale + g->ox * gs + line_x - 0.5f;
        q.y0 = (cursor_y - line_y) * scale + (kBaseline + g->oy) * gs + line_y - 0.5f;
        q.x1 = g->w * gs + q.x0; q.y1 = g->h * gs + q.y0;
        cursor_x += (float)Round(g->adv * ws);
        quads.push_back(q);
        count += 4;
        if (count > 0xfc) { flushes.push_back((long)count >> 1); count = 0; }
    }
    if (count > 0) flushes.push_back((long)count >> 1);
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main()
{
    const unsigned short t1[] = { 'a', 0x20, 'a', 0xa0, 'a', 'z', '\n', 'a', 0 };
    Render(t1, 100.0f, 50.0f, 1.0f, 0, 0, 20.0f);
    check(quads.size() == 4, "one quad per drawable glyph, missing glyph skipped");
    check(quads[0].x0 == 100.5f && quads[0].y0 == 54.5f && quads[0].x1 == 108.5f && quads[0].y1 == 64.5f, "quad corners with -0.5 offset");
    check(quads[1].x0 == 114.5f && quads[2].x0 == 128.5f, "space and 0xA0 advance by the rounded width");
    check(quads[3].x0 == 100.5f && quads[3].y0 == 74.5f, "newline returns to line_x and moves down one font height");
    check(quads[0].u == 5.0f / 16.0f && quads[0].v == 2.0f / 8.0f, "cell UVs");
    check(flushes.size() == 1 && flushes[0] == 8, "final flush, triangle count = vertices / 2");
    quads.clear(); flushes.clear();
    Render(t1, 100.0f, 50.0f, 1.0f, 8, 41.0f, 20.0f);
    check(quads[0].x0 == 100.5f && quads[0].y0 == 33.5f, "flag 8 centres vertically only (cursor_y = floor(50 - 20.5) = 29)");
    quads.clear(); flushes.clear();
    const unsigned short t2[] = { 'a', 'a', '\n', 'a', 0 };
    Render(t2, 100.0f, 0.0f, 1.0f, 4, 0, 10.0f);
    check(quads[0].x0 == 91.5f && quads[2].x0 == 95.5f, "flag 4 centres each line from line_x");
    quads.clear(); flushes.clear();
    Render(t2, 100.0f, 0.0f, 1.0f, 2, 0, 10.0f);
    check(quads[0].x0 == 82.5f && quads[2].x0 == 91.5f, "flag 2 right-aligns each line");
    quads.clear(); flushes.clear();
    std::vector<unsigned short> t3(70, 'a'); t3.push_back(0);
    Render(&t3[0], 0.0f, 0.0f, 1.0f, 0, 0, 10.0f);
    check(flushes.size() == 2 && flushes[0] == 128 && flushes[1] == 12, "flush after 64 quads (> 0xFC vertices), rest at the end");
    check(Round(2.5f) == 3 && Round(-2.5f) == -3 && Round(2.4f) == 2, "round half away from zero");
    if (!fails) std::printf("009a25d0_TEST PASS\n");
    return fails;
}