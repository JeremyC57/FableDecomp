// CEngineInternalPrimitiveParticleLine::GetWorldPosition(C3DVector& position) -- the line's midpoint: half of (start + end) with the start at +0x48 and the end at +0x54; always true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC3D50, length 88 bytes (96 with padding).
// Callees: none. Constant 0.5 (0x0122F59C). Shape: the three sums are built into a 12-byte local (Z parked at +0x14), the scaled vector is a second, named local
// (+0 .. +0x8) copied word by word into the argument (the implicit bitwise assignment through ecx/edx), `mov al,1` is the bool. The two vectors have no PDB
// layout for this class and are modelled locally.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CEngineInternalPrimitiveParticleLine {
    unsigned char _pad[0x48];
    float m_startX, m_startY, m_startZ;            // +0x48
    float m_endX, m_endY, m_endZ;                  // +0x54
    bool GetWorldPosition(C3DVector& position);
};

bool CEngineInternalPrimitiveParticleLine::GetWorldPosition(C3DVector& position)
{
    C3DVector sum(m_endX + m_startX, m_endY + m_startY, m_endZ + m_startZ);
    C3DVector result(sum.X * 0.5f, sum.Y * 0.5f, sum.Z * 0.5f);
    position = result;
    return true;
}
