// CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo(float duration, ScreenEffectPriority priority, const CRadialBlurParams& params) -- push a radial-blur entry onto the screen-effect priority stack with a default (world-tracked, origin) blur position
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B84E0, 101 bytes (+ int3 padding).
// Callees: NScreenEffect::CScreenEffectPriorityStack::InsertRadialBlurEntry(float, const CRadialBlurParams*,
//          const CRadialBlurPositionParams*, ScreenEffectPriority) @ 0x00881690.
// Layout (Ego_r.pdb): CEnvironmentSpellEffectInterpolator keeps its CScreenEffectPriorityStack at +0x90
//   (`add ecx,90h`); CRadialBlurPositionParams is 32 bytes (two C3DVectors, two track enums).
// Shape: the position block is a 0x20-byte frame local; its two track words (1 = world position) are stored before the
//   argument pushes and the six zero coordinates after them -- the C3DVector members have their own inline constructors
//   while the track words are plain initialisers of the outer constructor.
struct C3DVector {
    float x, y, z;
    C3DVector() : x(0.0f), y(0.0f), z(0.0f) {}
};

enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION {
    ENVIRONMENT_RADIAL_BLUR_TRACK_NONE = 0,
    ENVIRONMENT_RADIAL_BLUR_TRACK_WORLD_POS = 1,
    ENVIRONMENT_RADIAL_BLUR_TRACK_SCREEN_POS = 2
};

enum ScreenEffectPriority { SCREEN_EFFECT_PRIORITY_DEFAULT = 0 };

struct CRadialBlurParams;

struct CRadialBlurPositionParams {
    C3DVector AreaCenter;                                     // +0x00
    C3DVector BlurCenter;                                     // +0x0c
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos;      // +0x18
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION BlurTrackPos;      // +0x1c
    CRadialBlurPositionParams()
        : AreaTrackPos(ENVIRONMENT_RADIAL_BLUR_TRACK_WORLD_POS), BlurTrackPos(ENVIRONMENT_RADIAL_BLUR_TRACK_WORLD_POS) {}
};

struct CScreenEffectPriorityStack {
    void InsertRadialBlurEntry(float duration, const CRadialBlurParams* params,
                               const CRadialBlurPositionParams* position, ScreenEffectPriority priority);   // 0x00881690
};

struct CEnvironment;
struct CEnvironmentSpellEffectInterface;

struct CEnvironmentSpellEffectInterpolator {
    CEnvironment*                     Environment;           // +0x00
    CEnvironmentSpellEffectInterface* SpellInterface;        // +0x04
    unsigned char                     _pad8[0x90 - 0x8];     // radial-blur fade and theme-transition state
    CScreenEffectPriorityStack        ScreenEffectPriorityStack;   // +0x90
    void RadialBlurFadeTo(float duration, ScreenEffectPriority priority, const CRadialBlurParams& params);
};

void CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo(float duration, ScreenEffectPriority priority, const CRadialBlurParams& params)
{
    CRadialBlurPositionParams position;
    ScreenEffectPriorityStack.InsertRadialBlurEntry(duration, &params, &position, priority);
}
