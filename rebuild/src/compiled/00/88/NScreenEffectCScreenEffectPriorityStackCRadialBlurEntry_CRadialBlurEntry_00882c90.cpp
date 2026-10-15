// NScreenEffect::CScreenEffectPriorityStack::CRadialBlurEntry::CRadialBlurEntry(const CRadialBlurEntry& other) -- copy constructor: stack-entry base, blur parameters, position parameters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00882C90, length 104 bytes. No callees.
// Vtables: base CStackEntry 0x01277FF0, CRadialBlurEntry 0x01278050 (rtti-vtables.tsv, both stored here).
// Layout (Ego_r.pdb): CStackEntry {vptr, long curdurationcount +4, long durationcount +8, bool fadingout +0xc}
// (16 bytes); CRadialBlurEntry adds CRadialBlurParams param +0x10 (two CRGBFloatColour + four floats, 48 bytes)
// and CRadialBlurPositionParams posparam +0x40 (two C3DVector + two track-position enums, 32 bytes).
// Shape: the base is copied by its implicit copy constructor (base vtable store, two dwords and the byte), the
// derived vtable store follows; the all-POD param copies implicitly as `rep movsd` x12, posparam memberwise
// (C3DVector has a user copy constructor, so it is copied dword by dword).
// Leaf: ??0CRadialBlurEntry
struct C3DVector {
    float x, y, z;
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
};

struct CRGBFloatColour {
    float r, g, b, a;
};

namespace NScreenEffect {

struct CRadialBlurParams {
    CRGBFloatColour ThresholdColour;                    // +0x00
    CRGBFloatColour IntensityColour;                    // +0x10
    float Length;                                       // +0x20
    float Fade;                                         // +0x24
    float Range;                                        // +0x28
    float Twist;                                        // +0x2c
};

struct CRadialBlurPositionParams {
    C3DVector AreaCenter;                               // +0x00
    C3DVector BlurCenter;                               // +0x0c
    long      AreaTrackPos;                             // +0x18 ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION
    long      BlurTrackPos;                             // +0x1c ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION
};

struct CScreenEffectPriorityStack {
    struct CStackEntry {
        virtual void v00();                             // vtable 0x01277FF0
        long curdurationcount;                          // +0x04
        long durationcount;                             // +0x08
        bool fadingout;                                 // +0x0c
    };
    struct CRadialBlurEntry : CStackEntry {
        virtual void v00();                             // vtable 0x01278050
        CRadialBlurParams         param;                // +0x10
        CRadialBlurPositionParams posparam;             // +0x40
        CRadialBlurEntry(const CRadialBlurEntry& other);
    };
};

CScreenEffectPriorityStack::CRadialBlurEntry::CRadialBlurEntry(const CRadialBlurEntry& other)
    : CStackEntry(other), param(other.param), posparam(other.posparam)
{
}

} // namespace NScreenEffect