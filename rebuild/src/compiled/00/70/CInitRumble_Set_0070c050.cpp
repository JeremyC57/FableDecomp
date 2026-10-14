// CInitRumble::Set(bool a, long b, long c, long d, bool e, long f, long g) -- fill the rumble record: the three scales at +0xc..+0x14 = 1.0f, the two words at +0x18/+0x1c cleared, then the arguments into +0 (d), +4 (a), +8 (c), +5 (e), +0x20 (f), +0x24 (b), +0x28 (g)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C050, length 80 bytes.
// The chunk label "CInitRumble::CInitRumble" names a constructor, but eax is not `this` on return: a setter. Shape: one `mov eax,3F800000h` for three stores, `ret 1Ch`.
struct CInitRumble {
    long  m_0;                                     // +0x00
    bool  m_4;                                     // +0x04
    bool  m_5;                                     // +0x05
    long  m_8;                                     // +0x08
    float m_c;                                     // +0x0c
    float m_10;                                    // +0x10
    float m_14;                                    // +0x14
    long  m_18;                                    // +0x18
    long  m_1c;                                    // +0x1c
    long  m_20;                                    // +0x20
    long  m_24;                                    // +0x24
    long  m_28;                                    // +0x28
    void Set(bool a, long b, long c, long d, bool e, long f, long g);
};

void CInitRumble::Set(bool a, long b, long c, long d, bool e, long f, long g)
{
    m_c = 1.0f;
    m_10 = 1.0f;
    m_14 = 1.0f;
    m_18 = 0;
    m_1c = 0;
    m_0 = d;
    m_4 = a;
    m_8 = c;
    m_5 = e;
    m_20 = f;
    m_24 = b;
    m_28 = g;
}