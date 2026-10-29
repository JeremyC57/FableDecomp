// Behaviour test for CTCAssassinRush::~CTCAssassinRush @ 0x007A9D60: a running radial blur is faded out over the def's
// end time through world -> environment -> spell-effect interpolator and the key cleared; no key, no call.
#include <stdio.h>
static int g_fades = 0; static float g_seconds = 0; static void* g_key = 0;
class CEnvironmentSpellEffectInterpolator { public: void RadialBlurFadeOut(float seconds, void* key) { g_fades++; g_seconds = seconds; g_key = key; } };
class CEnvironment { public: CEnvironmentSpellEffectInterpolator* SpellEffectInterpolator;
    CEnvironmentSpellEffectInterpolator& GetSpellEffectInterpolator() { return *SpellEffectInterpolator; } };
class CWorld { public: CEnvironment* PEnvironment; CEnvironment& GetEnvironment() { return *PEnvironment; } };
static CWorld* g_world = 0;
struct CDef { float AssassinEndSeconds; };
struct CDefPointer { const CDef* Object; const CDef* operator->() const { return Object; } };
class CTCBase { public: CWorld& GetWorld() const { return *g_world; } virtual ~CTCBase() { } };
class CTCAssassinRush : public CTCBase { public: CDefPointer PDef; void* RadialBlurFadeKey; void** Seen;
    ~CTCAssassinRush() {
        if (RadialBlurFadeKey) {
            GetWorld().GetEnvironment().GetSpellEffectInterpolator().RadialBlurFadeOut(PDef->AssassinEndSeconds, RadialBlurFadeKey);
            RadialBlurFadeKey = 0;
        }
        *Seen = RadialBlurFadeKey;
    } };
int main()
{
    CEnvironmentSpellEffectInterpolator interp; CEnvironment env = { &interp }; CWorld world = { &env }; g_world = &world;
    CDef def = { 1.5f }; static int keyObject; void* seen = &keyObject;
    CTCAssassinRush* r = new CTCAssassinRush; r->PDef.Object = &def; r->RadialBlurFadeKey = &keyObject; r->Seen = &seen;
    delete r;
    if (g_fades != 1 || g_seconds != 1.5f || g_key != &keyObject || seen != 0) { printf("007a9d60_TEST FAIL\n"); return 1; }
    r = new CTCAssassinRush; r->PDef.Object = &def; r->RadialBlurFadeKey = 0; r->Seen = &seen; delete r;
    if (g_fades != 1) { printf("007a9d60_TEST FAIL\n"); return 1; }
    printf("007a9d60_TEST PASS\n");
    return 0;
}