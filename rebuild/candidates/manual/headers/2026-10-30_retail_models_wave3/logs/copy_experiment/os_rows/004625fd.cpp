// CMeleeCombatKnockdownEffects::operator=(const CMeleeCombatKnockdownEffects& other) -- the memberwise def assignment: the CParentDefClassBase part, then every CMeleeCombatKnockdownEffects member in declaration order; returns *this
// Leaf: ??4CMeleeCombatKnockdownEffects@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004625FD, 87 bytes, thiscall `ret 4`; size-optimised def TU.
// Identity (proven): CMeleeCombatKnockdownEffects::operator=; evidence: its only reference is the 5-byte `jmp` at 0x004625F8 (the tail-calling virtual in slot 19 of
//   vtable 0x01234DC4, RTTI .?AVCMeleeCombatKnockdownEffects@@; as the landed CHeroExperienceDef::operator= 0x004F914A), it copies exactly the CMeleeCombatKnockdownEffects members
//   (offsets and sizes asserted against Ego_r.pdb, 4 bytes below the PDB: the retail parent-def base is 0x39 bytes), and the PDB name sets hold ??4CMeleeCombatKnockdownEffects@@QAEAAV0@ABV0@@Z.
// Label: the manifest's "CParentDefClassBase::operator=" is propagated (family H055: the six H052 defs' assignments, each its own COMDAT).
// Callee: CParentDefClassBase::operator= 0x00431ED0 (CDefClassBase::operator=, then InstantiationName +0x25 and the SubDefInfoMap vector map +0x29).
// Layout: CParentDefClassBase is 0x39 bytes (pack(1) in the PDB, members from +0x3C); local classes -- the generated engine headers flatten the
//   bases (no base operator= to call), Review at landing.
// Shape: the compiler-generated assignment written out in declaration order; float members copy through eax as integer moves.
#pragma optimize("s", on)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#include "/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad/mig3/work/x/chain.h"
class CMeleeCombatKnockdownEffects : public CParentDefClassBase {                       // vtable 0x01234DC4
public:
    long BlendInFrames;                                // +0x3C (Ego_r +0x40)
    long BlendOutFrames;                               // +0x40 (Ego_r +0x44)
    long AttackerLightingChannel;                      // +0x44 (Ego_r +0x48)
    long TargetLightingChannel;                        // +0x48 (Ego_r +0x4C)
    long EnvironmentTheme;                             // +0x4C (Ego_r +0x50)
    bool CameraFOVChange;                              // +0x50 (Ego_r +0x54)
    bool CameraPan;                                    // +0x51 (Ego_r +0x55)
    float AttackerAndTargetSpeedMultiplier;            // +0x54 (Ego_r +0x58)
    bool PauseEntities;                                // +0x58 (Ego_r +0x5C)
    bool NonAttackingEntities;                         // +0x59 (Ego_r +0x5D)
    bool AttackerInvulnerable;                         // +0x5A (Ego_r +0x5E)
    CMeleeCombatKnockdownEffects& operator=(const CMeleeCombatKnockdownEffects& other);
};

CMeleeCombatKnockdownEffects& CMeleeCombatKnockdownEffects::operator=(const CMeleeCombatKnockdownEffects& other)
{
    CParentDefClassBase::operator=(other);
    BlendInFrames = other.BlendInFrames;
    BlendOutFrames = other.BlendOutFrames;
    AttackerLightingChannel = other.AttackerLightingChannel;
    TargetLightingChannel = other.TargetLightingChannel;
    EnvironmentTheme = other.EnvironmentTheme;
    CameraFOVChange = other.CameraFOVChange;
    CameraPan = other.CameraPan;
    AttackerAndTargetSpeedMultiplier = other.AttackerAndTargetSpeedMultiplier;
    PauseEntities = other.PauseEntities;
    NonAttackingEntities = other.NonAttackingEntities;
    AttackerInvulnerable = other.AttackerInvulnerable;
    return *this;
}