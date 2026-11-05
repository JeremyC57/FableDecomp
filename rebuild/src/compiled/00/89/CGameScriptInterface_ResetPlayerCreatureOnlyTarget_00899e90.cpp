// CGameScriptInterface::ResetPlayerCreatureOnlyTarget() const -- clear the hero creature's only-target (interface 8 component)
// Leaf: ?ResetPlayerCreatureOnlyTarget@CGameScriptInterface@@UBEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00899E90, 93 bytes, plain `ret`; Ego_r.pdb ?ResetPlayerCreatureOnlyTarget@CGameScriptInterface@@UBEXXZ.
// Body: hero = Component (+0x08)->PPlayerManager (+0x1c)->GetMainPlayer() (0x00449970)->GetPControlledCreature() (0x00487DC0);
// if hero, not dead (bit 0 of +0x91) and holding interface 8 (bit 8 of UsingComponents +0x20), the component from the
// Components map (LowerBound 0x0040F020, key in the `push ecx` slot) gets 0x008ABC90(NULL).
// Layout: CThing as the matched 0x0089AFF0 / 0x00897AA0 rows; CMainGameComponent PPlayerManager +0x1c (Ego_r.pdb, retail
// agrees below +0x24). The interface-8 component (modelled CTCTargetingBase at 0x0089D8B0) and the callee name
// (0x008ABC90, takes one pointer and wraps it in a counted handle) are not confirmed.
// Shape: /O2 (16-aligned entry); the GetTC out-pointer form.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCTargetingBase {                                // interface 8 (class not confirmed)
public:
    void SetOnlyTarget(CThing* target);                  // 0x008ABC90 (name not confirmed)
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMainGameComponent {
public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                      // +0x1c (scoped_ptr)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::ResetPlayerCreatureOnlyTarget() const
{
    CThing* hero = Component->PPlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        CTCTargetingBase* tc;
        if (hero->GetTC(TCI_TARGETING, &tc))
            tc->SetOnlyTarget(0);
    }
}