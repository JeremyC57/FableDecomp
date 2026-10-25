// (anonymous)::TransformControllerInputFromRadiallyToRectangularlyLimited(const C2DVector& input) -- map a stick position limited to the unit circle onto the unit square: keep the direction (as a turn angle from +Y, mirrored for negative X), stretch the length by 1 + (sqrt2 - 1) * sin^2(2 * angle) (sqrt2 on the diagonals) and clamp both components to [-1, 1]; inputs shorter than 0.0001 come back unchanged
// Grade: DIFFER(386v382) -- only the length spill and frame: retail keeps the sqrt result on the x87 stack (`fstp st(2) ; fstp st(0) ; fst [esp+10h]`, compare from the register) with a 0xc frame holding an unused slot; ours stores the length to its home and reloads it for the epsilon compare (8-byte frame). Everything after the first compare matches (direct sqrt, GetLength inline and sqrtf wrapper all give the same)
// Leaf: ?TransformControllerInputFromRadiallyToRectangularlyLimited@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A04200, 382 bytes, __fastcall (ecx = hidden C2DVector result, edx = input),
//   plain `ret`. Caller: CInputEvent::GetJoystickPosition() const @ 0x00A043A0 (passes its joystick-position data).
// Identity: Ego_r.pdb `?TransformControllerInputFromRadiallyToRectangularlyLimited@?A0x9ab0773c@@YI?AVC2DVector@@ABV2@@Z`, an
//   anonymous-namespace __fastcall helper of the CInputEvent TU (the chunk label "Math_NormalizeVector2D" is descriptive).
// Callees: _CIacos (MSVCR71 import thunk @ 0x00BFEBA2); sqrt / sin / cos are the x87 intrinsics.
// Globals (float pool): 0.0001f 0x0129BA3C, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0, 1.0f 0x0122DED8, 1/(2pi) 0x01237F08,
//   0.5f 0x0122F59C; doubles 2pi (as float) 0x0124F2B8 and 2.0 0x01244480.
// Shape: the cosine to +Y is the dot with (0, 1) (Y * 1 folds, X * 0 stays) over the length; it and the result X are clamped
//   in memory (if / else-if immediate stores), the result Y on the x87 stack.
typedef unsigned short wchar_t;                     // VC7.1 default /Zc:wchar_t-
extern "C" double __cdecl acos(double x);
extern "C" double __cdecl sqrt(double x);
extern "C" double __cdecl sin(double x);
extern "C" double __cdecl cos(double x);
#pragma intrinsic(acos, sqrt, sin, cos)
inline float sqrtf(float x) { return (float)sqrt((double)x); }   // VC7.1 <math.h> C++ wrapper

struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    float GetLength() const { return sqrtf(X * X + Y * Y); }
    float Dot(const C2DVector& o) const { return X * o.X + Y * o.Y; }
};

namespace {
const double TWO_PI = 6.2831854820251465;           // (double)(2 * 3.1415927f)

C2DVector __fastcall TransformControllerInputFromRadiallyToRectangularlyLimited(const C2DVector& input)
{
    float length = input.GetLength();
    if (length < 0.0001f)
        return input;
    float cosAngle = input.Dot(C2DVector(0.0f, 1.0f)) / length;
    if (cosAngle < -1.0f)
        cosAngle = -1.0f;
    else if (cosAngle > 1.0f)
        cosAngle = 1.0f;
    float angle = (float)acos(cosAngle) * 0.15915494f;
    float offset = input.X < 0.0f ? 0.5f : 0.0f;
    float s = (float)sin(angle * 2.0f * TWO_PI);
    float scale = length * (1.0f + ((float)sqrt(2.0) - 1.0f) * (s * s));
    float x = (float)sin((angle + offset) * TWO_PI) * scale;
    float y = (float)cos(angle * TWO_PI) * scale;
    if (x < -1.0f)
        x = -1.0f;
    else if (x > 1.0f)
        x = 1.0f;
    if (y < -1.0f)
        y = -1.0f;
    else if (y > 1.0f)
        y = 1.0f;
    return C2DVector(x, y);
}
}

C2DVector (__fastcall *g_keepTransformControllerInput)(const C2DVector&) =
    &TransformControllerInputFromRadiallyToRectangularlyLimited;   // keeps the internal-linkage body emitted; not part of the retail function
