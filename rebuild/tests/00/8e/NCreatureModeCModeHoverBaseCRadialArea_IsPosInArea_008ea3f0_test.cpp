// Behaviour test for 0x008EA3F0 CRadialArea::IsPosInArea: sector edges at ThetaLow/ThetaHigh turns measured from +Y
// towards +X, optional radius limit, zero offset always inside.
#include <cstdio>
#include <math.h>

static const float TWO_PI = 6.2831855f;

class C2DVector {
public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
    float Dot(const C2DVector& other) const { return X * other.X + Y * other.Y; }
};
class CRadialArea {
public:
    float ThetaLow, ThetaHigh, RadHigh;
    bool IsPosInArea(const C2DVector& pos, bool checkRadius) const
    {
        float length = pos.GetLength();
        if (length < 0.0001f)
            return true;
        double thetaLow = ThetaLow * (double)TWO_PI;
        C2DVector low;
        low.X = (float)sin(thetaLow);
        low.Y = (float)cos(thetaLow);
        double thetaHigh = ThetaHigh * (double)TWO_PI;
        C2DVector high;
        high.X = (float)sin(thetaHigh);
        high.Y = (float)cos(thetaHigh);
        float scale = 1.0f / length;
        C2DVector direction;
        direction.X = pos.X * scale;
        direction.Y = pos.Y * scale;
        if (!checkRadius || length < RadHigh) {
            float arcCos = high.Dot(low);
            if (direction.Dot(low) >= arcCos && direction.Dot(high) >= arcCos)
                return true;
        }
        return false;
    }
};

int main()
{
    CRadialArea area;
    area.ThetaLow = 0.0f;       // +Y
    area.ThetaHigh = 0.25f;     // +X
    area.RadHigh = 5.0f;
    int fail = 0;
    if (!area.IsPosInArea(C2DVector(0.0f, 0.0f), true)) fail = 1;          // zero offset
    if (!area.IsPosInArea(C2DVector(1.0f, 1.0f), true)) fail = 1;          // 45 degrees, inside
    if (area.IsPosInArea(C2DVector(-1.0f, 1.0f), false)) fail = 1;         // outside the sector
    if (area.IsPosInArea(C2DVector(1.0f, -1.0f), false)) fail = 1;         // outside the sector
    if (area.IsPosInArea(C2DVector(6.0f, 6.0f), true)) fail = 1;           // beyond RadHigh
    if (!area.IsPosInArea(C2DVector(6.0f, 6.0f), false)) fail = 1;         // radius check off
    if (fail) { std::printf("008ea3f0_TEST FAIL\n"); return 1; }
    std::printf("008ea3f0_TEST PASS\n");
    return 0;
}