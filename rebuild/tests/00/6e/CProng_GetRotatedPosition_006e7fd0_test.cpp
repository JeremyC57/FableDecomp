#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    void Rotate(const C3DVector& axis, float angle);
};
static C3DVector g_axis(0.0f, 0.0f, 0.0f);
static float g_angle = 0.0f;
void C3DVector::Rotate(const C3DVector& axis, float angle)
{   // stub: quarter turn about Z (records its arguments)
    g_axis = axis; g_angle = angle;
    float x = X; X = -Y; Y = x;
}
struct CProng {
    bool      Blocked;
    C3DVector Prong;
    long      ProngBlockageTimer;
    C3DVector GetRotatedPosition(float angle)
    {
        C3DVector v = Prong;
        v.Rotate(C3DVector(0.0f, 0.0f, 1.0f), angle);
        return v;
    }
};
int main()
{
    CProng p; p.Blocked = false; p.Prong = C3DVector(2.0f, 3.0f, 4.0f); p.ProngBlockageTimer = 0;
    C3DVector r = p.GetRotatedPosition(1.5f);
    if (r.X != -3.0f || r.Y != 2.0f || r.Z != 4.0f || g_angle != 1.5f || g_axis.X != 0.0f || g_axis.Y != 0.0f || g_axis.Z != 1.0f ||
        p.Prong.X != 2.0f || p.Prong.Y != 3.0f) { std::printf("006e7fd0_TEST FAIL\n"); return 1; }
    std::printf("006e7fd0_TEST PASS\n");
    return 0;
}