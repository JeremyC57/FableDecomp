// Behaviour test for NScript::CV_ChickenKickingScript::CKickedChicken::Main (0x00E64210): the landing-sector search (for each pair of
// adjacent angle markers around the arena, rotate the first marker direction by half the pair's angle -- angle in turns, acos / 2pi --
// and take the sector when the flattened strike direction lies within that half angle of the bisector; the last matching pair wins),
// the foul-line cross-product test (Z of (foulA - hero) ^ (foulB - hero) > 0 means a foul, band -1), and the distance band from the
// four line distances.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {} };
static V Sub(const V& a, const V& b) { return V(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static V Cross(const V& a, const V& b) { return V(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
static void Norm(V& v) { float m = std::sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); v.X /= m; v.Y /= m; v.Z /= m; }
static void RotateZ(V& v, float turns) { double r = turns * 6.2831855; float x = (float)(v.X * std::cos(r) - v.Y * std::sin(r));
    float y = (float)(v.X * std::sin(r) + v.Y * std::cos(r)); v.X = x; v.Y = y; }
static long Sector(const V& arena, const V* markers, int n, const V& strike)
{
    float dy = strike.Y - arena.Y;
    float dx = strike.X - arena.X;
    V dir; dir.X = dx; dir.Z = 0; dir.Y = dy; Norm(dir);
    long final = 0, sector = 1;
    for (int i = 1; i < n; ++i) {
        V a = Sub(markers[i - 1], arena), b = Sub(markers[i], arena); b.Z = 0; a.Z = 0;
        if (a.X * a.X + a.Y * a.Y > 1e-6f && b.X * b.X + b.Y * b.Y > 1e-6f) {
            Norm(a); Norm(b);
            float c = a.Z * b.Z; c += a.Y * b.Y; c += a.X * b.X;
            if (c < -1.0f) c = -1.0f; else if (c > 1.0f) c = 1.0f;
            float angle = 0.5f * ((float)std::acos(c) * 0.15915494f);
            RotateZ(a, angle);
            if (dir.X * a.X + dir.Y * a.Y + dir.Z * a.Z > std::cos(angle * 6.2831855)) final = sector;
        }
        ++sector;
    }
    return final;
}
static bool Foul(const V& fa, const V& fb, const V& hero) { return Cross(Sub(fa, hero), Sub(fb, hero)).Z > 0.0f; }
static long Band(float d, float d1, float d2, float d3, float d4)
{ return d > d4 ? 4 : d > d3 ? 3 : d > d2 ? 2 : d > d1 ? 1 : 0; }
int main()
{
    V arena(0, 0, 0);
    V m[4] = { V(10, 0, 5), V(7.0710678f, 7.0710678f, 0), V(0, 10, 2), V(-10, 0, 0) };   // 0, 45, 90, 180 degrees
    bool ok = Sector(arena, m, 4, V(5, 1, 9)) == 1;         // ~11 degrees: between markers 1 and 2
    ok = ok && Sector(arena, m, 4, V(2, 5, 0)) == 2;        // ~68 degrees
    ok = ok && Sector(arena, m, 4, V(-3, 3, 0)) == 3;       // 135 degrees
    ok = ok && Sector(arena, m, 4, V(0, -5, 0)) == 0;       // behind the markers
    V m2[3] = { V(10, 0, 0), V(0, 0, 3), V(0, 10, 0) };      // a degenerate (zero flat length) marker skips both of its pairs
    ok = ok && Sector(arena, m2, 3, V(5, 5, 0)) == 0;
    V fa(-5, 10, 0), fb(5, 10, 0);
    ok = ok && !Foul(fa, fb, V(0, 0, 0)) && Foul(fa, fb, V(0, 12, 0));
    ok = ok && Band(25, 10, 20, 30, 40) == 2 && Band(45, 10, 20, 30, 40) == 4 && Band(5, 10, 20, 30, 40) == 0
        && Band(30, 10, 20, 30, 40) == 2;
    std::printf(ok ? "00e64210_TEST PASS\n" : "00e64210_TEST FAIL\n");
    return ok ? 0 : 1;
}