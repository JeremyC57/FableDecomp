// _global::WrapAngle(float angle) -- wrap an angle once into [-pi, pi): subtract 2*pi at or above pi, add 2*pi below -pi, otherwise return it unchanged
// Leaf: WrapAngle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0B4C0, 63 bytes (`ret 4`, ecx unused). Descriptive name: the manifest's
//   "FloatThreshold_Clamp" is propagated. The only caller is the animated value's SetTarget 0x00C0B570 (0x00C0B5A4,
//   `mov ecx,[esp+74h] ; push ecx ; call` -- no `this`), so it is a static/free callee-pops routine.
// Constants: pi @ 0x0128F604, 2*pi @ 0x0128F608, -pi @ 0x012B1EB8.
// Shape: each early return reloads the argument (`fcomp` ; `fld [esp+4]`).
static const float kPi = 3.14159265f;           // 0x0128F604
static const float kTwoPi = 6.28318531f;        // 0x0128F608
static const float kMinusPi = -3.14159265f;     // 0x012B1EB8

float __stdcall WrapAngle(float angle)
{
    if (angle >= kPi)
        return angle - kTwoPi;
    if (angle < kMinusPi)
        return angle + kTwoPi;
    return angle;
}