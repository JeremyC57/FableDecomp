// CGameScriptInterface::SetAbilityAvailability(EHeroAbility, bool) const -- makes a hero special ability available or not
// Leaf: SetAbilityAvailability@CGameScriptInterface
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x0089D7C0, 99 bytes. Calls CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
// then CThing::GetTC<CTCSpecialAbilities> (Ego_r.pdb bool/out-pointer form; Components lower_bound 0x0040F020).
// Layout: CGameScriptInterface per engine/CGameScriptInterface.h (+0x14 PlayerManager); CThing retail (UsingComponents +0x20,
// Components +0x44, Dead +0x91 bit 0).
// CTCSpecialAbilities (interface 0xa7) retail: AbilityAvailability +0x1c (Ego_r.pdb +0x24 after a 12-byte list; retail list 4 bytes).
// Retail stores a byte per ability, so the vector is modelled over bool elements.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;       // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;      // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
enum EHeroAbility { HERO_ABILITY_FORCE_DWORD = 0x7fffffff };
template <class T> struct CRetailVector {                // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T& operator[](unsigned long n) { return _M_start[n]; }
};
class CTCSpecialAbilities {
public:
    unsigned char       _pad00[0x1c];
    CRetailVector<bool> AbilityAvailability;             // +0x1c
    void SetAbilityAvailable(EHeroAbility ability, bool available) { AbilityAvailability[ability] = available; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetAbilityAvailability(EHeroAbility ability, bool available) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCSpecialAbilities* tc;
    if (hero && !hero->IsDead() && hero->GetTC(TCI_SPECIAL_ABILITIES, &tc))
        tc->SetAbilityAvailable(ability, available);
}