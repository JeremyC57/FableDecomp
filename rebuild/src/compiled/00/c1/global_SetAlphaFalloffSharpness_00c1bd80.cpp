// SetAlphaFalloffSharpness(float sharpness) -- set the global fall-off exponent to 2^-sharpness
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1BD80, 26 bytes, `ret 4` (__stdcall: both callers push one float and never set ecx).
// Leaf: ?SetAlphaFalloffSharpness@@YGXM@Z
// Name: descriptive (no PDB name found). The catalog label "NCreatureAI::dynamic_initializer_for_COS_OF_THE_ANGLE_NEEDED_TO_FACE_GUARD"
//       is propagated and wrong: this is a plain setter, called from 0x00C09588 and 0x00C10E06. The only reader of its global
//       0x0139C974 (initially 1.0f) is the neighbour 0x00C1BDA0 (catalog "Render_CalculateAlphaFalloff"), which returns
//       pow(fraction, exponent).
// Callees: the CRT x87 pow intrinsic _CIpow @ 0x00BFEB40.
// Shape: `pow(2.0, -x)`: the double literal 2.0 (0x01244480) loaded first, the argument negated with `fchs`, the result
//        narrowed by the float store.
extern float g_alphaFalloffExponent;                 // 0x0139C974
extern "C" double __cdecl pow(double x, double y);
#pragma intrinsic(pow)
void __stdcall SetAlphaFalloffSharpness(float sharpness)
{
    g_alphaFalloffExponent = (float)pow(2.0, -sharpness);
}