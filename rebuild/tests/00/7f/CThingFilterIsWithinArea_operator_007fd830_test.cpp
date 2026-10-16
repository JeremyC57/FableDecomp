#include <cstdio>
struct V { float X, Y, Z; };
struct Thing { float physRadius; V pos; };
static V LineOut(const V& s, const V& e, const V& p)
{
    // closest point on segment s..e to p, return p - closest
    float dx = e.X - s.X, dy = e.Y - s.Y, dz = e.Z - s.Z;
    float len2 = dx * dx + dy * dy + dz * dz;
    float t = len2 > 0 ? ((p.X - s.X) * dx + (p.Y - s.Y) * dy + (p.Z - s.Z) * dz) / len2 : 0;
    if (t < 0) t = 0; if (t > 1) t = 1;
    V r = { p.X - (s.X + t * dx), p.Y - (s.Y + t * dy), p.Z - (s.Z + t * dz) };
    return r;
}
struct F { V* Start; V* End; float Radius; bool IncludeThingRad;
    bool operator()(const Thing* t) const {
        float tr = 0.0f; if (IncludeThingRad) tr = t->physRadius;
        V out = LineOut(*Start, *End, t->pos);
        float r = tr + Radius;
        return out.X * out.X + out.Y * out.Y < r * r; } };
int main()
{
    V s = { 0, 0, 0 }, e = { 10, 0, 0 };
    F f = { &s, &e, 1.0f, false };
    Thing a = { 1.0f, { 5, 1.5f, 0 } };
    bool ok = !f(&a);
    f.IncludeThingRad = true; ok = ok && f(&a);
    a.pos.Y = 0.5f; f.IncludeThingRad = false; ok = ok && f(&a);
    a.pos.X = 12; ok = ok && !f(&a);
    if (!ok) { std::printf("007fd830_TEST FAIL\n"); return 1; }
    std::printf("007fd830_TEST PASS\n");
    return 0;
}