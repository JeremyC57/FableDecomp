// Behaviour test for 0x006B82E0 CEnvironmentSpellEffectInterpolator::RadialBlurFadeOut: the (handle, time) pair reaches the priority
// stack at +0x90's DeleteRadialBlurEntry (stubbed here; the row object is linked in).
#include <cstdio>
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFADEOUT
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_DELETERADIALBLURENTRY
#include "engine/CEnvironmentSpellEffectInterpolator.h"

static void* g_this;
static void* g_handle;
static float g_time;

void NScreenEffect::CScreenEffectPriorityStack::DeleteRadialBlurEntry(void* handle, float fadeTime)
{
    g_this = this;
    g_handle = handle;
    g_time = fadeTime;
}

int main()
{
    static double storage[0x200];
    CEnvironmentSpellEffectInterpolator* interpolator = reinterpret_cast<CEnvironmentSpellEffectInterpolator*>(storage);
    interpolator->RadialBlurFadeOut(1.5f, (void*)0x22222222);
    bool ok = g_this == (char*)storage + 0x90 && g_handle == (void*)0x22222222 && g_time == 1.5f;
    if (ok) printf("006b82e0_TEST PASS\n");
    else printf("FAIL this=%p handle=%p\n", g_this, g_handle);
    return ok ? 0 : 1;
}
