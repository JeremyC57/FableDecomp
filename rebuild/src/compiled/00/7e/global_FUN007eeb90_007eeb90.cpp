// _global::FUN_007eeb90(long) -- store a dword at this+0x0C
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EEB90, 10 bytes, `ret 4`. Unreferenced out-of-line copy of an inline
//   setter; the owning class is not identified (modelled locally with the dword at +0x0C).
struct COwner_007eeb90 {
    unsigned char _pad[0x0C];
    long m_value;                                // +0x0C
    void FUN_007eeb90(long value);
};

void COwner_007eeb90::FUN_007eeb90(long value)
{
    m_value = value;
}