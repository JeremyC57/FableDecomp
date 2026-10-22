// CSwappingStateComponent::Initialise() -- run the CChangingStateComponent base Initialise, then seed the swap state from the component's current value at +0x30 and set the swap mode word to 13
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00547360, length 29 bytes.
// Callees: CChangingStateComponent::Initialise @ 0x0052C730 (zeroes its own +0x134..+0x158 state and calls its base).
// Shape: `push esi ; mov esi,ecx` keeps `this` for the two stores after the base call; the +0x30 word is read
// after the call (a plain member read in source order), the 13 is an immediate store. The PDB layout of
// CChangingStateComponent is the NUISystem one (0x18c bytes, its lists are wider than retail's), so the
// retail offsets are modelled with a descriptive local layout.
struct CChangingStateComponent {
    unsigned char _pad00[0x30];
    long          m_initialValue;                  // +0x30 (the value the swap state starts from)
    unsigned char _pad34[0x168 - 0x34];
    void Initialise();                             // 0x0052C730
};

struct CSwappingStateComponent : CChangingStateComponent {
    long m_swapValue;                              // +0x168
    long m_swapMode;                               // +0x16c
    void Initialise();
};

void CSwappingStateComponent::Initialise()
{
    CChangingStateComponent::Initialise();
    m_swapValue = m_initialValue;
    m_swapMode = 13;
}