// Behaviour test for 0x0080C190 (source body copied from src/0080c190.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
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
        if (radius < def.ExplosionRadiusSmall)
            radius = def.ExplosionRadiusSmall;
        else if (radius > def.ExplosionRadiusLarge)
            radius = def.ExplosionRadiusLarge;
        result = Clamp((radius - def.ExplosionRadiusSmall) / (def.ExplosionRadiusLarge - def.ExplosionRadiusSmall), 0.0f, 1.0f);
    }
    return result;
}
}
float (__fastcall* g_keep_0080c190)(const CExplosionDef&, float) = &GetExplosionEffectInterpolate;

// ---- test harness ----
extern const float g_epsilon_0129ba3c = 1e-4f;
static int Near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 0.0001f; }
int main()
{
    int ok = 1;
    static CExplosionDef def;
    def.ExplosionRadiusSmall = 2.0f;
    def.ExplosionRadiusLarge = 6.0f;
    if (!Near(g_keep_0080c190(def, 4.0f), 0.5f)) ok = 0;     // inside the band
    if (!Near(g_keep_0080c190(def, 1.0f), 0.0f)) ok = 0;     // below small
    if (!Near(g_keep_0080c190(def, 10.0f), 1.0f)) ok = 0;    // above large
    if (!Near(g_keep_0080c190(def, 5.0f), 0.75f)) ok = 0;
    def.ExplosionRadiusLarge = 2.00001f;                      // empty band
    if (!Near(g_keep_0080c190(def, 4.0f), 0.0f)) ok = 0;
    printf("0080c190_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}