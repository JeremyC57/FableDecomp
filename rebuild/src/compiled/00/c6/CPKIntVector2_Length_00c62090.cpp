// CPKIntVector2::Length() -- Euclidean length of a 2-D integer vector (integer squares summed, then fsqrt)
// Leaf: ?Length@CPKIntVector2@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C62090, 28 bytes (thiscall, returns float in st0).
// Callees: none (`sqrt` is the /Oi intrinsic: `fild; fsqrt`).
// Identity: the manifest's "Math_Vector2Length" is right in substance. Its caller 0x00C4695C builds the vector on the
//   stack from two loop integers and compares the length against a float radius. Descriptive class name.
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

class CPKIntVector2 {
public:
    long x, y;
    float Length() const;
};

float CPKIntVector2::Length() const
{
    return (float)sqrt((double)(x * x + y * y));
}