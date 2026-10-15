// CRenderSetting_00c44880::SetValue(long value) -- store the value at +0x104 when it differs from the current one and mark the setting dirty (bit 14 of the flag word at +0x14)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44880, length 28 bytes. Callees: none.
// Leaf: SetValue
// The chunk label "CTCLook::SetHeadTurnState" is propagated: the body lives in the 0x00C4xxxx engine-settings module next to
// the landed setters 0x00C445C0 (float +0xfc, bit 9) and 0x00C44860 (long +0x100, bit 13), and its only callers are the
// null-object `__stdcall` helpers 0x00C36040 / 0x00C4EEBD of that module; the class is named by address like its siblings.
// Shape: the change-tested store `if (m_value != value) { m_value = value; m_flags |= 0x4000; }` (`or dword ptr [ecx+14h],4000h`).
struct CRenderSetting_00c44880 {
    unsigned char _pad[0x14];
    unsigned long m_flags;                         // +0x14: bit 14 = value changed
    unsigned char _pad18[0x104 - 0x18];
    long          m_value;                         // +0x104
    void SetValue(long value);
};

void CRenderSetting_00c44880::SetValue(long value)
{
    if (m_value != value) {
        m_value = value;
        m_flags |= 0x4000;
    }
}