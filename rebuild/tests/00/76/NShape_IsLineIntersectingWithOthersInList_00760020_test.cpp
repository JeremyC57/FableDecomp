// Behaviour test for 0x00760020 NShape::IsLineIntersectingWithOthersInList (links the REAL row object). The row walks the closed
// polygon points_list edge by edge (the last edge closes back to the first point) and counts the edges that are the line itself in
// either direction (both end points within squared distance DELTA_NOTIONAL_ZERO) or that C2DLineF::IntersectsWith reports crossing;
// the result is count > 4. Out-of-line callee: C2DLineF::IntersectsWith (0x00A56C80, thiscall, 3 stack args) = a segment-crossing
// stub that also counts its calls (the equal-edge path must not call it). Fixtures: C2DLineF {P1 X,Y; P2 X,Y}, the STLport vector
// {start, finish, end_of_storage} of C3DVector. Expected values come from an independent model of the same rule.
#include <stdio.h>
#pragma comment(linker, "/alternatename:@CallRow@8=?IsLineIntersectingWithOthersInList@NShape@@YI_NABVC2DLineF@@ABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z")
#pragma comment(linker, "/alternatename:?IntersectsWith@C2DLineF@@QBE_NABV1@AAVC2DVector@@M@Z=@StubIntersects@20")
extern const float DELTA_NOTIONAL_ZERO = 1e-4f;
struct V3 { float X, Y, Z; };
struct V2 { float X, Y; };
struct Line { V2 P1, P2; };
struct Vec3s { V3* start; V3* finish; V3* eos; };
extern "C" bool __fastcall CallRow(const Line* line, const Vec3s* points);
static int g_calls;
static Line g_seen[16];                                  // the edges IntersectsWith was called on, in order
static bool Cross(const Line& a, const Line& b)
{
    float d1x = a.P2.X - a.P1.X, d1y = a.P2.Y - a.P1.Y, d2x = b.P2.X - b.P1.X, d2y = b.P2.Y - b.P1.Y;
    float den = d1x * d2y - d1y * d2x;
    if (den == 0) return false;
    float ex = b.P1.X - a.P1.X, ey = b.P1.Y - a.P1.Y;
    float t = (ex * d2y - ey * d2x) / den, u = (ex * d1y - ey * d1x) / den;
    return t >= 0 && t <= 1 && u >= 0 && u <= 1;
}
extern "C" bool __fastcall StubIntersects(const Line* self, int, const Line* other, V2* out, float tol)
{
    if (g_calls < 16) g_seen[g_calls] = *self;
    ++g_calls;
    if (tol != 1e-4f || !out) return false;
    return Cross(*self, *other);
}
static int g_fail = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d\n", line); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
static bool Same(V2 a, V2 b) { float dx = a.X - b.X, dy = a.Y - b.Y; return dx * dx + dy * dy < 1e-4f; }
static Line g_want[16];
static void Model(const Line& l, const V3* p, int n, int& count, int& calls)
{
    count = 0; calls = 0;
    for (int i = 0; i != n; ++i) {
        const V3& s = p[i]; const V3& e = p[(i + 1) % n];
        Line edge = { { s.X, s.Y }, { e.X, e.Y } };
        if ((Same(edge.P1, l.P1) && Same(edge.P2, l.P2)) || (Same(edge.P1, l.P2) && Same(edge.P2, l.P1))) ++count;
        else { if (calls < 16) g_want[calls] = edge; ++calls; if (Cross(edge, l)) ++count; }
    }
}
static void Run(const Line& l, V3* p, int n, int line)
{
    Vec3s v = { p, p + n, p + n };
    int count, calls; Model(l, p, n, count, calls);
    g_calls = 0;
    bool r = CallRow(&l, &v);
    Check(r == (count > 4), line);
    Check(g_calls == calls, line);
    for (int i = 0; i < calls && i < 16; ++i)             // each edge runs from its start point to the next point
        Check(g_seen[i].P1.X == g_want[i].P1.X && g_seen[i].P1.Y == g_want[i].P1.Y && g_seen[i].P2.X == g_want[i].P2.X && g_seen[i].P2.Y == g_want[i].P2.Y, line);
}
int main()
{
    // zig-zag polygon crossing y = 0 many times
    V3 zig[8] = { { 0, 1, 5 }, { 1, -1, 5 }, { 2, 1, 5 }, { 3, -1, 5 }, { 4, 1, 5 }, { 5, -1, 5 }, { 6, 1, 5 }, { 3, 4, 5 } };
    Line across = { { -1, 0 }, { 10, 0 } };
    Run(across, zig, 8, __LINE__);                     // 6 crossings: true
    Line shortl = { { -1, 0 }, { 2.5f, 0 } };
    Run(shortl, zig, 8, __LINE__);                     // 3 crossings: false
    Line five = { { -1, 0 }, { 5, 0 } }, four = { { -1, 0 }, { 4, 0 } };
    Run(five, zig, 8, __LINE__);                       // exactly 5 crossings: true (count > 4, not > 5)
    { Vec3s v = { zig, zig + 8, zig + 8 }; CHECK(CallRow(&five, &v)); CHECK(!CallRow(&four, &v)); }
    Run(four, zig, 8, __LINE__);                       // exactly 4: false
    // square, line through it: 2 crossings
    V3 sq[4] = { { 0, 0, 0 }, { 4, 0, 0 }, { 4, 4, 0 }, { 0, 4, 0 } };
    Line through = { { -1, 2 }, { 5, 2 } };
    Run(through, sq, 4, __LINE__);
    // the line is one of the polygon's edges, in both directions (no IntersectsWith call for it)
    Line edgef = { { 4, 0 }, { 4, 4 } }, edger = { { 4, 4.005f }, { 4.005f, 0 } };
    Run(edgef, sq, 4, __LINE__);
    Run(edger, sq, 4, __LINE__);
    // the closing edge (last point back to the first) equals the line
    Line closing = { { 0, 4 }, { 0, 0 } };
    Run(closing, sq, 4, __LINE__);
    // five equal edges in a degenerate list: count 5 > 4 without any crossing
    V3 deg[6] = { { 0, 0, 0 }, { 1, 0, 0 }, { 0, 0, 0 }, { 1, 0, 0 }, { 0, 0, 0 }, { 1, 0, 0 } };
    Line unit = { { 0, 0 }, { 1, 0 } };
    Run(unit, deg, 6, __LINE__);
    { Vec3s v = { deg, deg + 6, deg + 6 }; g_calls = 0; CHECK(CallRow(&unit, &v)); CHECK(g_calls == 0); }
    // exactly four counted: false
    Run(unit, deg, 4, __LINE__);
    { Vec3s v = { deg, deg + 4, deg + 4 }; CHECK(!CallRow(&unit, &v)); }
    if (!g_fail) printf("00760020_TEST PASS\n");
    return g_fail;
}