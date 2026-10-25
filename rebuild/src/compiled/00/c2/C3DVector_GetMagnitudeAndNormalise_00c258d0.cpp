// C3DVector::GetMagnitudeAndNormalise() -- scale the vector to unit length and return its old length; the zero vector is left alone and 0 returned
// Leaf: GetMagnitudeAndNormalise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C258D0, 151 bytes (`ret`, result on the FPU stack). The manifest's
//   "Math_Vector3Normalize" is close; the body is C3DVector::GetMagnitudeAndNormalise (the Ego_r.pdb name of the
//   non-const `float ()` member that normalises and returns the old length) of the 0x00C25xxx vector helpers (callers
//   0x00C1D4E3/ED/F7 normalise the three rows of a 3x3 frame and drop the result with `fstp st(0)`).
// Shape: the three components are copied into float locals first (integer moves into the frame, read from ecx
//   directly; a `C3DVector v = *this;` struct copy goes through `mov eax,ecx` instead); the
//   zero test compares each component with the 0.0f literal (pool 0x0122DEDC; `fld 0 ; fld c ; fucompp ;
//   test ah,44h ; jp` per component, `&&` chain), the length is sqrt of the sum of squares (fsqrt), the
//   reciprocal `1.0f / len` multiplies each copied component back into the members, the length stays on the
//   FPU stack as the result.
#include <math.h>

struct C3DVector {
    float X, Y, Z;
    float GetMagnitudeAndNormalise();
};

float C3DVector::GetMagnitudeAndNormalise()
{
    float x = X, y = Y, z = Z;
    if (x == 0.0f && y == 0.0f && z == 0.0f)
        return 0.0f;
    float length = (float)sqrt(x * x + y * y + z * z);
    float inv = 1.0f / length;
    X = x * inv;
    Y = y * inv;
    Z = z * inv;
    return length;
}