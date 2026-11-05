// CGameScriptInterface::SetPlayerCreatureOnlyTarget(const CScriptThing& target) const -- make a valid thing the hero creature's only target (interface 8 component)
// Leaf: ?SetPlayerCreatureOnlyTarget@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00899E10, 123 bytes, `ret 4`; Ego_r.pdb same symbol. Twin of the matched 0x00899E90 Reset form.
// Body: hero = Component (+0x08)->PPlayerManager (+0x1c)->GetMainPlayer() (0x00449970)->GetPControlledCreature() (0x00487DC0);
// if target.IsAlive() (slot 75), hero and not dead (bit 0 of +0x91): t = target.GetPThing() (slot 11, before the TC test);
// if the hero holds interface 8 (bit 8 of UsingComponents +0x20), its component (LowerBound 0x0040F020, key in the dead
// `target` slot) gets 0x008ABC90(t).
// Layout: as 0x00899E90. The interface-8 class and the 0x008ABC90 name are not confirmed.
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
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::SetPlayerCreatureOnlyTarget(const CScriptThing& target) const
{
    CThing* hero = Component->PPlayerManager->GetMainPlayer().GetPControlledCreature();
    if (target.IsAlive() && hero && !hero->IsDead())
    {
        CThing* t = target.GetPThing();
        CTCTargetingBase* tc;
        if (hero->GetTC(TCI_TARGETING, &tc))
            tc->SetOnlyTarget(t);
    }
}