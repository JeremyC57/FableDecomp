// NPlayerGui::CDrawCombatMultiplier::AddHitByMelee(CThing& attacker, CThing& victim, const C3DVector& pos) -- spawn the GUI def's "smack" particle effect at the hit position
// Leaf: ?AddHitByMelee@CDrawCombatMultiplier@NPlayerGui@@QAEXAAVCThing@@0ABVC3DVector@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643D8C, 89 bytes, `ret 0Ch`. Size-optimised NPlayerGui TU (frame pointer,
// `leave`, fldz/fld1 + fstp constant stores, no function alignment).
// Identity (NF348, 2026-11-24; was the CPlayerGui self): Ego_r.pdb + FableWin.pdb
// `?AddHitByMelee@CDrawCombatMultiplier@NPlayerGui@@QAEXAAVCThing@@0ABVC3DVector@@@Z`, in the NPlayerGui drawer code region. It is reached
// only through 0x00437990 = ?AddHitByMelee@CPlayerGui@@QAEXAAVCThing@@0ABVC3DVector@@@Z (`mov ecx,[ecx+150h] ; jmp` here: PDrawCombatMultiplier
// +0x150 in engine/CPlayerGui.h; Ego_r's CPlayerGui::AddHitByMelee jumps to CDrawCombatMultiplier::AddHitByMelee), called from 0x00667F48
// with the GUI singleton 0x013B8790. The two land as one unit. Catalog label "InitParticleEmitter" is descriptive.
// Callees: PeekGuiDef 0x00438570 (__fastcall, as in the landed DrawStatBox 0x00645CE9), CRightHandedSet(const C3DVector& up,
// const C3DVector& forward) 0x00487050, NParticleEngine::GetParticleEmitterManager() 0x00AEAAF0, CreateEmitter(long,
// const C3DVector&, bool, const CRightHandedSet&) 0x00AEB6D0 (result dropped).
// Layout: CPlayerGuiDef SmackEffect at +0x48C (Ego_r.pdb +0x4BC; twelve 16-byte PDB vectors before it are 12 bytes in
// retail). `this`, attacker and victim are unused.
// Shape: the def is fetched first (named local); CreateEmitter's arguments evaluate right to left, so the orientation
// temporaries come first: forward (0,1,0), then up (0,0,1), then the set; GetParticleEmitterManager() runs after the pushes.
#pragma optimize("s", on)
#include "engine/C3DVector.h"

class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);   // 0x00487050
};

class CPlayerGuiDef { public:
    unsigned char _pad[0x48c];
    long          SmackEffect;                                        // +0x48C (retail)
};
extern const CPlayerGuiDef* __fastcall PeekGuiDef();                  // 0x00438570

namespace NParticleEngine {
class CParticleEmitter;
class CParticleEmitterManager {
public:
    CParticleEmitter* CreateEmitter(long effect, const C3DVector& pos, bool start,
                                    const CRightHandedSet& orientation);   // 0x00AEB6D0
};
// Name-fix pass (2026-11-12): Ego_r.pdb + FableWin.pdb ?GetParticleEmitterManager@NParticleEngine@@YIAAVCParticleEmitterManager@1@XZ
// (landed row 0x00AEAAF0); was the static CParticleEmitterManager::Get() stand-in.
CParticleEmitterManager& __fastcall GetParticleEmitterManager();      // 0x00AEAAF0
}

class CThing;

// NPlayerGui::CDrawCombatMultiplier has no shared header: its one member defined here is declared below.
namespace NPlayerGui {
class CDrawCombatMultiplier {
public:
    void AddHitByMelee(CThing& attacker, CThing& victim, const C3DVector& pos);
};
}

void NPlayerGui::CDrawCombatMultiplier::AddHitByMelee(CThing& attacker, CThing& victim, const C3DVector& pos)
{
    const CPlayerGuiDef* def = PeekGuiDef();
    NParticleEngine::GetParticleEmitterManager().CreateEmitter(def->SmackEffect, pos, false,
        CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
}