#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CBase {
    virtual void v00() { }
    unsigned char _body[0xa0];
};
struct CCameraPointModeBase : CBase {
    virtual C3DVector GetLookDirection() const { return C3DVector(1.0f, 0.0f, 0.0f); }
};
int main()
{
    CCameraPointModeBase m;
    C3DVector d = m.GetLookDirection();
    if (d.X != 1.0f || d.Y != 0.0f || d.Z != 0.0f) { std::printf("0056e2b0_TEST FAIL\n"); return 1; }
    std::printf("0056e2b0_TEST PASS\n");
    return 0;
}