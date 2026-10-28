// C3DVector::AccurateNormalise() -- returns *this; scale the vector to unit length with a true square root
// Leaf: ?AccurateNormalise@C3DVector@@QAEAAV1@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A14440, 61 bytes, no calls. Global: 1.0f @ 0x0122DED8.
// Name: Ego_r/FableWin PDB ?AccurateNormalise@C3DVector@@QAEAAV1@XZ -- the sqrt-based twin of the table-driven
//   C2DVector::Normalise (landed 0x00A14540); returns this (`mov eax,ecx`), unlike GetMagnitudeAndNormalise
//   (0x00A14480, returns the magnitude). Already bound by its callers (e.g. landed 0x0084CDF0, 0x00A0BE30). The catalog
//   label "C3DVector::GetScaled" is propagated (GetScaled takes a vector and returns by value).
// Shape: the components are read into locals first (three `fld`), `x*x + y*y + z*z` under the sqrt intrinsic, then
//   `1.0f / length` (`fdivr [1.0]`) multiplied into each member.
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);

#pragma pack(push, 1)
class C3DVector {
public:
    float X;   // +0x00
    float Y;   // +0x04
    float Z;   // +0x08
    C3DVector& AccurateNormalise();
};
#pragma pack(pop)

C3DVector& C3DVector::AccurateNormalise()
{
    float x = X;
    float y = Y;
    float z = Z;
    float length = (float)sqrt(x * x + y * y + z * z);
    float inverse = 1.0f / length;
    X *= inverse;
    Y *= inverse;
    Z *= inverse;
    return *this;
}