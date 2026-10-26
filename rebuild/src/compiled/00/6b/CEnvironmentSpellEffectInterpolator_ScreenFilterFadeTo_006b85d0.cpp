// CEnvironmentSpellEffectInterpolator::ScreenFilterFadeTo(float duration, ScreenEffectPriority priority, float alpha, float saturation, float contrast, float brightness, const CRGBFloatColour& tint, const CThingGraphicArgList* things) -- build a screen-filter parameter block and push it onto the screen-effect priority stack
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B85D0, 102 bytes (+ int3 padding).
// Callees: NScreenEffect::CScreenEffectPriorityStack::InsertScreenFilterEntry(float, const CScreenFilterParams*,
//          const vector<CThingGraphicArgPasser>*, ScreenEffectPriority) @ 0x00881C90 (manifest:
//          "CEnvironmentSpellEffectInterface_AddScreenFilter", untyped).
// Layout (Ego_r.pdb): CScreenFilterParams { Alpha, Saturation, Contrast, Brightness, CRGBFloatColour Tint } = 32 bytes;
//   the interpolator's CScreenEffectPriorityStack sits at +0x90 (`add ecx,90h`).
// Shape: the block is a 0x20-byte frame local filled field by field (the tint as a POD copy, dword by dword) before
//   the argument pushes; the thing list is forwarded as a plain pointer.
class CRGBFloatColour { public: float R, G, B, A; };

struct CScreenFilterParams {
    float           Alpha;                             // +0x00
    float           Saturation;                        // +0x04
    float           Contrast;                          // +0x08
    float           Brightness;                        // +0x0c
    CRGBFloatColour Tint;                              // +0x10
};

enum ScreenEffectPriority { SCREEN_EFFECT_PRIORITY_DEFAULT = 0 };

struct CThingGraphicArgList;                           // vector<CScreenEffectPriorityStack::CThingGraphicArgPasser>

struct CScreenEffectPriorityStack {
    void InsertScreenFilterEntry(float duration, const CScreenFilterParams* params,
                                 const CThingGraphicArgList* things, ScreenEffectPriority priority);   // 0x00881C90
};

class CEnvironment;
class CEnvironmentSpellEffectInterface;

class CEnvironmentSpellEffectInterpolator { public:
    CEnvironment*                     Environment;           // +0x00
    CEnvironmentSpellEffectInterface* SpellInterface;        // +0x04
    unsigned char                     _pad8[0x90 - 0x8];     // radial-blur fade and theme-transition state
    CScreenEffectPriorityStack        ScreenEffectPriorityStack;   // +0x90
    void ScreenFilterFadeTo(float duration, ScreenEffectPriority priority, float alpha, float saturation, float contrast,
                            float brightness, const CRGBFloatColour& tint, const CThingGraphicArgList* things);
};

void CEnvironmentSpellEffectInterpolator::ScreenFilterFadeTo(float duration, ScreenEffectPriority priority, float alpha,
                                                             float saturation, float contrast, float brightness,
                                                             const CRGBFloatColour& tint, const CThingGraphicArgList* things)
{
    CScreenFilterParams params;
    params.Alpha = alpha;
    params.Saturation = saturation;
    params.Contrast = contrast;
    params.Brightness = brightness;
    params.Tint = tint;
    ScreenEffectPriorityStack.InsertScreenFilterEntry(duration, &params, things, priority);
}