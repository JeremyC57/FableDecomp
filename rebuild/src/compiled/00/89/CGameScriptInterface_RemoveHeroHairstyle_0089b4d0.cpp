// CGameScriptInterface::RemoveHeroHairstyle() const -- removes the hero's hairstyle appearance modifiers
// Leaf: RemoveHeroHairstyle@CGameScriptInterface
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x0089B4D0, 91 bytes. Calls CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
// then CThing::GetTC<CTCHeroAttachableAppearanceModifiers> (Ego_r.pdb bool/out-pointer form; Components lower_bound 0x0040F020); CTCHeroAttachableAppearanceModifiers::RemoveAllAppearanceModifiers 0x00706640.
// Layout: CGameScriptInterface per engine/CGameScriptInterface.h (+0x14 PlayerManager); CThing retail (UsingComponents +0x20,
// Components +0x44, Dead +0x91 bit 0).
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
enum EHeroAttachableAppearanceModifierType { HERO_APPEARANCE_MODIFIER_HAIRSTYLE = 0, HERO_APPEARANCE_MODIFIER_FORCE_DWORD = 0x7fffffff };
class CTCHeroAttachableAppearanceModifiers {
public:
    void RemoveAllAppearanceModifiers(EHeroAttachableAppearanceModifierType type);   // 0x00706640
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::RemoveHeroHairstyle() const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCHeroAttachableAppearanceModifiers* tc;
    if (hero && !hero->IsDead() && hero->GetTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &tc))
        tc->RemoveAllAppearanceModifiers(HERO_APPEARANCE_MODIFIER_HAIRSTYLE);
}