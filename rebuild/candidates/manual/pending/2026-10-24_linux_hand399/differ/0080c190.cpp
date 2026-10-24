// Grade: DIFFER(128v122) -- the radius clamp reloads the stack parameter (fld [esp+4] ; fcomp) per compare where retail loads it once and keeps it on the x87 stack (fcom ; fstp st0 ; fld bound)
// _anon::GetExplosionEffectInterpolate(const CExplosionDef& def, float radius) -- explosion effect blend factor: where radius sits between the def's small and large effect radii, clamped to 0..1 (0 when the band is empty)
// Leaf: ?GetExplosionEffectInterpolate@
// Retail: Fable.exe @ 0x0080C190, 122 bytes (+ int3 padding), `ret 4`: __fastcall, the def reference in ecx, the
// float radius on the stack, result on the x87 stack.
// Identity: Ego_r.pdb names `?GetExplosionEffectInterpolate@?A0x41ace303@@YIMABVCExplosionDef@@M@Z` (an anonymous-
// namespace __fastcall free function, float(const CExplosionDef&, float)); the callers 0x0080D174 / 0x0080D490 /
// 0x0080D65F (CTCDExplosion::CreateStaticExplosion / UpdateDynamicExplosion) pass PExplosionDef (+0x38) in ecx.
// Members: Ego_r.pdb CExplosionDef ExplosionRadiusSmall +0x50, ExplosionRadiusLarge +0x54 (the def body above +0x28
// is the CDefClassBase part, modelled as padding).
// Label: catalogued `Explosion_CalcFalloffIntensity` (descriptive).
// Globals: 0.0f @ 0x0122DEDC and 1.0f @ 0x0122DED8 (compiler literals), epsilon 1e-4f @ 0x0129BA3C (extern variable).
// Shape: `if (large - small > eps) { clamp radius to [small, large] (if / else if); clamp (r - small) / (large - small)
// to [0, 1]; } else 0` -- the 0.0f result is preloaded before the band test.
extern const float g_epsilon_0129ba3c;            // 0x0129BA3C (1e-4f)
class CExplosionDef {
public:
    char  _base[0x50];                            // +0x00 CDefClassBase part and the fields up to +0x4C
    float ExplosionRadiusSmall;                   // +0x50
    float ExplosionRadiusLarge;                   // +0x54
};
template <class T>
inline T Clamp(const T& v, const T& lo, const T& hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}
namespace {
float __fastcall GetExplosionEffectInterpolate(const CExplosionDef& def, float radius)
{
    float result = 0.0f;
    if (def.ExplosionRadiusLarge - def.ExplosionRadiusSmall > g_epsilon_0129ba3c) {
        float r = Clamp(radius, def.ExplosionRadiusSmall, def.ExplosionRadiusLarge);
        result = Clamp((r - def.ExplosionRadiusSmall) / (def.ExplosionRadiusLarge - def.ExplosionRadiusSmall), 0.0f, 1.0f);
    }
    return result;
}
}
float (__fastcall* g_keep_0080c190)(const CExplosionDef&, float) = &GetExplosionEffectInterpolate;
