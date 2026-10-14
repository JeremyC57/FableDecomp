// CCombatRing::GetAttackerProngPosition(CThing* attacker) const -- the attacker's prong rotated by the ring's RotationAngle (CProng @ 0x006E7FD0), or the unit X vector when the attacker holds no prong; by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E8810, length 64 bytes.
// Callees: CCombatRing::GetAttackerProng(CThing*) @ 0x006E87D0 (landed), the prong's rotated-position getter @ 0x006E7FD0 (C3DVector by hidden pointer, the angle by value;
// the manifest label "CTCPhysicsRigidBody::GetFacingVector" is propagated). Shape: the null test against a zero register that then feeds the two 0.0 stores of the
// fallback vector (1.0 as an immediate), the prong path hands the hidden pointer through and returns it, `ret 8`. The chunk label "CGuiControlComboBox::ToggleEntryWithName"
// is propagated; the body is CDefendingCombatantInfo::CCombatRing (Ego_r.pdb: RotationAngle +0).
struct CThing;
struct C3DVector {
    float x, y, z;
    C3DVector(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};
struct CProng {                                    // CDefendingCombatantInfo::CProng
    C3DVector GetRotatedPosition(float angle);     // 0x006E7FD0
};

struct CCombatRing {                               // CDefendingCombatantInfo::CCombatRing
    float RotationAngle;                           // +0x00
    CProng* GetAttackerProng(CThing* attacker);    // 0x006E87D0
    C3DVector GetAttackerProngPosition(CThing* attacker);
};

C3DVector CCombatRing::GetAttackerProngPosition(CThing* attacker)
{
    CProng* prong = GetAttackerProng(attacker);
    if (prong)
        return prong->GetRotatedPosition(RotationAngle);
    return C3DVector(1.0f, 0.0f, 0.0f);
}
