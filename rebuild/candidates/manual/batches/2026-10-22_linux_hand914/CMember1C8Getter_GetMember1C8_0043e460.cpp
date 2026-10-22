// CMember1C8Getter::GetMember1C8() -- returns the dword member at +0x1C8
// Leaf: GetMember1C8
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043E460, 7 bytes. No callees; unreferenced (no call or address
// reference in the image), recovered from a mis-cut catalog row. Owner class unknown, so a local
// descriptive model holds the single member.
// Shape: `mov eax,[ecx+1C8h] ; ret`.
struct CMember1C8Getter {
    unsigned char _pad000[0x1c8];
    long m_value1C8;               // +0x1C8
    long GetMember1C8();
};

long CMember1C8Getter::GetMember1C8()
{
    return m_value1C8;
}
