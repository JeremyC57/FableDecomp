#include <cstdio>
// Behaviour model of CShape::EditorDrawHighlightPoints (0x007617A0): one ground-snapped vertical post per shape point
// (bottom at ground + 0.15, the second end 0.25 further along Y and snapped the same way), a marker when the cursor is
// within 0.25 (squared XY distance below 0.0625), then a cross at every selected point snapped to ground + 0.15.
struct V { float X, Y, Z; };
static float Ground(const V& p) { return p.X * 0.5f + p.Y; }
struct Recorder { int lines; int markers; int crosses; V lastStart; V lastEnd; V lastCross; };
static void Draw(const V* points, int count, const V* const* selected, int selectedCount, const V& cursor, Recorder& r)
{
    for (int i = 0; i < count; ++i) {
        V top = points[i];
        V bottom = top; bottom.Y = top.Y + 0.25f;
        top.Z = Ground(top) + 0.15f;
        bottom.Z = Ground(bottom) + 0.15f;
        float dx = top.X - cursor.X;
        float dy = top.Y - cursor.Y;
        if (dy * dy + dx * dx < 0.0625f) ++r.markers;
        ++r.lines; r.lastStart = top; r.lastEnd = bottom;
    }
    for (int j = 0; j < selectedCount; ++j) {
        V p = *selected[j];
        p.Z = Ground(p) + 0.15f;
        ++r.crosses; r.lastCross = p;
    }
}
static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
int main()
{
    V points[3] = { { 0.0f, 0.0f, 9.0f }, { 2.0f, 1.0f, 9.0f }, { 4.0f, 4.0f, 9.0f } };
    const V* selected[1] = { &points[1] };
    V cursor = { 2.1f, 1.1f, 0.0f };
    Recorder r = { 0, 0, 0 };
    Draw(points, 3, selected, 1, cursor, r);
    bool ok = r.lines == 3 && r.markers == 1 && r.crosses == 1
        && Near(r.lastStart.Z, 4.0f * 0.5f + 4.0f + 0.15f) && Near(r.lastEnd.Y, 4.25f) && Near(r.lastEnd.Z, 2.0f + 4.25f + 0.15f)
        && Near(r.lastCross.Z, 1.0f + 1.0f + 0.15f);
    Recorder e = { 0, 0, 0 };
    Draw(points, 0, selected, 0, cursor, e);
    ok = ok && e.lines == 0 && e.crosses == 0;
    if (!ok) { std::printf("007617a0_TEST FAIL\n"); return 1; }
    std::printf("007617a0_TEST PASS\n");
    return 0;
}
