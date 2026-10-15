// CEnvironmentSpellEffectInterface::SetRadialBlurCenterWorldPos(const C3DVector& position, bool interpolate) -- make both the blur centre and the area centre track a world position (current frame, and previous frame too when not interpolating)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00885110, 138 bytes (+ int3 padding).
// Callees: none.
// Layout (Ego_r.pdb, matches retail): CRadialBlurPositionParams { C3DVector AreaCenter +0; C3DVector BlurCenter +0xc;
//   ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos +0x18, BlurTrackPos +0x1c } sits at +0x34 (current) and +0xb4
//   (PrevFrameRadialBlurPositionParams) of CEnvironmentSpellEffectInterface. Track value 1 = world position
//   (the screen-position setters 0x008851A0 / 0x00885210 store 2).
// Shape: the blur centre is set first, then the area centre; each C3DVector copy re-reads the argument through a scratch
//   register; the constant 1 lives in esi for all four track stores.
struct C3DVector { float x, y, z; };

enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION {
    ENVIRONMENT_RADIAL_BLUR_TRACK_NONE = 0,
    ENVIRONMENT_RADIAL_BLUR_TRACK_WORLD_POS = 1,
    ENVIRONMENT_RADIAL_BLUR_TRACK_SCREEN_POS = 2
};

struct CRadialBlurPositionParams {
    C3DVector AreaCenter;                                     // +0x00
    C3DVector BlurCenter;                                     // +0x0c
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos;      // +0x18
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION BlurTrackPos;      // +0x1c
};

struct CEnvironment;

struct CEnvironmentSpellEffectInterface {
    CEnvironment*             Environment;                    // +0x00
    unsigned char             _pad4[0x34 - 0x4];              // CRadialBlurParams RadialBlurParams
    CRadialBlurPositionParams RadialBlurPositionParams;       // +0x34
    unsigned char             _pad54[0xb4 - 0x54];            // displacement, screen filter, previous radial blur params
    CRadialBlurPositionParams PrevFrameRadialBlurPositionParams; // +0xb4
    void SetRadialBlurCenterWorldPos(const C3DVector& position, bool interpolate);
};

void CEnvironmentSpellEffectInterface::SetRadialBlurCenterWorldPos(const C3DVector& position, bool interpolate)
{
    RadialBlurPositionParams.BlurCenter = position;
    RadialBlurPositionParams.BlurTrackPos = ENVIRONMENT_RADIAL_BLUR_TRACK_WORLD_POS;
    if (!interpolate) {
        PrevFrameRadialBlurPositionParams.BlurCenter = position;
        PrevFrameRadialBlurPositionParams.BlurTrackPos = ENVIRONMENT_RADIAL_BLUR_TRACK_WORLD_POS;
    }
    RadialBlurPositionParams.AreaCenter = position;
    RadialBlurPositionParams.AreaTrackPos = ENVIRONMENT_RADIAL_BLUR_TRACK_WORLD_POS;
    if (!interpolate) {
        PrevFrameRadialBlurPositionParams.AreaCenter = position;
        PrevFrameRadialBlurPositionParams.AreaTrackPos = ENVIRONMENT_RADIAL_BLUR_TRACK_WORLD_POS;
    }
}
