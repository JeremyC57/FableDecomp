// CCombatWheel::ReassignAttacker(CWorld*, CCombatantInfo*, CCombatantInfo*, const C3DVector*) -- give the attacker a fresh preferred attack dolly: the flat direction from the defender to the given position, or to the attacker turned by a random 0.25..0.75 turn; when it is not degenerate it is normalised and stored with a five-second timer
// Leaf: ?ReassignAttacker@CCombatWheel@@QAEXPAVCWorld@@PAUCCombatantInfo@@1PBVC3DVector@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E80F0, 383 bytes (385 incl. a padding byte). `ret 10h` (`this` unused).
// Callees: CThing::GetPosition 0x004C73D0, CWorld's random-seed accessor 0x0049D810 (catalogued
//          CVertexBufferWin32::DoSizeof), `__fastcall` GFFloatRandom(float, unsigned long*) 0x004978A0,
//          C3DVector::Rotate(const C3DVector&, float) 0x00A13C60, C3DVector::AccurateNormalise @ 0x00A14440 (catalogued
//          GetScaled), _ftol2 0x00BFEA70. Globals: 1e-4f 0x0129BA3C (read as a variable), 0.25f 0x0123078C,
//          5.0f 0x01261D64, the integer frame rate 0x01375550.
//
// Shape: each arm assigns the difference through the implicit operator= (the dead Z copy is kept before the
// `Z = 0` store); the squared length is `X*X + Y*Y + Z*Z` (VC7.1 emits Y first; the else arm folds Z = 0);
// the rotation axis is a C3DVector(0, 0, 1) temporary in the subtraction's dead slots.
// Layout: CCombatantInfo / CAttackingCombatantInfo of the landed copy ctor 0x006F4990: Parent +0,
// PreferredAttackDollySet +0x10, PreferredAttackDollyTimer +0x14, PreferredAttackDollyVector +0x18.
class C2DVector { public:
    float X;
    float Y;
};
#include "engine/C3DVector.h"
extern const float DELTA_NOTIONAL_ZERO; // 0x0129BA3C (1e-4f)
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

#include "engine/CThing.h"
class CThingCreatureBase : public CThing { public:
};
class CWorld { public:
    unsigned long* GetWorldSeed() const;         // 0x0049D810
};
class CAttackingCombatantInfo { public:
    CThingCreatureBase* Attacked;           // +0x00
    long AttackerNumber;                    // +0x04
    long CombatWheelRingIndex;              // +0x08
    bool PreferredAttackDollySet;           // +0x0c
    long PreferredAttackDollyTimer;         // +0x10
    C2DVector PreferredAttackDollyVector;   // +0x14
    bool UsingCombatWheel;                  // +0x1c
};
struct CCombatantInfo {
    CThingCreatureBase* Parent;             // +0x00
    CAttackingCombatantInfo AttackerInfo;   // +0x04
};
class CCombatWheel {
public:
    void ReassignAttacker(CWorld* world, CCombatantInfo* attacker, CCombatantInfo* defender, const C3DVector* position);
};

void CCombatWheel::ReassignAttacker(CWorld* world, CCombatantInfo* attacker, CCombatantInfo* defender, const C3DVector* position)
{
    C3DVector dolly;
    if (position) {
        dolly = *position - defender->Parent->GetPos();
        dolly.Z = 0.0f;
    } else {
        dolly = attacker->Parent->GetPos() - defender->Parent->GetPos();
        dolly.Z = 0.0f;
        if (dolly.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO)
            dolly.Rotate(C3DVector(0.0f, 0.0f, 1.0f), GFFloatRandom(0.5f, world->GetWorldSeed()) + 0.25f);
    }
    if (dolly.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
        dolly.AccurateNormalise();
        attacker->AttackerInfo.PreferredAttackDollySet = true;
        attacker->AttackerInfo.PreferredAttackDollyTimer = (long)(CMainGameComponent::GetConstantFPS() * 5.0f);
        attacker->AttackerInfo.PreferredAttackDollyVector.X = dolly.X;
        attacker->AttackerInfo.PreferredAttackDollyVector.Y = dolly.Y;
    }
}