// SSettings_0041c930::SSettings_0041c930() -- a 0x20-byte record: two floats 1.0, three dwords 0, two dwords 1 and a -1 terminator id
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041C930, length 45 bytes.
// Leaf: ??0SSettings_0041c930
// The chunk label (CSoundInit::CSoundInit) is propagated over every small zero/default constructor of the
// sound/engine modules; no vtable is stored, so the record is named by address.
// Shape: no frame, `mov eax,ecx`, integer zero in ecx and one in edx, the two 1.0f floats as immediates.
// The stores run +0, +4, +8, +0xc, +0x14, +0x18, +0x1c and only then +0x10: the members initialised in the
// initialiser list come out in declaration order, the +0x10 dword is assigned in the constructor BODY
// (scheduled after the list). The zeroes share a register, so they are integers (a float zero is an
// immediate store).
struct SSettings_0041c930 {
    float m_scale0;                             // +0x00 = 1.0f
    float m_scale1;                             // +0x04 = 1.0f
    long  m_8;                                  // +0x08 = 0
    long  m_c;                                  // +0x0c = 1
    long  m_10;                                 // +0x10 = 1 (body)
    long  m_14;                                 // +0x14 = 0
    long  m_18;                                 // +0x18 = 0
    long  m_id;                                 // +0x1c = -1
    SSettings_0041c930();
};

SSettings_0041c930::SSettings_0041c930()
    : m_scale0(1.0f), m_scale1(1.0f), m_8(0), m_c(1), m_14(0), m_18(0), m_id(-1)
{
    m_10 = 1;
}
