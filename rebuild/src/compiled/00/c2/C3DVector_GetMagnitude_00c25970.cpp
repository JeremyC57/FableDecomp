// C3DVector::GetMagnitude() const -- the Euclidean length sqrt(x*x + y*y + z*z) of the vector
// Leaf: GetMagnitude
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C25970, 33 bytes (`ret`, result on the FPU stack). The manifest's
//   "Math_Vector3Length" is close; the body is C3DVector::GetMagnitude (the Ego_r.pdb name of the `float () const`
//   length member) of the 0x00C25xxx vector helpers (callers
//   0x00C2035D, 0x00C45879, 0x00C625C8 take the length of an operator- result).
// Shape: all three components are loaded onto the FPU stack first (Z, Y, X: the right-to-left argument evaluation of
//   an inline three-float helper), squared and summed X, Y, Z from stack registers (`fld st(0) ; fmul st(1)` ...),
//   then fsqrt and the three argument copies popped (`fstp st(3) ; fstp st(0) ; fstp st(0)`).
#include <math.h>

inline float Magnitude(float x, float y, float z)
{
    return (float)sqrt(x * x + y * y + z * z);
}

class C3DVector { public:
    float X, Y, Z;
    float GetMagnitude() const;
};

float C3DVector::GetMagnitude() const
{
    return Magnitude(X, Y, Z);
}