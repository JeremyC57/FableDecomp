// CMember2ECGetter::GetMember2EC() -- returns the dword member at +0x2EC
// Leaf: GetMember2EC
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043E440, 7 bytes. No callees; unreferenced (no call or address
// reference in the image), recovered from a mis-cut catalog row. Owner class unknown, so a local
// descriptive model holds the single member.
// Shape: `mov eax,[ecx+2ECh] ; ret` -- an inline-style getter emitted out of line.
struct CMember2ECGetter {
    unsigned char _pad000[0x2ec];
    long m_value2EC;               // +0x2EC
    long GetMember2EC();
};

long CMember2ECGetter::GetMember2EC()
{
    return m_value2EC;
}
