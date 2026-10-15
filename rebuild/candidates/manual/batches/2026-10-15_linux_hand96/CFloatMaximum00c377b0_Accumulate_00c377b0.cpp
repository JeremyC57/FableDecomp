// CFloatMaximum_00c377b0::Accumulate(float value) -- raise the running maximum at +0xc to the value when the value is larger
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C377B0, length 24 bytes. Callees: none.
// Leaf: Accumulate
// The chunk label "CTCLook::SetHeadTurnState" is propagated (CTCLook has no float at +0xc that is maxed): the only caller is
// the landed forwarder 0x00C44BC0 (`target->Receive(m_value)` with the member at +4) in the 0x00C4xxxx engine module, so
// the class is named by address. Shape: `fld value ; fcomp [ecx+0Ch] ; test ah,41h ; jne` is `value > m_max`
// (skip when below or equal), and the float is copied as a dword through eax.
struct CFloatMaximum_00c377b0 {
    unsigned char _pad[0xc];
    float         m_max;                           // +0x0c
    void Accumulate(float value);
};

void CFloatMaximum_00c377b0::Accumulate(float value)
{
    if (value > m_max)
        m_max = value;
}
