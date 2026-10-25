// PreProcessStickVector(C2DVector& stick) -- analogue-stick conditioning for the look-around input: remove a dead zone (0.25 on X, 0.23 on Y) and rescale the remainder to 0..1 keeping the sign, then apply a squared response curve per axis
// Leaf: ?PreProcessStickVector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068D5F0, 272 bytes (266 without padding), anonymous-namespace `__fastcall` (ecx = stick),
//   plain `ret`. Ego_r.pdb `?PreProcessStickVector@?A0xc5e6a41b@@YIXAAVC2DVector@@@Z`; called from
//   CInputProcessFirstPersonLookAround::ProcessInput 0x0068D700 (and the other look-around input processes).
// Callees: the CRT pow intrinsic _CIpow 0x00BFEB40 (twice; fabs is the inline `fabs` instruction). Float pool: 0.25f
//   0x0123078C, 0.0f 0x0122DEDC, 4/3 0x01246054, 0.23f 0x0125BF00, 1/0.77 0x0125BEFC, 2.0 (double) 0x01244480.
// Shape: the X result stays on the x87 stack across the Y computation (compare with `fcom`, no reload) while the Y result is
//   homed in a frame float; the sign of each pre-curve value is an inline `value < 0 ? -1 : 1` long (written -1 then 1 into a
//   frame dword for `fimul`). The curve is an inline helper whose `magnitude` local produces retail's dead `fst` of |value|
//   ahead of the pow call (both calls share its frame word); the X dead-zone remainder multiplies by the sign first
//   (`fimul`), the Y one loads the sign first (`fild ; fmul st(1)`) -- the operand orders of the source expressions.
// Label: the landed caller 0x0068D700 declares it `PreProcessStickVector(C2DVector*)`; the PDB parameter is a reference (same code).
extern "C" double __cdecl fabs(double);
extern "C" double __cdecl pow(double, double);
#pragma intrinsic(fabs, pow)
class C2DVector {
public:
    float X, Y;
};
namespace {
inline long Sign(float value) { return value < 0.0f ? -1 : 1; }
inline float ApplyResponseCurve(float value)
{
    long sign = Sign(value);
    float magnitude = (float)fabs(value);
    return (float)fabs(pow(magnitude, 2.0)) * sign;
}

void __fastcall PreProcessStickVector(C2DVector& stick)
{
    float x = (float)fabs(stick.X) - 0.25f;
    if (x < 0.0f)
        x = 0.0f;
    else
        x = x * Sign(stick.X) * (1.0f / 0.75f);
    float y = (float)fabs(stick.Y) - 0.23f;
    if (y < 0.0f)
        y = 0.0f;
    else
        y = Sign(stick.Y) * y * (1.0f / 0.77f);
    stick.X = ApplyResponseCurve(x);
    stick.Y = ApplyResponseCurve(y);
}
}
// Takes the address so the anonymous-namespace function is emitted on its own.
void* g_PreProcessStickVector = (void*)&PreProcessStickVector;