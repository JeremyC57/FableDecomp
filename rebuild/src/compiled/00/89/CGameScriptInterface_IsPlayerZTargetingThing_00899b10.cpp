// CGameScriptInterface::IsPlayerZTargetingThing(const CScriptThing&) const -- is the main player z-targeting, with its live hero's targeting component aimed at this live thing
// Leaf: ?IsPlayerZTargetingThing@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00899B10, 140 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: player = PlayerManager (+0x14)->GetMainPlayer() (0x00449970); target = thing.GetPThing() (CScriptThing slot 11,
// +0x2c); if player.ZTargeting (+0x20a) and target alive (Dead +0x91 bit 0) and hero = player.GetPControlledCreature()
// (0x00487DC0, `add ecx,2Ch; jmp`, CPlayer::ControlledCreature +0x2c) alive and holding interface 8: PeekTC (const
// LowerBound 0x004365B0) and compare its slot 33 (+0x84) with target.
// Layout: engine/CPlayer.h ZTargeting +0x20a (Ego_r.pdb, no shift); CThing as the matched 0x00899E90 row; the interface-8
// class (modelled CTCTargetingBase, as 0x0089A490) and the slot-33 name are not confirmed. CScriptThing is declared
// locally with its virtuals (engine/CScriptThing.h flattens the vptr).
// Shape: /O2 (16-aligned entry); the PeekTC out-pointer form, with the end-or-mismatch choice written as one ternary
// whose ->component is stored (`*out = ((it == last || 8 < it->type) ? last : it)->component`): the if/assign form puts
// the component in eax and copies it to ecx (142 bytes); the ternary loads it straight into ecx as retail does.
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCTargetingBase {                                // interface 8 (class not confirmed)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTarget() const;                   // slot 33 (+0x84, name not confirmed)
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
bool CGameScriptInterface::IsPlayerZTargetingThing(const CScriptThing& thing) const
{
    CPlayer& player = PlayerManager->GetMainPlayer();
    CThing* target = thing.GetPThing();
    if (player.ZTargeting && target && !target->IsDead())
    {
        CThing* hero = player.GetPControlledCreature();
        if (hero && !hero->IsDead())
        {
            const const CTCTargetingBase* tc;
            if (hero->PeekTC(TCI_TARGETING, &tc) && tc->GetTarget() == target)
                return true;
        }
    }
    return false;
}