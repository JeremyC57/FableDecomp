// CEngineInternalPrimitiveLine::GetWorldPosition() const -- the line's midpoint: half of (end - start) per component, returned through the hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A11430, length 66 bytes.
// Callees: none. Constant 0.5 (0x0122F59C). Shape: the three differences are computed first (Z parked in the 12-byte frame), then each is scaled by 0.5 and
// stored into the hidden result (X, Y from the x87 stack, Z reloaded); `ret 4` pops the hidden pointer. The chunk label keeps the method name; the line's
// start (+0) and end (+0xc) vectors are modelled locally (no PDB layout).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CEngineInternalPrimitiveLine { public:
    float m_startX, m_startY, m_startZ;            // +0x00
    float m_endX, m_endY, m_endZ;                  // +0x0c
    C3DVector GetWorldPosition() const;
};

C3DVector CEngineInternalPrimitiveLine::GetWorldPosition() const
{
    C3DVector delta(m_endX - m_startX, m_endY - m_startY, m_endZ - m_startZ);
    return C3DVector(delta.X * 0.5f, delta.Y * 0.5f, delta.Z * 0.5f);
}