// CDistanceFalloff_00c1bda0::GetAttenuation(float distance) -- distance attenuation between the near (+0) and far (+4) radii: 0 at or beyond far, 1 at or inside near, else ((far - distance) / (far - near)) raised to the module's falloff exponent
// Leaf: GetAttenuation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BDA0, 89 bytes (+ int3 padding), __fastcall `ret 4`, float in st(0).
// Callers: 0x00C3449F, 0x00C344AC, 0x00C458B5 (the libpk 3D voice code). Globals: the exponent @ 0x0139C974 (set by
//   0x00C1BD80 = pow(K, -x)); constants 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8. Callee: MSVCR71 _CIpow (0x00BFEB40, the
//   pow intrinsic).
// Shape: the span is a frame float (`push ecx` word), the remaining distance is a new local homed in the dead
//   argument slot (`fst [esp+8]`); `<= 0` compiles to `test ah,41h ; jp` and `>= span` to `test ah,1 ; jne`, each
//   early return loading its constant; the last return is the float pow overload (`(float)pow(double, double)`).
// Label correction: the catalog's `Render_CalculateAlphaFalloff` is descriptive; names descriptive.
extern "C" double __cdecl pow(double x, double y);
#pragma intrinsic(pow)

extern float g_falloffExponent;            // 0x0139C974

struct CDistanceFalloff_00c1bda0 {
    float m_near;                          // +0x00
    float m_far;                           // +0x04
    float GetAttenuation(float distance);
};

float CDistanceFalloff_00c1bda0::GetAttenuation(float distance)
{
    float span = m_far - m_near;
    float remaining = m_far - distance;
    if (remaining <= 0.0f)
        return 0.0f;
    if (remaining >= span)
        return 1.0f;
    return (float)pow(remaining / span, g_falloffExponent);
}