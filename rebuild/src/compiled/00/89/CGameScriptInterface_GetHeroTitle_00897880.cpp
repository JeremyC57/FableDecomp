// CGameScriptInterface::GetHeroTitle() const -- script call: the main player's hero's title (CTCHero::GetTitle), 0 without a live hero holding the hero component
// Leaf: ?GetHeroTitle@CGameScriptInterface@@UBE?AW4EHeroTitle@@XZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00897880, 88 bytes, thiscall `ret`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 205 holds this body; slot 205 is
//   ?GetHeroTitle@CGameScriptInterface@@UBE?AW4EHeroTitle@@XZ in ghidra_out/cgsi_slots_aligned.tsv.
//   Getter: FableWin.pdb ?GetTitle@CTCHero@@QBE?AW4EHeroTitle@@XZ, inline (+0x10). Interface 0x29 = CTCHero (landed
//   CTCHero::GetInterfaceType 0x006CA6F0 returns 0x29).
// Body: PlayerManager (+0x14)->GetMainPlayer() (0x00449970)->GetPControlledCreature() (0x00487DC0); live hero holding
//   interface 0x29 (UsingComponents word 1 bit 9, `test ch,2`): GetTC (non-const LowerBound 0x0040F020), return +0x10.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;    // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
enum EHeroTitle { EHERO_TITLE_NONE_PLACEHOLDER = 0 };
class CTCHero {                                         // interface 0x29
public:
    unsigned char _pad00[0x10];
    EHeroTitle Title;                                    // +0x10
    EHeroTitle GetTitle() const { return Title; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
EHeroTitle CGameScriptInterface::GetHeroTitle() const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        CTCHero* tc;
        if (hero->GetTC(TCI_HERO, &tc))
            return tc->GetTitle();
    }
    return (EHeroTitle)0;
}