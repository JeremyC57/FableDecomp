// C2DVector::AccurateNormalise() -- returns *this; scale the vector to unit length with a true square root
// Leaf: ?AccurateNormalise@C2DVector@@QAEAAV1@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A14510, 42 bytes, no calls. Global: 1.0f @ 0x0122DED8.
// Name: Ego_r/FableWin PDB ?AccurateNormalise@C2DVector@@QAEAAV1@XZ -- the sqrt twin sitting right before the table-driven
//   C2DVector::Normalise (landed 0x00A14540); already bound to this address by its callers. The catalog label
//   "C2DVector::Dot" is propagated (Dot takes a vector and returns a float).
// Shape: as C3DVector::AccurateNormalise 0x00A14440 with two components.
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);

class C2DVector {
public:
    float X;   // +0x00
    float Y;   // +0x04
    C2DVector& AccurateNormalise();
};

C2DVector& C2DVector::AccurateNormalise()
{
    float x = X;
    float y = Y;
    float length = (float)sqrt(x * x + y * y);
    float inverse = 1.0f / length;
    X *= inverse;
    Y *= inverse;
    return *this;
}