// CAmbientCube::UpdateDistances(const C3DVector& listener, float height) -- for each of the cube's eight apexes, measure the distance from the listener (raised by up to `height`, but not above an apex that is higher than the listener) to the apex position and keep the smallest distance seen
// Leaf: ?UpdateDistances@CAmbientCube@@QAEXABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C45810, 142 bytes (+ int3 padding), __fastcall `ret 8`.
// Library: libpk ambient sound (no PDB/RTTI): descriptive names (the TU's dump prints "Ambient cube:-" and
//   "   Apex (" per corner). Only caller the API forwarder 0x00C36220 (`add ecx,4Ch ; jmp`: the cube lives at +0x4C of
//   its owner). The manifest's "CParticleMorphs_FindMinimumDistance" is propagated.
// Layout: eight 0x1C-byte apexes {offset, position, minimum distance}; SetCentre 0x00C458D0 fills the positions.
// Callees: the landed `__fastcall` operator- 0x00C257B0 and C3DVector::GetMagnitude 0x00C25970.
// Shape: the listener is copied into a local every pass; the raised height is `top > apex.Z ? apex.Z : top` (one
//   store per arm); the difference must be a NAMED local (GetMagnitude then takes `lea ecx,[esp+18h]`, a direct
//   `(a - b).GetMagnitude()` passes the returned pointer, `mov ecx,eax`, 140 bytes).
class C3DVector { public:
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
    void UpdateDistances(const C3DVector& listener, float height);
};

void CAmbientCube::UpdateDistances(const C3DVector& listener, float height)
{
    for (int i = 0; i < 8; ++i) {
        C3DVector point = listener;
        if (m_apexes[i].m_position.Z > point.Z) {
            float top = point.Z + height;
            point.Z = top > m_apexes[i].m_position.Z ? m_apexes[i].m_position.Z : top;
        }
        C3DVector delta = point - m_apexes[i].m_position;
        float distance = delta.GetMagnitude();
        if (distance < m_apexes[i].m_minDistance)
            m_apexes[i].m_minDistance = distance;
    }
}