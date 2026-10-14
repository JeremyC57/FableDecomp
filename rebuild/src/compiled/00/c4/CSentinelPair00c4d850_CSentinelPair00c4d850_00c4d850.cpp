// CSentinelPair_00c4d850::CSentinelPair_00c4d850() -- two sentinel-marked longs (0xC0DAF00D) followed by two zeroed longs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4D850, length 24 bytes. No callees.
// The manifest label (C3DDummyObject::C3DDummyObject) is propagated; C3DDummyObject is 52 bytes
// and this object is 16. The body stores the four dwords from the top down (+0xc, +8, +4, +0):
// two chained assignments (the right-most member is stored first). Named by what it holds.
struct CSentinelPair_00c4d850 {
    unsigned long m_first;                 // +0 = 0xC0DAF00D
    unsigned long m_second;                // +4 = 0xC0DAF00D
    unsigned long m_count;                 // +8
    unsigned long m_extra;                 // +0xc
    CSentinelPair_00c4d850();
};

CSentinelPair_00c4d850::CSentinelPair_00c4d850()
{
    m_count = m_extra = 0;                 // chained: +0xc then +8
    m_first = m_second = 0xC0DAF00D;       // chained: +4 then +0
}