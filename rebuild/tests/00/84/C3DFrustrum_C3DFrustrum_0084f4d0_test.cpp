#include <cstdio>
// Behaviour model of 0x0084F4D0 `anonymous namespace'::C3DFrustrum::C3DFrustrum (the ctor has internal linkage in its
// TU, so the test carries the model): copy the apex and up vectors, start with an empty point vector and a zero FOV.
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct SFrustrumPoint { C3DVector Position; float Angle; };
struct SPointVector { SFrustrumPoint* _M_start; SFrustrumPoint* _M_finish; SFrustrumPoint* _M_end_of_storage; SPointVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {} };
class C3DFrustrum {
public:
    C3DFrustrum(const C3DVector& apex, const C3DVector& up) : Apex(apex), Up(up), FOV(0.0f) {}
    C3DVector Apex; C3DVector Up; SPointVector Points; float FOV;
};
int main()
{
    if (sizeof(C3DFrustrum) != 0x28) { std::printf("0084f4d0_TEST FAIL size\n"); return 1; }
    C3DVector apex(1.0f, 2.0f, 3.0f), up(0.0f, 0.0f, 1.0f);
    C3DFrustrum f(apex, up);
    if (f.Apex.X != 1.0f || f.Apex.Y != 2.0f || f.Apex.Z != 3.0f || f.Up.Z != 1.0f || f.Up.X != 0.0f
        || f.Points._M_start != 0 || f.Points._M_finish != 0 || f.Points._M_end_of_storage != 0 || f.FOV != 0.0f)
        { std::printf("0084f4d0_TEST FAIL\n"); return 1; }
    std::printf("0084f4d0_TEST PASS\n");
    return 0;
}
