// C3DVector::GetMagnitudeAndNormalise() -- scale the vector to unit length (true square root) and return its former length
// Leaf: ?GetMagnitudeAndNormalise@C3DVector@@QAEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A14480, 61 bytes, no calls. Global: 1.0f @ 0x0122DED8.
// Name: Ego_r/FableWin PDB ?GetMagnitudeAndNormalise@C3DVector@@QAEMXZ -- the body leaves the length in st(0) as the float
//   result (`fld [1.0] ; fdiv st,st(1)` keeps it below the reciprocal) and does not return this, unlike AccurateNormalise
//   0x00A14440 next to it. The catalog label "C3DVector::GetScaled" is propagated; the binding
//   ?GetLength@C3DVector@@QBEMXZ -> 0x00A14480 in reloc_bindings is wrong (GetLength does not write the vector).
// Shape: components read into locals, `x*x + y*y + z*z` under the sqrt intrinsic, `1.0f / length` multiplied in, length
//   returned.
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);

#pragma pack(push, 1)
class C3DVector {
public:
    float X;   // +0x00
    float Y;   // +0x04
    float Z;   // +0x08
    float GetMagnitudeAndNormalise();
};
#pragma pack(pop)

float C3DVector::GetMagnitudeAndNormalise()
{
    float x = X;
    float y = Y;
    float z = Z;
    float length = (float)sqrt(x * x + y * y + z * z);
    float inverse = 1.0f / length;
    X *= inverse;
    Y *= inverse;
    Z *= inverse;
    return length;
}