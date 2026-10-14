// CMapInfo::CMapInfo() -- the two dwords at +0x10 / +0x14 cleared, the three CCharStrings at +0x18 / +0x1c / +0x2c default-constructed (0x0099E4B0), then Clear (0x004FDDE0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FDE50, length 48 bytes.
// Callees: CCharString::CCharString @ 0x0099E4B0, CMapInfo::Clear @ 0x004FDDE0. Shape: no vtable, `this` in esi, one zero register, `ret`.
struct CCharString {
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};

struct CMapInfo {
    unsigned char _pad00[0x10];
    long          m_10;                            // +0x10
    long          m_14;                            // +0x14
    CCharString   m_name;                          // +0x18
    CCharString   m_file;                          // +0x1c
    unsigned char _pad20[0xc];
    CCharString   m_region;                        // +0x2c
    void Clear();                                  // 0x004FDDE0
    CMapInfo();
};

CMapInfo::CMapInfo()
    : m_10(0), m_14(0), m_name(), m_file(), m_region()
{
    Clear();
}
