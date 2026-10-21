// CTrackedTransform_00c404a0::SetDirection(const CTrackedVector& direction) -- stores the direction at +0x18 and raises dirty bit 0x10000 (+0x48) only when one of its three floats differs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C404A0, length 75 bytes.
// Leaf: SetDirection
// The manifest label "CShaderRenderManager::SetCameraPos" is propagated (the PDB CShaderRenderManager holds a
// CArray at +0x0c); the class and method names follow the landed caller 0x00C338F0 (script module, no RTTI).
// Shape: three `fld arg ; fld member ; fucompp ; test ah,44h` compares chained with `||` (`jp` to the update for
// the first two, `jnp` past it for the last: `member != argument`), the 12-byte POD copied through eax/eax/edx,
// `or dword ptr [ecx+48h],10000h`, `ret 4`.
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector& operator=(const C3DVector& o) { X = o.X; Y = o.Y; Z = o.Z; return *this; }   // user operator=: the member stores may alias, so the flag RMW stays one `or [mem],imm`
};

struct CTrackedVector {                         // the setter's argument: a position wrapper built from a C3DVector
    C3DVector Value;
    CTrackedVector(const C3DVector& value) : Value(value) { }
};

struct CTrackedTransform_00c404a0 {
    unsigned char _pad00[0x0c];
    C3DVector     m_position;                   // +0x0c (0x00C40450)
    C3DVector     m_direction;                  // +0x18
    unsigned char _pad24[0x24];
    unsigned long m_dirtyFlags;                 // +0x48

    void SetDirection(const CTrackedVector& direction);
};

void CTrackedTransform_00c404a0::SetDirection(const CTrackedVector& direction)
{
    if (m_direction.X != direction.Value.X || m_direction.Y != direction.Value.Y || m_direction.Z != direction.Value.Z) {
        m_direction = direction.Value;
        m_dirtyFlags |= 0x10000;
    }
}