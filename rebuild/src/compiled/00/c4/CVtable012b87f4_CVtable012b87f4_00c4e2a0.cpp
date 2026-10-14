// CVtable_012b87f4::CVtable_012b87f4(bool flag) -- this class's vtable (0x012B87F4, no RTTI), the word at +4 and the word at +0xec cleared, the flag widened into the word at +0xe8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4E2A0, length 48 bytes.
// The chunk label "CIndexBuffer::CIndexBuffer" is propagated; named by its vtable. Shape: `mov eax,ecx` first, the flag in bl, `cmp bl,cl ; setne dl`, `ret 4`.
struct CVtable_012b87f4 {                          // vtable 0x012B87F4
    virtual void v00();
    long          m_4;                             // +0x04
    unsigned char _pad08[0xe0];
    long          m_e8;                            // +0xe8 (the flag as a word)
    long          m_ec;                            // +0xec
    CVtable_012b87f4(bool flag);
};

CVtable_012b87f4::CVtable_012b87f4(bool flag)
    : m_4(0), m_e8(flag != false), m_ec(0)
{
}