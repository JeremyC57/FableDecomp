// CGameScriptInterface::EntityUnsheatheMeleeWeapon(const CScriptThing& thing, bool instant) const -- for a live creature that carries no melee weapon: an AI creature finishes its current action and draws its melee weapon (at once through the combat manager when `instant`); the player character's draw is queued as a controlled-creature game event instead
// Leaf: ?EntityUnsheatheMeleeWeapon@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00890C80, 242 bytes, `ret 8`, /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
//   CGameScriptInterface vtable slot 508 (engine/CGameScriptInterface.h); Ego_r.pdb / FableWin.pdb
//   ?EntityUnsheatheMeleeWeapon@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z. The melee twin of 0x00890D80 (same shape, same length).
// Callees (PDB spellings): CScriptThing::IsAlive (slot 75, +0x12C) / GetPThing (slot 11, +0x2C);
//   ?IsThingCarryingMeleeWeapon@CTCCarrying@@SI_NABVCThing@@@Z @ 0x006A15A0 (Ego_r.pdb / FableWin.pdb, public static fastcall; the thing in
//   ecx; engine/CTCCarrying.h ISTHINGCARRYINGMELEEWEAPON, as the witness 0x008E4100); the creature's slot 77 IsPlayerCharacter (+0x134;
//   engine/CThingCreatureBase.h ORIGINAL_VTABLE); ?FinishCurrentAction@CThingCreatureBase@@QAEXXZ @ 0x00662650;
//   ?UnsheatheMeleeWeapon@CThingCreatureBase@@QAEXXZ @ 0x006649C0 (three PDBs); the inline CWorld::GetCombatManager (FableWin.pdb; World +0x04,
//   PThingCombatManager +0x4C) and ?CreatureUnsheathWeapon@CCombatManager@@QAEXAAVCThingCreatureBase@@W4EUnsheatheWeaponType@@@Z @ 0x006EBDD0
//   (UNSHEATHE_MELEE_WEAPON = 4); ?AddData@CGameEvent@@QAEXJPBX@Z @ 0x009F1760; the inline CMainGameComponent::Get() (protected static
//   PMainGameComponent 0x013B86A0) and ?AddGameEvent@CMainGameComponent@@QAEXABVCGameEvent@@@Z @ 0x0041649C.
// Layout: CThing ThingTypeFlags +0x6C (bit 8: a creature), PlayerNumber +0x90; CGameEvent (Ego_r.pdb, 40 bytes) Type +0, Player +4,
//   EndPos +0x25, Valid +0x26, Replacement +0x27; event type GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_MELEE_WEAPON = 33 (FableWin TPI).
// Shape: no null test on the thing (retail reads +0x6C straight after the slot-11 call); the event's bool payload is homed in the dead
//   `instant` argument slot.
// NF253 (q12711): ported off the HEAD local models (the local CThingNative with a member-pointer slot-77 call and its own
//   UnsheatheMeleeWeapon, the local ScriptMethods / CBaseClassNative / CCPPointerInfo / CountedScriptImp models of CScriptThing, the local
//   ComponentMap / GameEventMethods / CCombatManager and the stand-in CGameScriptInterfaceMethods) onto the shared headers, the 0x00890D80
//   recipe (NF249); the self leaf, 0x006A15A0, 0x006649C0 and 0x006EBDD0 now bind their PDB spellings (were
//   ?EntityUnsheatheMeleeWeapon@CGameScriptInterfaceMethods@@QBEXABUScriptMethods@@_N@Z, ?IsThingCarryingMeleeWeapon@@YI_NPAUCThingNative@@@Z,
//   ?UnsheatheMeleeWeapon@CThingNative@@QAEXXZ, ?CreatureUnsheathWeapon@CCombatManager@@QAEXPAUCThingNative@@J@Z).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHEMELEEWEAPON
#define FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_UNSHEATH
#define FABLE_RETAIL_COMBAT_INPUT_PASS26
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_NSCRIPT
#include "engine/CThingCreatureBase.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCCarrying.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCombatManager.h"
#include "engine/CWorld.h"
#include "engine/EGameEventType.h"
#include "engine/CGameEvent.h"
#include "engine/CMainGameComponent.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntityUnsheatheMeleeWeapon(const CScriptThing& thing, bool instant) const
{
    if (thing.IsAlive()) {
        CThing* pthing = thing.GetPThing();
        if (pthing->GetTypeFlags() & 8) {
            if (!CTCCarrying::IsThingCarryingMeleeWeapon(*pthing)) {
                CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(pthing);
                if (!creature->IsPlayerCharacter()) {
                    creature->FinishCurrentAction();
                    if (!instant)
                        creature->UnsheatheMeleeWeapon();
                    else
                        World->GetCombatManager().CreatureUnsheathWeapon(*creature, UNSHEATHE_MELEE_WEAPON);
                } else {
                    CGameEvent event(GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_MELEE_WEAPON);
                    event.SetPlayer(creature->GetPlayerNumber());
                    instant = false;
                    event.AddData(1, &instant);
                    CMainGameComponent::Get()->AddGameEvent(event);
                }
            }
        }
    }
}
