// CMarkerInfo::IsRotatedOnMap() -- true unless the marker's icon (+8) is one of the five fixed (non-rotating) gui icons of the gui def (PeekGuiDef @ 0x00438570 -> +0x958: the dwords +0x28, +0x18, +0x1c, +0x20, +0x24)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A230, length 64 bytes.
// Leaf: IsRotatedOnMap
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`, no arguments). Shape: five compares with one shared false exit, `ret`.
struct SGuiMarkerIcons_0082a230 {
    unsigned char _pad00[0x18];
    long m_iconA;                                  // +0x18
    long m_iconB;                                  // +0x1c
    long m_iconC;                                  // +0x20
    long m_iconD;                                  // +0x24
    long m_iconE;                                  // +0x28
};
struct CGuiDef_0082a230 {
    unsigned char           _pad00[0x958];
    SGuiMarkerIcons_0082a230* m_pMarkerIcons;      // +0x958
};
CGuiDef_0082a230* __fastcall PeekGuiDef_00438570();   // 0x00438570

struct CMarkerInfo {
    unsigned char _pad00[8];
    long          m_icon;                          // +0x08
    bool IsRotatedOnMap();
};

bool CMarkerInfo::IsRotatedOnMap()
{
    SGuiMarkerIcons_0082a230* icons = PeekGuiDef_00438570()->m_pMarkerIcons;
    if (m_icon == icons->m_iconE || m_icon == icons->m_iconA || m_icon == icons->m_iconB || m_icon == icons->m_iconC || m_icon == icons->m_iconD)
        return false;
    return true;
}
