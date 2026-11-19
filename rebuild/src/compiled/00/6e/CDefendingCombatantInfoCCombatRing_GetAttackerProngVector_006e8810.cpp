// CDefendingCombatantInfo::CCombatRing::GetAttackerProngVector(const CThingCreatureBase& attacker) const -- the attacker's prong vector
//   rotated by the ring's RotationAngle (CProng::GetProngVector 0x006E7FD0), or the unit X vector when the attacker holds no prong; by value
// Leaf: ?GetAttackerProngVector@CCombatRing@CDefendingCombatantInfo@@QBE?AVC3DVector@@ABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E8810, length 64 bytes, `ret 8`.
// Identity (NF93 q11358): Ego_r.pdb 0x00711B20 / FableWin.pdb 0x02410110 ?GetAttackerProngVector@CCombatRing@CDefendingCombatantInfo@@QBE?AVC3DVector@@ABVCThingCreatureBase@@@Z;
//   engine/CDefendingCombatantInfo.h (WHEEL opt-in) declares it at 0x006E8810 and the landed caller 0x006E94D0 binds it. Was the local
//   CCombatRing / CProng model spelling a made-up GetAttackerProngPosition(CThing*).
// Callees: GetAttackerProng(const CThingCreatureBase&) const 0x006E87D0, CProng::GetProngVector(float) const 0x006E7FD0 (hidden result).
// Shape: the null test against a zero register that then feeds the two 0.0 stores of the fallback vector (1.0 as an immediate); the
//   prong path hands the hidden pointer through and returns it.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_WHEEL
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefendingCombatantInfo.h"

C3DVector CDefendingCombatantInfo::CCombatRing::GetAttackerProngVector(const CThingCreatureBase& attacker) const
{
    const CProng* prong = GetAttackerProng(attacker);
    if (prong)
        return prong->GetProngVector(RotationAngle);
    return C3DVector(1.0f, 0.0f, 0.0f);
}
