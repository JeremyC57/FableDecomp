// CEnvironmentSpellEffectInterpolator::RadialBlurSetCenterWorldPos(void* position, void* a) -- set the radial-blur priority stack's area centre (both variants) and its area track position (both variants, with 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B8310, length 61 bytes.
// Callees: CScreenEffectPriorityStack::SetAreaCenter @ 0x00881010 and @ 0x008810C0 (position, a), SetAreaTrackPos @ 0x00881150 and @ 0x008811D0 (position, 1), all on
// the stack at +0x90. Shape: the two arguments live in edi / ebx, the stack address in esi; four `push ; push ; mov ecx,esi ; call` groups; `ret 8`.
struct CScreenEffectPriorityStack {
    void SetAreaCenter(void* position, void* a);         // 0x00881010
    void SetAreaCenterB(void* position, void* a);        // 0x008810C0
    void SetAreaTrackPos(void* position, long flag);     // 0x00881150
    void SetAreaTrackPosB(void* position, long flag);    // 0x008811D0
};

struct CEnvironmentSpellEffectInterpolator {
    unsigned char              _pad[0x90];
    CScreenEffectPriorityStack m_radialBlurStack;  // +0x90
    void RadialBlurSetCenterWorldPos(void* position, void* a);
};

void CEnvironmentSpellEffectInterpolator::RadialBlurSetCenterWorldPos(void* position, void* a)
{
    m_radialBlurStack.SetAreaCenter(position, a);
    m_radialBlurStack.SetAreaCenterB(position, a);
    m_radialBlurStack.SetAreaTrackPos(position, 1);
    m_radialBlurStack.SetAreaTrackPosB(position, 1);
}