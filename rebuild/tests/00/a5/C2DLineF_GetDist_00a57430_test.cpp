#include <cstdio>
#include <cmath>
// Behaviour copy of C2DLineF::GetDist(const C2DLineF&, float*, float*, bool*) (0x00A57430) over simple stand-in callees.
struct Vec {
    float X, Y;
    Vec() {}
    Vec(float x, float y) : X(x), Y(y) {}
    Vec operator-(const Vec& o) const { return Vec(X - o.X, Y - o.Y); }
    Vec operator+(const Vec& o) const { return Vec(X + o.X, Y + o.Y); }
    Vec operator*(float s) const { return Vec(X * s, Y * s); }
    float Dot(const Vec& o) const { return X * o.X + Y * o.Y; }
    float LenSq() const { return X * X + Y * Y; }
};
struct Seg {
    Vec Point1, Point2;
    Seg(float a, float b, float c, float d) : Point1(a, b), Point2(c, d) {}
    bool Crosses(const Seg& o) const
    {
        Vec d = Point2 - Point1, e = o.Point2 - o.Point1;
        float den = d.X * e.Y - d.Y * e.X;
        if (den == 0.0f) return false;
        Vec w = o.Point1 - Point1;
        float t = (w.X * e.Y - w.Y * e.X) / den, u = (w.X * d.Y - w.Y * d.X) / den;
        return t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f;
    }
    // clamped closest point; projected = unclamped foot; before = foot before Point1
    float PointOnLine(const Vec& p, Vec& closest, Vec* projected, bool* before) const
    {
        Vec d = Point2 - Point1;
        float t = (p - Point1).Dot(d) / d.LenSq();
        if (before) *before = t < 0.0f;
        if (projected) *projected = Point1 + d * t;
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        closest = Point1 + d * t;
        return 0.0f;
    }
    float GetDist(const Seg& other, float* endDist, float* projDist, bool* onLeft) const
    {
        if (Crosses(other)) { if (endDist) *endDist = 0.0f; if (projDist) *projDist = 0.0f; return 0.0f; }
        Vec c0, c1, c2, c3, p2, p3;
        bool b2, b3;
        other.PointOnLine(Point1, c0, 0, 0);
        other.PointOnLine(Point2, c1, 0, 0);
        float d2 = PointOnLine(other.Point1, c2, &p2, &b2);
        float d3 = PointOnLine(other.Point2, c3, &p3, &b3);
        if (endDist) {
            *endDist = d2 < d3 ? d2 : d3;
            if (*endDist != 0.0f && b2 != b3) *endDist = 0.0f;
        }
        if (projDist) {
            float a = (other.Point1 - p2).LenSq(), b = (other.Point2 - p3).LenSq();
            *projDist = (float)std::sqrt(a < b ? a : b);
        }
        int nearest = 0;
        float s[4];
        s[0] = (Point1 - c0).LenSq(); s[1] = (Point2 - c1).LenSq();
        s[2] = (other.Point1 - c2).LenSq(); s[3] = (other.Point2 - c3).LenSq();
        float best = s[0];
        for (int i = 1; i < 4; ++i) if (s[i] < best) { best = s[i]; nearest = i; }
        if (onLeft) {
            Vec dir = Point2 - Point1, perp(-dir.Y, dir.X), v;
            if (nearest == 0) v = c0 - Point1;
            else if (nearest == 1) v = c1 - Point2;
            else if (nearest == 2) v = other.Point1 - c2;
            else v = other.Point2 - c3;
            *onLeft = v.Dot(perp) > 0.0f;
        }
        return (float)std::sqrt(best);
    }
};
int main() {
    Seg base(0.0f, 0.0f, 4.0f, 0.0f);
    float e = -1, p = -1; bool side = false;
    Seg cross(2.0f, -1.0f, 2.0f, 1.0f);
    bool ok = base.GetDist(cross, &e, &p, &side) == 0.0f && e == 0.0f && p == 0.0f;
    Seg above(1.0f, 3.0f, 3.0f, 5.0f);
    float d = base.GetDist(above, &e, &p, &side);
    ok = ok && d == 3.0f && side == true && p == 3.0f;
    Seg below(1.0f, -2.0f, 3.0f, -6.0f);
    d = base.GetDist(below, 0, 0, &side);
    ok = ok && d == 2.0f && side == false;
    if (!ok) { std::printf("00a57430_TEST FAIL\n"); return 1; }
    std::printf("00a57430_TEST PASS\n"); return 0;
}