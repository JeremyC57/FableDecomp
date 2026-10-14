#pragma optimize("s", on)
// CFloatRecord_004d4033::CFloatRecord_004d4033() -- default constructor of a 28-byte record: six floats (one of them 1.0f) and a long set to 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4033, length 39 bytes.
// Callees: none. Size-optimised TU (odd address inside the /Os region): every float is stored through the x87
// (`fldz`/`fld1` + `fstp`, 8 bytes a pair) instead of a 10-byte immediate; the long at +0xc is an immediate store
// the scheduler hoists next to the first float. The manifest label "CDateAndTime::CDateAndTime" is propagated
// from a same-length body; CDateAndTime holds no floats, so the class is named by address and shape.
struct CFloatRecord_004d4033 {
    float m_start;          // +0x00  0.0f
    float m_scale;          // +0x04  1.0f
    float m_offset;         // +0x08  0.0f
    long  m_count;          // +0x0c  1
    float m_a;              // +0x10  0.0f
    float m_b;              // +0x14  0.0f
    float m_c;              // +0x18  0.0f
    CFloatRecord_004d4033();
};

CFloatRecord_004d4033::CFloatRecord_004d4033()
    : m_start(0.0f), m_scale(1.0f), m_offset(0.0f), m_count(1), m_a(0.0f), m_b(0.0f), m_c(0.0f)
{
}
