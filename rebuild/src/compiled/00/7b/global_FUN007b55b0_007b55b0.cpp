// _global::FUN_007b55b0(unsigned char) -- store a byte flag at this+0x241
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B55B0, 13 bytes, `ret 4`. Unreferenced out-of-line copy of an inline
//   one-byte setter; the owning class is not identified (modelled locally with the byte at +0x241).
struct CByteOwner_007b55b0 {
    unsigned char _pad[0x241];
    unsigned char m_value;                       // +0x241
    void FUN_007b55b0(unsigned char value);
};

void CByteOwner_007b55b0::FUN_007b55b0(unsigned char value)
{
    m_value = value;
}