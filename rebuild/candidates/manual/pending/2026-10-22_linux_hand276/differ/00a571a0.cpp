// C2DLineF::GetDistanceToPoint(p, closest, radius, projected, before) -- distance from p to a capsule-trimmed segment, with the clamped closest point
// Grade: DIFFER(653v654) -- x87 operand order of the length square (retail X*X first) and p/before esi/edx swap; GetLength/GetLengthSquared/inline sqrt/named-local forms all keep Y*Y first
// Leaf: GetDistanceToPoint
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    float Dot(const C2DVector& o) const { return X * o.X + Y * o.Y; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
};
struct C2DLineF {
    C2DVector Point1;   // +0x00
    C2DVector Point2;   // +0x08
    C2DVector GetClosestPoint(const C2DVector& p) const;     // 0x00A57140
    float GetDistanceToPoint(const C2DVector& p, C2DVector& closest, float radius, C2DVector* projected, bool* before) const;
};

float C2DLineF::GetDistanceToPoint(const C2DVector& p, C2DVector& closest, float radius, C2DVector* projected, bool* before) const
{
    C2DVector dir = Point2 - Point1;
    float len = dir.GetLength();
    if (len == 0.0f) {
        closest = Point1;
        if (projected)
            *projected = Point1;
        if (before)
            *before = false;
        return 0.0f;
    }
    C2DVector u = dir * (1.0f / len);
    float proj = (p - Point1).Dot(u);
    if (before)
        *before = proj < 0.0f;
    if (radius * 2.0f > len) {
        closest = (Point2 + Point1) * 0.5f;
        if (projected)
            *projected = GetClosestPoint(p);
        return radius - len;
    }
    if (proj < radius) {
        closest = Point1 + u * radius;
        float result = radius - proj;
        if (projected)
            *projected = Point1 + u * proj;
        return result;
    }
    if (len - radius < proj) {
        closest = Point2 - u * radius;
        float result = proj - (len + radius);
        if (projected)
            *projected = Point1 + u * proj;
        return result;
    }
    closest = Point1 + u * proj;
    if (projected)
        *projected = closest;
    return 0.0f;
}
