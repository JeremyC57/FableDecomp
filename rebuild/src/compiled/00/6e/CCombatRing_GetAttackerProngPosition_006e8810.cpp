// CCombatRing::GetAttackerProngPosition(CThing* attacker) const -- the attacker's prong rotated by the ring's RotationAngle (CProng @ 0x006E7FD0), or the unit X vector when the attacker holds no prong; by value
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E8810, length 64 bytes.
// Callees: CCombatRing::GetAttackerProng(CThing*) @ 0x006E87D0 (landed), the prong's rotated-position getter @ 0x006E7FD0 (C3DVector by hidden pointer, the angle by value;
// the manifest label "CTCPhysicsRigidBody::GetFacingVector" is propagated). Shape: the null test against a zero register that then feeds the two 0.0 stores of the
// fallback vector (1.0 as an immediate), the prong path hands the hidden pointer through and returns it, `ret 8`. The chunk label "CGuiControlComboBox::ToggleEntryWithName"
// is propagated; the body is CDefendingCombatantInfo::CCombatRing (Ego_r.pdb: RotationAngle +0).
class CThing;
#include "engine/C3DVector.h"
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