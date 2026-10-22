// CInputTypeMouseButtonEvent::IsInputTypeWithMouseButton(long button) -- whether this event's button word (+4) is the given one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00447A90, length 32 bytes.
// The manifest leaf is `vfunc_7`. Shape: `xor eax,eax ; cmp ; sete al`, `ret 4`.
struct CInputTypeMouseButtonEvent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual bool IsInputTypeWithMouseButton(long button);            // slot 7
    long m_button;                                 // +0x04
};

bool CInputTypeMouseButtonEvent::IsInputTypeWithMouseButton(long button)
{
    return m_button == button;
}