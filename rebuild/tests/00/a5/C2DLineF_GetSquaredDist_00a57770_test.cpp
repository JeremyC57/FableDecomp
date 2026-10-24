// Behaviour test for 0x00A57770 C2DLineF::GetSquaredDist(const C2DVector&)
#include <cstdio>

class C2DVector {
public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    float GetSquaredMagnitude() const { return X * X + Y * Y; }
};

class C2DLineF {
public:
    C2DVector Point1;
    C2DVector Point2;
    // 0x00A571A0 stand-in: the closest point of the segment to p (projection clamped to the end points)
    float GetPointOnLine(const C2DVector& p, C2DVector& closest, float, C2DVector*, bool*) const
    {
        C2DVector d = Point2 - Point1;
        C2DVector q = p - Point1;
        float len2 = d.GetSquaredMagnitude();
        float t = len2 > 0.0f ? (q.X * d.X + q.Y * d.Y) / len2 : 0.0f;
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        closest = C2DVector(Point1.X + d.X * t, Point1.Y + d.Y * t);
        return t;
    }
    float GetSquaredDist(const C2DVector& point)
    {
        C2DVector closest;
        GetPointOnLine(point, closest, 0.0f, 0, 0);
        return (point - closest).GetSquaredMagnitude();
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    C2DLineF line;
    line.Point1 = C2DVector(0.0f, 0.0f);
    line.Point2 = C2DVector(4.0f, 0.0f);
    bool ok = Near(line.GetSquaredDist(C2DVector(1.0f, 3.0f)), 9.0f) &&     // perpendicular foot inside
              Near(line.GetSquaredDist(C2DVector(7.0f, 4.0f)), 25.0f) &&    // beyond Point2: 3-4-5
              Near(line.GetSquaredDist(C2DVector(2.0f, 0.0f)), 0.0f);       // on the line
    if (!ok) { std::printf("00a57770_TEST FAIL\n"); return 1; }
    std::printf("00a57770_TEST PASS\n");
    return 0;
}