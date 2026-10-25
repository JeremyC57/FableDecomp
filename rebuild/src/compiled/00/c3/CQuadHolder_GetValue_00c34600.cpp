// CQuadHolder::GetValue() const -- returns the held 16-byte value (four words) by hidden pointer
// Leaf: GetValue
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C34600, 33 bytes, __fastcall `ret 4` (the hidden result pointer), eax = result.
//   Descriptive name (no PDB covers this library): the catalog's `GFToScreen` is propagated. Seven callers
//   (0x00C1C547, 0x00C20A79, 0x00C211C6, 0x00C2134B, 0x00C21482, 0x00C21506, 0x00C21AF9) call it on a member at
//   +0x38 of their object and hand the 16-byte frame copy to the next routine.
// Shape: a POD struct returned by value: four dword moves through esi, the result pointer copied to edx.
struct SQuad {
    float m_x, m_y, m_z, m_w;
};

struct CQuadHolder {
    SQuad m_value;                          // +0x00
    SQuad GetValue() const;
};

SQuad CQuadHolder::GetValue() const
{
    return m_value;
}