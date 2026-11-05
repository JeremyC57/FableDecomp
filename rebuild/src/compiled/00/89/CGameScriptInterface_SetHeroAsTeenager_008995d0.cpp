// CGameScriptInterface::SetHeroAsTeenager(bool) const -- switch the live hero's morph to (or from) the teenage look and, with hero stats present, reset its age and training state
// Leaf: ?SetHeroAsTeenager@CGameScriptInterface@@UBEX_N@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x008995D0, 176 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: hero = PlayerManager (+0x14)->GetMainPlayer() (0x00449970).GetPControlledCreature() (0x00487DC0); if hero and not
// Dead: GetTC<interface 3>(&morph) (LowerBound 0x0040F020; the result is used unchecked -- when the bit is clear retail
// reads the uninitialised local, which shares the argument slot) ->SetAsTeenager(teenager) (0x0071BA40); if
// GetTC<interface 4>(&stats): stats->SetAge(1.0f) (0x0057B26F) and stats->ResetTrainingStatus() (0x0057A5BE).
// Names: CTCHeroMorph::SetAsTeenager(bool) is an Ego_r.pdb symbol; interface 4 is modelled as CTCHeroStats (its code sits
// beside CTCHeroStats::GetMoralityCategory 0x0057A6B8) and both of its method names are guesses among the PDB's
// CTCHeroStats QAEXM / QAEXXZ members.
// Layout: CThing as the matched 0x00899B10 row (UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0).
// Shape: /O2 (16-aligned entry); GetTC uses the one-ternary component store (the 0x00899B10 idiom).
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCHeroMorph {                                    // interface 3
public:
    enum { INTERFACE_TYPE = 3 };
    void SetAsTeenager(bool teenager);                   // 0x0071BA40 (Ego_r.pdb ?SetAsTeenager@CTCHeroMorph@@QAEX_N@Z)
};
class CTCHeroStats {                                    // interface 4
public:
    enum { INTERFACE_TYPE = 4 };
    void SetAge_0057b26f(float age);                              // 0x0057B26F (clamped to [0, 1]; name not confirmed)
    void ResetTrainingStatus_0057a5be();                          // 0x0057A5BE (name not confirmed)
};
class CThingPlayerCreature : public CThing { };
class CPlayer : public CPlayerPdb { public:
    CThingPlayerCreature* GetPControlledCreature() const; // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::SetHeroAsTeenager(bool teenager) const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        CTCHeroMorph* morph;
        hero->GetTC(TCI_APPEARANCE_MORPH, &morph);
        morph->SetAsTeenager(teenager);
        CTCHeroStats* stats;
        if (hero->GetTC(TCI_HERO_STATS, &stats))
        {
            stats->SetAge_0057b26f(1.0f);
            stats->ResetTrainingStatus_0057a5be();
        }
    }
}