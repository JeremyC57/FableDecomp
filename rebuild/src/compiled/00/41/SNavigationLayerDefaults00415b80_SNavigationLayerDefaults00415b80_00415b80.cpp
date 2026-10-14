// SNavigationLayerDefaults_00415b80::SNavigationLayerDefaults_00415b80() -- a 0x2c-byte record: the nine dwords +0..+0x18 and +0x20 / +0x24 set to -1, the bytes +0x1c and +0x29 cleared, +0x28 and +0x2a set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00415B80, length 48 bytes.
// The chunk label (CNavigationLayer::CNavigationLayer) is propagated; named by address. Shape: no frame, -1 in ecx (`or ecx,-1`), zero in dl, one in cl, `ret`.
struct SNavigationLayerDefaults_00415b80 {
    long m_0, m_4, m_8, m_c, m_10, m_14, m_18;    // +0x00..+0x18
    bool m_1c;                                     // +0x1c
    long m_20, m_24;                               // +0x20, +0x24
    bool m_28;                                     // +0x28
    bool m_29;                                     // +0x29
    bool m_2a;                                     // +0x2a
    SNavigationLayerDefaults_00415b80();
};

SNavigationLayerDefaults_00415b80::SNavigationLayerDefaults_00415b80()
    : m_0(-1), m_4(-1), m_8(-1), m_c(-1), m_10(-1), m_14(-1), m_18(-1), m_1c(false), m_20(-1), m_24(-1), m_28(true), m_29(false), m_2a(true)
{
}