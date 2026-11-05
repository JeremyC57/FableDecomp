// CTCBoss::SetNextBossPhase(long phase, bool force) -- when phase changes are enabled (or forced) store the new phase and remember the boss's position as the phase start
// Leaf: ?SetNextBossPhase@CTCBoss@@QAEXJ_N@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
//
// Retail 0x007AEDA0, 56 bytes (`ret 8`). Callee: CThing::GetPos() const @ 0x004C73D0 (returns a
// const C3DVector&; the manifest label "CVertexBufferWin32::DoSizeof" is propagated).
// Layout (Ego_r.pdb CTCBoss, offsets confirmed by the bytes): currentBossPhase +0x18,
// EnablePhaseChanges +0x21, PhaseStartPos +0x22 -- an UNALIGNED C3DVector, the thing-component
// hierarchy is byte packed. Shape: `EnablePhaseChanges || force` tests the member byte first and
// the bool argument second; the phase store is scheduled before the GetPos call and the
// 12-byte vector is copied through `add esi,22h` (a plain struct copy).

#include "engine/C3DVector.h"

#include "engine/CThing.h"

#pragma pack(push, 1)
class CTCBoss
{ public:
    void*         __vftable;                        // +0x00 (CTCBase)
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];                   // +0x08 (CTCBase flags)
    void*         PBossDef;                         // +0x0c CDefPointer<const CBossDef>
    long          GenericCounter;                   // +0x10
    long          StrikesSinceHit;                  // +0x14
    long          currentBossPhase;                 // +0x18
    long          NextWeapon;                       // +0x1c
    bool          BCountStrikes;                    // +0x20
    bool          EnablePhaseChanges;               // +0x21
    C3DVector     PhaseStartPos;                    // +0x22

    void SetNextBossPhase(long phase, bool force);
};
#pragma pack(pop)

void CTCBoss::SetNextBossPhase(long phase, bool force)
{
    if (EnablePhaseChanges || force)
    {
        currentBossPhase = phase;
        PhaseStartPos = ParentThing->GetPos();
    }
}