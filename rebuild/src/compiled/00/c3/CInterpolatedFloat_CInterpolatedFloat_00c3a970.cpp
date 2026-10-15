// CInterpolatedFloat::CInterpolatedFloat(float target, float time) -- constructor of a smoothed scalar (optionally angle-wrapped): not an angle, reset to 0, then aim at `target` over `time`
// Leaf: ??0CInterpolatedFloat
// Retail: Fable.exe @ 0x00C3A970, 37 bytes (`ret 8`, returns `this`).
// Callees: CInterpolatedFloat::Reset(float) 0x00C0B510 (manifest "Matrix_SetIdentityRotation" is
//          propagated: it wraps the value into [-pi, pi] when the +0x30 angle flag is set, then
//          writes it to the current/target words +0x00/+0x04/+0x1C and zeroes the rates),
//          CInterpolatedFloat::SetTarget(float, float) 0x00C3A950 (forwards (target, 0, time) to
//          0x00C0B570).
// Label: the manifest's "CTCTavernGameSpotTheAddition::QuitMidGame" is propagated. The only caller is
// the constructor at 0x00C39130 (manifest "CTCTavernGameSpotTheAddition_Constructor") building this
// object as its member at +0x138 with (0, 0); the class name is descriptive.
// Shape: the init-list flag store sits between the hoisted `push 0` (Reset's 0.0f argument) and the
//        call; both float parameters are forwarded as dwords.
struct CInterpolatedFloat {
    float m_current;                    // +0x00
    float m_target;                     // +0x04
    unsigned char _pad_0x08[0x28];      // +0x08 rates / previous values
    bool m_isAngle;                     // +0x30 wrap into [-pi, pi]
    CInterpolatedFloat(float target, float time);
    void Reset(float value);                    // 0x00C0B510
    void SetTarget(float target, float time);   // 0x00C3A950
};

CInterpolatedFloat::CInterpolatedFloat(float target, float time)
    : m_isAngle(false)
{
    Reset(0.0f);
    SetTarget(target, time);
}