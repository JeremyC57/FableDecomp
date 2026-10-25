// CAmbientCube::SetCentre(const C3DVector& centre) -- place the cube: each of the eight apex positions becomes the centre plus that apex's offset
// Leaf: ?SetCentre@CAmbientCube@@QAEXABUC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C458D0, 63 bytes (+ int3 padding), __fastcall `ret 4`.
// Library: libpk ambient sound (no PDB/RTTI): descriptive names. Callers: the owner's forwarder 0x00C361F0 (cube at
//   owner +0x4C, after 0x00C457E0 on the same cube) and 0x00C461C9. The manifest's "CQuadric_AccumulateAll" is
//   propagated.
// Callee: the landed `__fastcall` operator+ 0x00C257E0 (hidden result in ecx, centre in edx, offset pushed).
// Shape: a countdown of eight over a 0x1C-byte stride; the returned temporary is copied into the position with three
//   dword moves through edx/eax.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetMagnitude() const;             // 0x00C25970
};
C3DVector __fastcall operator-(const C3DVector& a, const C3DVector& b);   // 0x00C257B0
C3DVector __fastcall operator+(const C3DVector& a, const C3DVector& b);   // 0x00C257E0

struct SAmbientApex {
    C3DVector m_offset;                     // +0x00 corner offset from the cube's centre
    C3DVector m_position;                   // +0x0c corner position
    float     m_minDistance;                // +0x18 closest listener distance seen
};

class CAmbientCube {
public:
    SAmbientApex m_apexes[8];               // +0x00
    void SetCentre(const C3DVector& centre);
};

void CAmbientCube::SetCentre(const C3DVector& centre)
{
    for (int i = 0; i < 8; ++i)
        m_apexes[i].m_position = centre + m_apexes[i].m_offset;
}