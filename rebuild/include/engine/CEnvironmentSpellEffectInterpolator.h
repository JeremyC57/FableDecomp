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
#ifdef FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADETO
class CRGBFloatColour;
#endif
class CEnvironmentSpellEffectInterpolator {
public:
#ifdef FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FRAMEUPDATE
    // Opt-in (row 0x006B3FF0 CEnvironment::Update): Ego_r.pdb ?FrameUpdate@CEnvironmentSpellEffectInterpolator@@QAEXXZ, retail 0x006B8640
    // (landed). Declaration only.
    void FrameUpdate();                                                  // 0x006B8640
#endif
    void ScreenFilterSetFlags(const NScreenEffect::CScreenEffectPriorityStack::ThingGraphicArgPassers& things, void* handle);  // 0x006B83E0
#ifdef FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADETO
    // Opt-in (chunk 1010, row 0x008A9880 CGameScriptInterface::ScreenFilterFadeTo): Ego_r.pdb
    // ?ScreenFilterFadeTo@CEnvironmentSpellEffectInterpolator@@QAEPAXMW4ScreenEffectPriority@NScreenEffect@@MMMMABVCRGBFloatColour@@ABV?$vector@
    // UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@V?$allocator@...@std@@@std@@@Z = retail 0x006B85D0 (102 bytes;
    // the script interface pushes the eight arguments and `push 1` for the priority). Needs FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER
    // (the enum). Declaration only.
    void* ScreenFilterFadeTo(float time, NScreenEffect::ScreenEffectPriority priority, float alpha, float saturation, float contrast,
                             float brightness, const CRGBFloatColour& tint,
                             const NScreenEffect::CScreenEffectPriorityStack::ThingGraphicArgPassers& things);   // 0x006B85D0
#endif
#ifdef FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADEOUT
    // Opt-in (chunk 898, row 0x0079CF30 CTCBulletTime::Pause): Ego_r.pdb / FableWin.pdb
    // ?ScreenFilterFadeOut@CEnvironmentSpellEffectInterpolator@@QAEXMPAX@Z = retail 0x006B83F0 (the PDB-named forwarder
    // CGameScriptInterface::ScreenFilterFadeOut 0x00890200 tail-jumps there with the environment's +0x1964 interpolator and its own
    // (float, void*) arguments; the catalog's `_Cons_val<...>` label is a propagated one). Declaration only.
    void ScreenFilterFadeOut(float time, void* handle);                  // 0x006B83F0
#endif
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
