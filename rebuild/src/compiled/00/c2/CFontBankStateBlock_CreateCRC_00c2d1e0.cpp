// CFontBankStateBlock::CreateCRC(long status) -- set the bank-update status on both CBankFile members at +0x18 (0x180 bytes each)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D1E0, length 48 bytes.
// Callees: CBankFile::SetBankUpdateStatus @ 0x00C38850 (on each bank, the status). Shape: the loop strength-reduced to a running bank address in esi and a
// count-down in edi (two iterations, `nop`-aligned top), `ret 4`. The manifest label "CFontBankStateBlock::CreateCRC" is propagated (the body sets a status).
struct CBankFile_00c2d1e0 {
    unsigned char _body[0x180];
    void SetBankUpdateStatus(long status);         // 0x00C38850
};

struct CFontBankStateBlock {
    unsigned char      _pad[0x18];
    CBankFile_00c2d1e0 m_banks[2];                 // +0x18
    void SetBanksUpdateStatus(long status);
};

void CFontBankStateBlock::SetBanksUpdateStatus(long status)
{
    for (int i = 0; i < 2; ++i)
        m_banks[i].SetBankUpdateStatus(status);
}