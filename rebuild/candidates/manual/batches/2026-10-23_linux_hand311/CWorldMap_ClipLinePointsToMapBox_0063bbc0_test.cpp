// Behaviour test for CWorldMap::ClipLinePointsToMapBox (0x0063BBC0): a point outside the map box slides along the line to each side plane it is beyond.
#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; V3() {} V3(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static V3 Sub(const V3& a, const V3& b) { return V3(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static float Dot(const V3& a, const V3& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static V3 Cross(const V3& a, const V3& b) { return V3(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
struct Plane {
    V3 N; float D;
    void Init(const V3& a, const V3& b, const V3& c) { N = Cross(Sub(b, a), Sub(c, a)); float l = std::sqrt(Dot(N, N)); N = V3(N.X / l, N.Y / l, N.Z / l); D = Dot(N, a); }
    bool Hit(const V3& p, const V3& d, V3* out) const { float den = Dot(N, d); if (den == 0.0f) return false; float t = (D - Dot(p, N)) / den; *out = V3(d.X * t + p.X, d.Y * t + p.Y, d.Z * t + p.Z); return true; }
};
struct Box { int TLX, TLY, BRX, BRY; };
static void Clip(const Box& box, V3& point, const V3& towards)
{
    int x = (int)point.X, y = (int)point.Y;
    if (x >= box.TLX && x < box.BRX && y >= box.TLY && y < box.BRY) return;
    V3 start = point;
    V3 dir = Sub(towards, point);
    float inv = 1.0f / std::sqrt(Dot(dir, dir));
    dir = V3(dir.X * inv, dir.Y * inv, dir.Z * inv);
    float w = (float)(box.BRX - box.TLX), h = (float)(box.BRY - box.TLY);
    Plane plane;
    if (point.X < 0.0f) { plane.Init(V3(0, 0, 0), V3(0, h, 0), V3(0, 0, 1)); plane.Hit(start, dir, &point); }
    if (point.Y < 0.0f) { plane.Init(V3(0, 0, 0), V3(w, 0, 0), V3(0, 0, 1)); plane.Hit(start, dir, &point); }
    if (w < point.X) { plane.Init(V3(w, 0, 0), V3(w, h, 0), V3(w, 0, 1)); plane.Hit(start, dir, &point); }
    if (h < point.Y) { plane.Init(V3(0, h, 0), V3(w, h, 0), V3(0, h, 1)); plane.Hit(start, dir, &point); }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 0.001f; }
int main()
{
    Box box = { 0, 0, 100, 50 };
    V3 inside(10.0f, 10.0f, 3.0f);
    Clip(box, inside, V3(20.0f, 20.0f, 0.0f));
    if (!Near(inside.X, 10.0f) || !Near(inside.Y, 10.0f)) { std::printf("inside 0063bbc0_TEST FAIL\n"); return 1; }
    V3 left(-10.0f, 20.0f, 0.0f);
    Clip(box, left, V3(10.0f, 20.0f, 4.0f));
    if (!Near(left.X, 0.0f) || !Near(left.Y, 20.0f) || !Near(left.Z, 2.0f)) { std::printf("%f %f %f 0063bbc0_TEST FAIL\n", left.X, left.Y, left.Z); return 1; }
    V3 far(120.0f, 70.0f, 0.0f);
    Clip(box, far, V3(80.0f, 30.0f, 0.0f));
    if (!Near(far.X, 100.0f) || !Near(far.Y, 50.0f)) { std::printf("%f %f 0063bbc0_TEST FAIL\n", far.X, far.Y); return 1; }
    std::printf("0063bbc0_TEST PASS\n");
    return 0;
}
