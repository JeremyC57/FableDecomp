// _global::FUN_00a0d260() -- clear the byte flag at this+0x26
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0D260, 5 bytes, `mov byte [ecx+26h],0 ; ret`. Unreferenced out-of-line
//   copy of an inline reset; the owning class is not identified (modelled locally with the byte at +0x26).
struct COwner_00a0d260 {
    unsigned char _pad[0x26];
    bool m_flag;                                 // +0x26
    void FUN_00a0d260();
};

void COwner_00a0d260::FUN_00a0d260()
{
    m_flag = false;
}