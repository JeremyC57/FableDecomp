#ifndef FABLE_ENGINE_CENVIRONMENTSPELLEFFECTINTERPOLATOR_H
#define FABLE_ENGINE_CENVIRONMENTSPELLEFFECTINTERPOLATOR_H
// CEnvironmentSpellEffectInterpolator (new, chunk 765): no RTTI (not polymorphic). Ego_r.pdb members (struct_layouts_egor.tsv, sizeof
// 0x1B0): Environment +0x00, SpellInterface +0x04, TargetRadialBlurParams / FadeStartRadialBlurParams (CRadialBlurParams, 0x30 each)
// +0x08 / +0x38, RadialBlurFadeDuration +0x68, RadialBlurFadeStartTime +0x70, RadialBlurFadeActive +0x78, ThemeTransitionActive +0x79,
// ThemeTransitionStartTime +0x80, ThemeTransitionDuration +0x88, ThemeTransitionTargetIndex +0x8C, ScreenEffectPriorityStack +0x90
// (retail ScreenFilterSetFlags 0x006B83E0 forwards `add ecx,90h`, so the prefix matches retail). The CRadialBlurParams members are
// opaque; the priority stack is member-less (header debt: sizeof not modelled).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"
class CEnvironment;
class CEnvironmentSpellEffectInterface;
class CEnvironmentSpellEffectInterpolator {
public:
    void ScreenFilterSetFlags(const NScreenEffect::CScreenEffectPriorityStack::ThingGraphicArgPassers& things, void* handle);  // 0x006B83E0
private:
    CEnvironment* Environment;                                           // +0x00
    CEnvironmentSpellEffectInterface* SpellInterface;                    // +0x04
    unsigned char TargetRadialBlurParams[0x30];                          // +0x08 CEnvironmentSpellEffectInterface::CRadialBlurParams
    unsigned char FadeStartRadialBlurParams[0x30];                       // +0x38 CEnvironmentSpellEffectInterface::CRadialBlurParams
    float RadialBlurFadeDuration;                                        // +0x68
    double RadialBlurFadeStartTime;                                      // +0x70
    bool RadialBlurFadeActive;                                           // +0x78
    bool ThemeTransitionActive;                                          // +0x79
    double ThemeTransitionStartTime;                                     // +0x80
    float ThemeTransitionDuration;                                       // +0x88
    long ThemeTransitionTargetIndex;                                     // +0x8c
    NScreenEffect::CScreenEffectPriorityStack ScreenEffectPriorityStack; // +0x90
    friend struct CEnvironmentSpellEffectInterpolatorLayout;
};
struct CEnvironmentSpellEffectInterpolatorLayout {
    enum { StackOffset = offsetof(CEnvironmentSpellEffectInterpolator, ScreenEffectPriorityStack) };
};
FABLE_STATIC_ASSERT(CEnvironmentSpellEffectInterpolatorLayout::StackOffset == 0x90);
#endif
