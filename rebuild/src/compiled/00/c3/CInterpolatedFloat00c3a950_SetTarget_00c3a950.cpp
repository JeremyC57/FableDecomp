// CInterpolatedFloat_00c3a950::SetTarget(float target, float time) -- aim a smoothed scalar at a new target over `time`: forwards (target, 0.0f, time) to the full three-argument SetTarget (target, arrival rate, time)
// Leaf: ?SetTarget@CInterpolatedFloat_00c3a950@@QAEXMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3A950, 20 bytes (+ int3 padding), __fastcall `ret 8`.
// Library: libpk audio (no PDB/RTTI); descriptive names. `this` is the 0x34-byte ramped value of the landed
//   0x00C3A970 CInterpolatedFloat constructor (which calls this with (target, time)) and of 0x00C0B510 SetImmediate.
//   Callers: 0x00C2D244, 0x00C2D6C7, 0x00C38CE0, 0x00C38CF1, 0x00C3A98A. The manifest's "ConstructString" is propagated.
// Callee: SetTarget(float target, float rate, float time) 0x00C0B570 (`ret 0Ch`; a non-positive time falls back to
//   SetImmediate).
// Shape: `this` stays in ecx (no adjustment); 0.0f is pushed as the integer immediate 0, the floats as dwords.
struct CInterpolatedFloat_00c3a950 {
    float m_current;                    // +0x00
    float m_target;                     // +0x04
    unsigned char _pad_0x08[0x28];      // +0x08 rates / time / duration / coefficients
    bool m_isAngle;                     // +0x30 wrap into [-pi, pi)
    void SetTarget(float target, float time);
    void SetTarget(float target, float rate, float time);   // 0x00C0B570
};

void CInterpolatedFloat_00c3a950::SetTarget(float target, float time)
{
    SetTarget(target, 0.0f, time);
}