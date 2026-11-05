// CGameScriptInterface::GetBestScoreCoinGolfOakVale() const -- the hero's best CoinGolfOakVale score (CTCHeroStats::BestScoreCoinGolfOakVale truncated to long), 0 when the main player's creature is missing or dead
// Leaf: ?GetBestScoreCoinGolfOakVale@CGameScriptInterface@@UBEJXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008a16b0, 94 bytes, thiscall, `ret`.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 733; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order) names it ?GetBestScoreCoinGolfOakVale@CGameScriptInterface@@UBEJXZ; it reads Ego_r.pdb CTCHeroStats::BestScoreCoinGolfOakVale (PDB +412, retail +0x184) through interface 4 (CTCHeroStats in the landed GetInterface(&stats, 4) callers).
// Callees: CPlayerManager::GetMainPlayer() 0x00449970 (non-const: it calls the non-const GetPlayer 0x004498C0), CPlayer::PeekPControlledCreature() const 0x00487DD0 (Ego_r.pdb, a const
//   CThingPlayerCreature*), the const CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x004365B0 (the inlined const
//   GetInterface), __ftol2 0x00BFEA70.
// Body: alive test (non-null, Dead bit 0 of +0x91 clear), presence bit 4 of the interface bitset (`test byte ptr [eax+20h],10h`).
// Header debt: engine/CGameScriptInterface.h, CPlayer, CThing and engine/CTCHeroStats.h are flat layouts (PDB offsets); modelled locally.
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCHeroStats {
public:
    unsigned char _pad00[0x184];
    float         BestScoreCoinGolfOakVale;                                   // +0x184
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
long CGameScriptInterface::GetBestScoreCoinGolfOakVale() const
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    const const CTCHeroStats* stats;
    if (hero && !hero->IsDead() && hero->PeekTC(TCI_HERO_STATS, &stats))
        return (long)stats->BestScoreCoinGolfOakVale;
    return 0;
}