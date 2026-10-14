// CGuiControlComboBox_0065be32::ReleaseMembers() -- hand each of the five words at +0x4c..+0x5c to its own out-of-line member (the five __stdcall thunks @ 0x0066106D..0x006610B5)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065BE32, length 58 bytes.
// Callees: the landed thunks InvokeOnSelf_006603a0 @ 0x0066106D, _006603dd @ 0x0066107F, _0066041a @ 0x00661091, _00660457 @ 0x006610A3, _00660494 @ 0x006610B5
// (`__stdcall`, one argument: the member's address; retail keeps `this` in ecx for them). Shape: `this` in esi, each member addressed with `lea`, ecx reloaded before every call but
// the first. Size-optimised TU (0x0065xxxx). The chunk label "CGuiControlComboBox::OnDraw" is not trusted (Ego_r.pdb CGuiControlComboBox +0x4c..+0x5c are unnamed); named by address.
#pragma optimize("s", on)
struct CMember_0065be32 { void* m_p; };

struct CGuiControlComboBox_0065be32 {
    unsigned char     _pad[0x4c];
    CMember_0065be32  m_member4c;                  // +0x4c
    CMember_0065be32  m_member50;                  // +0x50
    CMember_0065be32  m_member54;                  // +0x54
    CMember_0065be32  m_member58;                  // +0x58
    CMember_0065be32  m_member5c;                  // +0x5c
    void Release4c(CMember_0065be32* member);      // 0x0066106D
    void Release50(CMember_0065be32* member);      // 0x0066107F
    void Release54(CMember_0065be32* member);      // 0x00661091
    void Release58(CMember_0065be32* member);      // 0x006610A3
    void Release5c(CMember_0065be32* member);      // 0x006610B5
    void ReleaseMembers();
};

void CGuiControlComboBox_0065be32::ReleaseMembers()
{
    Release4c(&m_member4c);
    Release50(&m_member50);
    Release54(&m_member54);
    Release58(&m_member58);
    Release5c(&m_member5c);
}
