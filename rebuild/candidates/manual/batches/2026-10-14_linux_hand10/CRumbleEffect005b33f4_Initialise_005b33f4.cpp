// CRumbleEffect_005b33f4::Initialise(a, b, c, d, e, f, g) -- zero two floats, forward the seven arguments to the base Initialise, return *this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B33F4, length 49 bytes.
// Callees: the base Initialise @ 0x0070C050 (seven dword arguments, `ret 1Ch`; the manifest labels
// it the CInitRumble constructor, a propagated name).
// Shape: a frame-pointer body that re-pushes its arguments straight from the caller's slots
// (`push dword ptr [ebp+N]`, no spills) with `fldz ; fstp` for the two zeroed floats at +0x18 and
// +0x1c BEFORE the base call: that order rules out a constructor (base/member initialisers run
// first), so this is an Initialise that returns a reference to itself (`mov eax,esi`). The settings
// are size with frame-pointer omission off: `#pragma optimize("y", off)` then
// `#pragma optimize("s", on)` (the "gsy" form spills `this` to the frame instead of keeping esi).
// The manifest's "CAIStateGroup_WhisperRetreat" label is propagated; the class is named by its body.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CRumbleEffectBase {
    unsigned char _pad[0x18];                   // +0x00..+0x17 (the base's own state)
    CRumbleEffectBase& Initialise(long a, long b, long c, long d, long e, long f, long g);   // 0x0070C050
};

struct CRumbleEffect_005b33f4 : CRumbleEffectBase {
    float m_elapsed;                            // +0x18
    float m_intensity;                          // +0x1c
    CRumbleEffect_005b33f4& Initialise(long a, long b, long c, long d, long e, long f, long g);
};

CRumbleEffect_005b33f4& CRumbleEffect_005b33f4::Initialise(long a, long b, long c, long d, long e, long f, long g)
{
    m_elapsed = 0.0f;
    m_intensity = 0.0f;
    CRumbleEffectBase::Initialise(a, b, c, d, e, f, g);
    return *this;
}
