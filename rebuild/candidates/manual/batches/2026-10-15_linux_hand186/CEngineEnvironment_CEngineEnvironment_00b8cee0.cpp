// CEngineEnvironment::CEngineEnvironment() -- engine environment (wind) state: base ctor, own vtable, the three wind vectors (1,0,0), zero speeds/frame/flags, then both 64-entry wind-skew histories cleared
// Leaf: ??0CEngineEnvironment
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8CEE0, 224 bytes incl. padding (plain `ret`). The manifest label
//          "CBlendedEnvironmentTheme::CBlendedEnvironmentTheme" is propagated and wrong: vtable 0x012A3C68 is
//          CEngineEnvironment (rtti-vtables.tsv: slot 1 FrameStart 0x00B8C830, slot 7 WorldFrameUpdate 0x00B8CCF0).
// Callees: the 12-byte base constructor @ 0x00B59710 (manifest "CNavigationLayer::CNavigationLayer", propagated).
// Layout (Ego_r.pdb CEngineEnvironment, 0x654; rebuild/include/engine/CEngineEnvironment.h has no vtable/base
//          so a local model is used): +0xC..+0x18 interpolations (not set here), +0x1C WindSkewHistory[64],
//          +0x31C InterpolatedWindSkewHistory[64], +0x61C/+0x628/+0x634 Current/Next/PreviousWindVector,
//          +0x640..+0x648 speeds, +0x64C CurrentRenderFrame, +0x650 EnvironmentalEffectsAlwaysUpdate, +0x651 ShowWindVector.
// Shape: C3DVector has a user ctor and a user copy ctor but no operator=, so each history slot is written through a
//          pointer register (`lea ebx,[eax-300h]`); the two zero temporaries are hoisted to the frame and re-read.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct CEnvironmentBase {                       // 12-byte base: vptr + two words
    virtual void v00();
    long m_base04;                              // +0x04
    long m_base08;                              // +0x08
    CEnvironmentBase();                         // 0x00B59710
};

struct CEngineEnvironment : CEnvironmentBase {
    float     CurrentInterpolation;             // +0x0C
    float     BulletTimeInterpolation;          // +0x10
    float     PausedInterpolation;              // +0x14
    float     NonPausedInterpolation;           // +0x18
    C3DVector WindSkewHistory[64];              // +0x1C
    C3DVector InterpolatedWindSkewHistory[64];  // +0x31C
    C3DVector CurrentWindVector;                // +0x61C
    C3DVector NextWindVector;                   // +0x628
    C3DVector PreviousWindVector;               // +0x634
    float     CurrentWindSpeed;                 // +0x640
    float     NextWindSpeed;                    // +0x644
    float     PreviousWindSpeed;                // +0x648
    long      CurrentRenderFrame;               // +0x64C
    bool      EnvironmentalEffectsAlwaysUpdate; // +0x650
    bool      ShowWindVector;                   // +0x651
    virtual void FrameStart();                  // slot 1, 0x00B8C830
    CEngineEnvironment();
};

CEngineEnvironment::CEngineEnvironment()
    : CurrentWindVector(1.0f, 0.0f, 0.0f), NextWindVector(1.0f, 0.0f, 0.0f), PreviousWindVector(1.0f, 0.0f, 0.0f),
      CurrentWindSpeed(0.0f), NextWindSpeed(0.0f), PreviousWindSpeed(0.0f), CurrentRenderFrame(0),
      EnvironmentalEffectsAlwaysUpdate(false), ShowWindVector(false)
{
    for (long i = 0; i < 64; ++i) {
        WindSkewHistory[i] = C3DVector(0.0f, 0.0f, 0.0f);
        InterpolatedWindSkewHistory[i] = C3DVector(0.0f, 0.0f, 0.0f);
    }
}
