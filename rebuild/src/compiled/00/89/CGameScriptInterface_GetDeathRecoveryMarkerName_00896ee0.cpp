// CGameScriptInterface::GetDeathRecoveryMarkerName() const -- the hero's death-recovery marker name (hero-stats component), or "" without a live hero holding it
// Leaf: ?GetDeathRecoveryMarkerName@CGameScriptInterface@@UBE?AVCCharString@@XZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00896EE0, 125 bytes, `ret 4` (hidden result); Ego_r.pdb same symbol.
// Body: hero = PlayerManager (+0x14)->GetMainPlayer() (0x00449970)->PeekPControlledCreature() (0x00487DD0, Ego_r.pdb order Get 0x00493650 then Peek 0x00493660: `add ecx,2Ch;
// jmp` into CPlayer::ControlledCreature +0x2c, Ego_r.pdb CIntelligentPointer<CThingPlayerCreature> at 44); if hero, not dead
// and holding interface 4 (bit 4 of UsingComponents +0x20): PeekTC (const LowerBound 0x004365B0) and return a copy
// (0x0099EC30) of CTCHeroStats::DeathRecoveryMarkerName (+0x170); else return CCharString("", -1) (0x0099EBF0, literal
// 0x0122D70E).
// Layout: Ego_r.pdb CTCHeroStats DeathRecoveryMarkerName +0x188; retail has it at +0x170 (-0x18, the only struct with that
// member name). Interface 4 = CTCHeroStats rests on that member; the CCharString(const char*, long) length argument
// meaning (-1 = strlen) is as the landed C2DVector::ToString row.
// Shape: /O2 (16-aligned entry); the PeekTC out-pointer form.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCHeroStats {                                    // interface 4
public:
    unsigned char _pad000[0x170];
    CCharString   DeathRecoveryMarkerName;               // +0x170 (Ego_r.pdb +0x188)
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
CCharString CGameScriptInterface::GetDeathRecoveryMarkerName() const
{
    const CThing* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (hero && !hero->IsDead())
    {
        const const CTCHeroStats* stats;
        if (hero->PeekTC(TCI_HERO_STATS, &stats))
            return stats->DeathRecoveryMarkerName;
    }
    return CCharString("", -1);
}