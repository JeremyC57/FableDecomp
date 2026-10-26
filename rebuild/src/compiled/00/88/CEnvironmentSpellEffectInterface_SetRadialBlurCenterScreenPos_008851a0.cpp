// CEnvironmentSpellEffectInterface::SetRadialBlurCenterScreenPos(const C2DVector& position, bool interpolate) -- make the blur centre track a screen position (stored as a C3DVector with z = 0; current frame, and previous frame too when not interpolating)
// Leaf: SetRadialBlurCenterScreenPos
// Retail: Fable.exe (TLC, VC7.1) @ 0x008851A0, 101 bytes (+ int3 padding).
// Label correction: the manifest's "SetRadialBlurCenterAreaScreenPos" is shared with 0x00885210; this body writes the BLUR
//   centre (+0x40 / +0xc0, track words +0x50 / +0xd0), 0x00885210 the AREA centre (+0x34 / +0xb4, +0x4c / +0xcc).
// Callees: none.
// Layout (Ego_r.pdb, matches retail): CRadialBlurPositionParams { C3DVector AreaCenter +0; C3DVector BlurCenter +0xc;
//   AreaTrackPos +0x18; BlurTrackPos +0x1c } at +0x34 (current) and +0xb4 (previous frame). Track value 2 = screen position.
// Shape: the 3D point is a named C3DVector local built by a (x, y, z) constructor in the 12-byte frame -- the zero z is
//   stored to the frame and read back (`mov [esp+14h],0 ; mov esi,[esp+14h]`) -- then copied into each centre.
class C2DVector { public: float x, y; };

class C3DVector { public:
    float x, y, z;
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

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

class CEnvironment;

class CEnvironmentSpellEffectInterface { public:
    CEnvironment*             Environment;                    // +0x00
    unsigned char             _pad4[0x34 - 0x4];              // CRadialBlurParams RadialBlurParams
    CRadialBlurPositionParams RadialBlurPositionParams;       // +0x34
    unsigned char             _pad54[0xb4 - 0x54];            // displacement, screen filter, previous radial blur params
    CRadialBlurPositionParams PrevFrameRadialBlurPositionParams; // +0xb4
    void SetRadialBlurCenterScreenPos(const C2DVector& position, bool interpolate);
};

void CEnvironmentSpellEffectInterface::SetRadialBlurCenterScreenPos(const C2DVector& position, bool interpolate)
{
    C3DVector centre(position.x, position.y, 0.0f);
    RadialBlurPositionParams.BlurCenter = centre;
    RadialBlurPositionParams.BlurTrackPos = ENVIRONMENT_RADIAL_BLUR_TRACK_SCREEN_POS;
    if (!interpolate) {
        PrevFrameRadialBlurPositionParams.BlurCenter = centre;
        PrevFrameRadialBlurPositionParams.BlurTrackPos = ENVIRONMENT_RADIAL_BLUR_TRACK_SCREEN_POS;
    }
}