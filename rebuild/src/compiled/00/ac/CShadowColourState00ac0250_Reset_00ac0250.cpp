// CShadowColourState_00ac0250::Reset() -- clear the flag byte (+0x30), set the three colours (+0x34, +0x38, +0x3c) to opaque black and zero the three words after them (+0x40..+0x48)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC0250, length 89 bytes (+ int3 padding). No callees.
// The chunk label "CInputProcessJumpingAndRolling::Init" is propagated (an engine module; eax is not `this` on return, so not a constructor).
// Shape: one `push ecx` frame slot holds each CRGBColour(0, 0, 0, 255) temporary in turn -- the inline four-channel Set stores R, G, B, A
// into the D3DCOLOR byte order (+2, +1, +0, +3; the landed 0x005BA48A model) and the dword is reloaded into esi/edx for the member
// store; the byte clear and the three word clears come from the shared zero register.
struct CRGBColour {                                 // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
};
struct CShadowColourState_00ac0250 {
    unsigned char _pad[0x30];
    bool          m_enabled;                        // +0x30
    unsigned char _pad31[3];
    CRGBColour    m_colour0;                        // +0x34
    CRGBColour    m_colour1;                        // +0x38
    CRGBColour    m_colour2;                        // +0x3c
    long          m_value40;                        // +0x40
    long          m_value44;                        // +0x44
    long          m_value48;                        // +0x48
    void Reset();
};

void CShadowColourState_00ac0250::Reset()
{
    m_enabled = false;
    m_colour0 = CRGBColour(0, 0, 0, 255);
    m_colour1 = CRGBColour(0, 0, 0, 255);
    m_colour2 = CRGBColour(0, 0, 0, 255);
    m_value40 = 0;
    m_value44 = 0;
    m_value48 = 0;
}