// CMatrix3x4_00a2e4b0::CMatrix3x4_00a2e4b0() -- identity: twelve floats zeroed, the three diagonal entries (+0, +0x10, +0x20) set to 1.0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E4B0, length 45 bytes. No callees.
// The manifest label (C3DDummyObject::C3DDummyObject) is propagated: the object is the 48-byte
// CMatrix3x4 itself (three rows of three plus a translation row), stored from the highest
// offset down -- the zero stores from +0x2c to +4 with the zero register, then 1.0f into
// +0x20, +0x10 and +0 from a register holding 0x3F800000.
struct CMatrix3x4_00a2e4b0 {
    float m[4][3];
    CMatrix3x4_00a2e4b0();
};

CMatrix3x4_00a2e4b0::CMatrix3x4_00a2e4b0()
{
    m[3][2] = 0.0f; m[3][1] = 0.0f; m[3][0] = 0.0f;
    m[2][1] = 0.0f; m[2][0] = 0.0f;
    m[1][2] = 0.0f; m[1][0] = 0.0f;
    m[0][2] = 0.0f; m[0][1] = 0.0f;
    m[2][2] = 1.0f; m[1][1] = 1.0f; m[0][0] = 1.0f;
}
