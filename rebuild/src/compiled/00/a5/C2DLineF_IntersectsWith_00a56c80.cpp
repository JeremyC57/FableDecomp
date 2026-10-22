// C2DLineF::IntersectsWith(const C2DLineF&, C2DVector&, float) const -- segment intersection with tolerance bounds
// Grade: RELOCATION_MATCH (retry A 11a) -- the first projection dot is sequenced (pa = A.X*P.X; pa += A.Y*P.Y) and t is a double local, so t stays on the x87 stack for both scaled components
// Leaf: IntersectsWith
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
namespace std {
template <class T> inline const T& min(const T& a, const T& b) { return b < a ? b : a; }
template <class T> inline const T& max(const T& a, const T& b) { return a < b ? b : a; }
}
class C2DVector {
public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector& Normalise();   // 0x00A14510
};
class C2DLineF {
public:
    float X1, Y1, X2, Y2;
    float GetLowestX() const;    // 0x00A56B80
    float GetHighestX() const;   // 0x00A56BB0
    float GetLowestY() const;    // 0x00A56BE0
    float GetHighestY() const;   // 0x00A56C10
    bool IntersectsWith(const C2DLineF& other, C2DVector& intersection, float tolerance) const;
};

bool C2DLineF::IntersectsWith(const C2DLineF& other, C2DVector& intersection, float tolerance) const
{
    const double& lo1 = std::min<double>(X1, X2);
    const double& hi1 = std::max<double>(other.X1, other.X2);
    if ((float)(lo1 - hi1) > tolerance)
        return false;
    const double& lo2 = std::min<double>(other.X1, other.X2);
    const double& hi2 = std::max<double>(X1, X2);
    if ((float)(lo2 - hi2) > tolerance)
        return false;
    const double& lo3 = std::min<double>(Y1, Y2);
    const double& hi3 = std::max<double>(other.Y1, other.Y2);
    if ((float)(lo3 - hi3) > tolerance)
        return false;
    const double& lo4 = std::min<double>(other.Y1, other.Y2);
    const double& hi4 = std::max<double>(Y1, Y2);
    if ((float)(lo4 - hi4) > tolerance)
        return false;
    if ((float)fabs(other.X1 - other.X2) < tolerance && (float)fabs(other.Y1 - other.Y2) < tolerance)
        return false;
    if ((float)fabs(X1 - X2) < tolerance && (float)fabs(Y1 - Y2) < tolerance)
        return false;
    C2DVector dirA, dirB, point;
    point.X = X1 - other.X1;
    point.Y = Y1 - other.Y1;
    dirA.X = X2 - X1;
    dirA.Y = Y2 - Y1;
    dirB.X = other.X2 - other.X1;
    dirB.Y = other.Y2 - other.Y1;
    dirA.Normalise();
    dirB.Normalise();
    float dot = dirA.X * dirB.X + dirA.Y * dirB.Y;
    float denominator = dot * dot - 1.0f;
    if (denominator != 0.0f) {
        float pa = dirA.X * point.X;
        pa += dirA.Y * point.Y;
        double t = (pa - (dirB.X * point.X + dirB.Y * point.Y) * dot) / denominator;
        {
            C2DVector scaled(dirA.X * t, dirA.Y * t);
            point.X = scaled.X + X1;
            point.Y = scaled.Y + Y1;
        }
        float limit = -tolerance;
        if (!(point.X - GetLowestX() > limit))
            return false;
        if (!(GetHighestX() - point.X > limit))
            return false;
        if (!(point.Y - GetLowestY() > limit))
            return false;
        if (!(GetHighestY() - point.Y > limit))
            return false;
        if (!(point.X - other.GetLowestX() > limit))
            return false;
        if (!(other.GetHighestX() - point.X > limit))
            return false;
        if (!(point.Y - other.GetLowestY() > limit))
            return false;
        if (!(other.GetHighestY() - point.Y > limit))
            return false;
        intersection = point;
        return true;
    }
    return false;
}