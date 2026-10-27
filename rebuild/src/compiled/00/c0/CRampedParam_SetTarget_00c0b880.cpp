// CRampedParam::SetTarget(float target, float time) -- aim a ramped audio parameter at a new target over time: forwards (target, 0.0f, time) to the full three-argument SetTarget (target, arrival rate, time)
// Leaf: ?SetTarget@CRampedParam@@QAEXMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0B880, 20 bytes, thiscall, `ret 8`.
// Identity (probable): byte twin of the landed CInterpolatedFloat_00c3a950::SetTarget (0x00C3A950) and calls the same
//   SetTarget(float target, float rate, float time) 0x00C0B570 of the 0x34-byte ramped parameter (the landed
//   CRampedParam::SetImmediate 0x00C0B510 model); the two copies are the same inline emitted in two libpk TUs.
// Shape: this stays in ecx; 0.0f is pushed as the integer immediate 0, the floats as dwords.
struct CRampedParam {                           // 0x34 bytes (landed SetImmediate 0x00C0B510 layout)
    float m_value;                              // +0x00 current value
    float m_target;                             // +0x04 target value
    unsigned char _pad08[0x28];                 // +0x08 rates / time / duration / coefficients
    bool  m_bAngle;                             // +0x30 wrap the value into [-pi, pi)
    void SetTarget(float target, float time);
    void SetTarget(float target, float rate, float time);   // 0x00C0B570
};

void CRampedParam::SetTarget(float target, float time)
{
    SetTarget(target, 0.0f, time);
}