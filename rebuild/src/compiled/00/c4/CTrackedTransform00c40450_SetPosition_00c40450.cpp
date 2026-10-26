// CTrackedTransform_00c40450::SetPosition(const CTrackedVector& position) -- stores the position at +0x0c and raises dirty bit 0x8000 (+0x48) only when one of its three floats differs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40450, length 75 bytes.
// Leaf: SetPosition
// The manifest label "CShaderRenderManager::SetCameraPos" is propagated (the PDB CShaderRenderManager holds a
// CArray at +0x0c); the class and method names follow the landed caller 0x00C338C0 (script module, no RTTI).
// Shape: three `fld arg ; fld member ; fucompp ; test ah,44h` compares chained with `||` (`jp` to the update for
// the first two, `jnp` past it for the last: `member != argument`), the 12-byte POD copied through eax/eax/edx,
// `or dword ptr [ecx+48h],8000h`, `ret 4`.
class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector& operator=(const C3DVector& o) { X = o.X; Y = o.Y; Z = o.Z; return *this; }   // user operator=: the member stores may alias, so the flag RMW stays one `or [mem],imm`
};

struct CTrackedVector {                         // the setter's argument: a position wrapper built from a C3DVector
    C3DVector Value;
    CTrackedVector(const C3DVector& value) : Value(value) { }
};

struct CTrackedTransform_00c40450 {
    unsigned char _pad00[0x0c];
    C3DVector     m_position;                   // +0x0c
    C3DVector     m_direction;                  // +0x18 (0x00C404A0)
    unsigned char _pad24[0x24];
    unsigned long m_dirtyFlags;                 // +0x48

    void SetPosition(const CTrackedVector& position);
};

void CTrackedTransform_00c40450::SetPosition(const CTrackedVector& position)
{
    if (m_position.X != position.Value.X || m_position.Y != position.Value.Y || m_position.Z != position.Value.Z) {
        m_position = position.Value;
        m_dirtyFlags |= 0x8000;
    }
}