// CInputTypeMouseMovementEvent::GetMovementRatio() -- the ratio of the two offsets (+0x1c and +0x10, each less the base at +0x48 and the virtual slot 6 origin) as a float
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006925E0, length 64 bytes.
// The chunk label "operator==" is propagated onto a float-returning body. Shape: an 8-byte frame for the two integers, `fild ; fidiv`, `ret`.
struct CInputTypeMouseMovementEvent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual long GetOrigin();                      // slot 6 (+0x18)
    unsigned char _pad04[0xc];
    long m_10;                                     // +0x10
    unsigned char _pad14[8];
    long m_1c;                                     // +0x1c
    unsigned char _pad20[0x28];
    long m_base;                                   // +0x48
    float GetMovementRatio();
};

float CInputTypeMouseMovementEvent::GetMovementRatio()
{
    long denominator = m_10 - m_base - GetOrigin();
    long numerator = m_1c - m_base - GetOrigin();
    return (float)numerator / (float)denominator;
}