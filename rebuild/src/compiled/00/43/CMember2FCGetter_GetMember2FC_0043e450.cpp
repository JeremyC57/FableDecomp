// CMember2FCGetter::GetMember2FC() -- returns the dword member at +0x2FC
// Leaf: GetMember2FC
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043E450, 7 bytes. No callees; unreferenced (no call or address
// reference in the image), recovered from a mis-cut catalog row. Owner class unknown, so a local
// descriptive model holds the single member.
// Shape: `mov eax,[ecx+2FCh] ; ret`.
struct CMember2FCGetter {
    unsigned char _pad000[0x2fc];
    long m_value2FC;               // +0x2FC
    long GetMember2FC();
};

long CMember2FCGetter::GetMember2FC()
{
    return m_value2FC;
}