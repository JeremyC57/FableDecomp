// CGameScriptInterface::SetHeroAsWearing(const CCharString& defName) const -- dress the live hero creature in the named clothing through its CTCInventoryClothing (interface 0x12)
// Leaf: ?SetHeroAsWearing@CGameScriptInterface@@UBEXABVCCharString@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089B3F0, thiscall, virtual.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 190; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb
//   vtable order) names it ?SetHeroAsWearing@CGameScriptInterface@@UBEXABVCCharString@@@Z.
// Guard: the main player's controlled creature exists and is not dead; the component is CTCInventoryClothing (interface 0x12: GetInterfaceType of its RTTI vtable, or its Ego_r.pdb method).
// Callees: CPlayerManager::GetMainPlayer() 0x00449970 (non-const; 0x00449960 is the const one), CPlayer::GetPControlledCreature() const 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound(const ETCInterfaceType&) 0x0040F020 (the inlined CThing::GetInterface), CTCInventoryClothing::SetAsWearing(const CCharString&) 0x005B413D (Ego_r.pdb, landed).
// Shape: the landed L083/L084 GetInterface model (2026-10-27_linux_hand955); the lookup key is homed in the `push ecx` slot.
// Layout: local models -- engine/CGameScriptInterface.h carries its vptr as a data member and the CThing/CScriptThing/TC headers are flat
//   Ego_r layouts (Dinkumware offsets); component offsets are retail (Ego_r minus 4 per STLport container before the member).
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString;
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;  // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCInventoryClothing {
public:
    void SetAsWearing(const CCharString& defName);         // 0x005B413D
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::SetHeroAsWearing(const CCharString& defName) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCInventoryClothing* clothing;
    if (hero && !hero->IsDead() && hero->GetTC(TCI_INVENTORY_CLOTHING, &clothing))
        clothing->SetAsWearing(defName);
}