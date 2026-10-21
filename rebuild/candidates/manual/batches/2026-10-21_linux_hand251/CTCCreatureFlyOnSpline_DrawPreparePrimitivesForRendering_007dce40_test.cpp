#include <cstdio>
// behaviour test: with a spline, DrawPreparePrimitivesForRendering emits one yellow (255,255,0,255) 0.05-wide line per handle,
// line i joining the spline points at progress i/n and (i+1)/n (clamped to [0,1]); without a spline or handles nothing is drawn
struct SLine { int handle; float from, to; unsigned char r, g, b, a; float width; };
static SLine g_lines[16]; static int g_n;
static void Draw(bool hasSpline, int handles)
{
    if (!hasSpline)
        return;
    float progress = 0.0f, last = 0.0f;               // position == progress for the test spline
    for (int i = 0; i != handles; ++i) {
        progress += 1.0f / (float)handles;
        if (progress < 0.0f) progress = 0.0f; else if (progress > 1.0f) progress = 1.0f;
        SLine l = { i, last, progress, 255, 255, 0, 255, 0.05f };
        g_lines[g_n++] = l;
        last = progress;
    }
}
int main()
{
    Draw(false, 4);
    bool ok = g_n == 0;
    Draw(true, 0);
    ok = ok && g_n == 0;
    Draw(true, 4);
    ok = ok && g_n == 4 && g_lines[0].from == 0.0f && g_lines[0].to == 0.25f && g_lines[3].handle == 3 &&
         g_lines[3].to > 0.999f && g_lines[3].to <= 1.0f && g_lines[2].from == g_lines[1].to && g_lines[1].b == 0 &&
         g_lines[1].r == 255 && g_lines[1].width == 0.05f;
    if (!ok) { std::printf("007dce40_TEST FAIL\n"); return 1; }
    std::printf("007dce40_TEST PASS\n");
    return 0;
}
