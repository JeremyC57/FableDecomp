// CTools::IsPosInViewingFrustrum(const C3DVector& pos, const C3DVector& lookAt, const C3DVector& eye, float fov) -- 1 when the direction eye->pos lies inside the cone of half-angle fov/2 (fov as a fraction of a turn) around eye->lookAt
// Leaf: IsPosInViewingFrustrum
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875120, length 164 bytes (`ret 10h`, `this` unused; 176 with padding).
// Callees: C3DVector::Normalise @ 0x00A14440 (x2). Constants: 0x0122F59C = 0.5f, 0x0124F2B8 = (double)6.2831855f.
// Shape: the two differences are inline C3DVector operator- results built straight in their locals (user
// constructor); the dot product is written y, x, z so VC7.1's right-operand-first FPU order gives retail's z, x, y
// loads; `cos` is the intrinsic over `fov * 0.5f * (double)TWO_PI`; the result is a long (`mov eax,1`).
#include <math.h>
#pragma intrinsic(cos)
struct C3DVector {
    float x, y, z;
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(x - o.x, y - o.y, z - o.z); }
    void Normalise();                                       // 0x00A14440
};
static const float TWO_PI = 6.28318548f;

struct CTools {
    long IsPosInViewingFrustrum(const C3DVector& pos, const C3DVector& lookAt, const C3DVector& eye, float fov);
};

long CTools::IsPosInViewingFrustrum(const C3DVector& pos, const C3DVector& lookAt, const C3DVector& eye, float fov)
{
    C3DVector view = lookAt - eye;
    view.Normalise();
    C3DVector dir = pos - eye;
    dir.Normalise();
    if (view.y * dir.y + view.x * dir.x + view.z * dir.z > cos(fov * 0.5f * (double)TWO_PI))
        return 1;
    return 0;
}
