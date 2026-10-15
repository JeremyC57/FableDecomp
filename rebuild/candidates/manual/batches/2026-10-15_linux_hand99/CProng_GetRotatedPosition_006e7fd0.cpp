// CProng::GetRotatedPosition(float angle) -- the prong offset rotated about the Z axis by angle, by value (CDefendingCombatantInfo::CProng)
// Leaf: GetRotatedPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E7FD0, length 95 bytes; 5 callers in CCombatRing code (0x006E882D in the landed
// CCombatRing::GetAttackerProngPosition 0x006E8810, which already names this callee). The chunk label
// "CTCPhysicsRigidBody::GetFacingVector" is a propagated BSim name.
// Callees: C3DVector::Rotate(const C3DVector& axis, float angle) @ 0x00A13C60 (thiscall on the local copy).
// Shape: the packed prong vector (+1, +5, +9) is copied into a frame local through integer registers, the axis temporary
// (0, 0, 1.0f) is built with immediates, Rotate is called on the local, and the result is returned through C3DVector's
// memberwise user copy constructor: X goes through the x87 (`fld [esp] ; fstp [eax]`) and Y/Z through integer moves. Without
// the user copy constructor the copy is a block copy (`inc ecx` base, 97 v 95).
// Layout: Ego_r.pdb CDefendingCombatantInfo::CProng (0x1c, packed): Blocked +0, Prong (C3DVector) +1, ProngBlockageTimer +0x10,
// POccupier +0x14. No engine header; modelled locally (the nested class is written at namespace scope).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    void Rotate(const C3DVector& axis, float angle);   // 0x00A13C60
};

#pragma pack(push, 1)
struct CProng {                                    // CDefendingCombatantInfo::CProng
    bool      Blocked;                             // +0x00
    C3DVector Prong;                               // +0x01
    long      ProngBlockageTimer;                  // +0x0d
    C3DVector GetRotatedPosition(float angle);
};
#pragma pack(pop)

C3DVector CProng::GetRotatedPosition(float angle)
{
    C3DVector v = Prong;
    v.Rotate(C3DVector(0.0f, 0.0f, 1.0f), angle);
    return v;
}
