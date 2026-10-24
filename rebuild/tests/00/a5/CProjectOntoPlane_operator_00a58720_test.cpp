// Behaviour test for 0x00A58720 CProjectOntoPlane::operator()(const C3DVector&) const
#include <cstdio>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float operator*(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct CProjectOntoPlane {
    const C3DVector* Origin;
    const C3DVector* AxisX;
    const C3DVector* AxisY;
    C2DVector operator()(const C3DVector& p) const
    {
        C3DVector d = p - *Origin;
        return C2DVector(d * *AxisX, d * *AxisY);
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    C3DVector origin(1.0f, 1.0f, 1.0f), ax(1.0f, 0.0f, 0.0f), ay(0.0f, 0.0f, 1.0f);
    CProjectOntoPlane f;
    f.Origin = &origin;
    f.AxisX = &ax;
    f.AxisY = &ay;
    C2DVector r = f(C3DVector(4.0f, 6.0f, 9.0f));       // (3,5,8) . X = 3, . Z = 8
    C3DVector diag(0.6f, 0.8f, 0.0f);
    f.AxisX = &diag;
    C2DVector s = f(C3DVector(4.0f, 5.0f, 1.0f));       // (3,4,0) . (0.6,0.8,0) = 5
    if (!Near(r.X, 3.0f) || !Near(r.Y, 8.0f) || !Near(s.X, 5.0f) || !Near(s.Y, 0.0f)) {
        std::printf("00a58720_TEST FAIL\n");
        return 1;
    }
    std::printf("00a58720_TEST PASS\n");
    return 0;
}