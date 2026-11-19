// Leaf: ?SetupEnflameSpell@CTCSpecialAbilities@@AAE_NXZ
// CTCSpecialAbilities::SetupEnflameSpell() -- start the creature's enflame-spell cast: target the creature's current combat target (with the player's lock flag when both are player-flagged), and when a primary-slot weapon is alive wrap a heap cast action holding the weapon in a generic strike response; returns whether the action was accepted
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +4.
// Retail: Fable.exe @ 0x0072C790, 416 bytes (plain `ret`, no EH frame).
// Callees: LowerBound 0x0040F020 / 0x004365B0 (const), CTCInventory::GetThingInPrimarySlot 0x006A1440, combat TC virtual
//   slot 33 (+0x84, current target), CThing::PeekPlayer 0x004C7A10, operator new 0x00BFEA1A,
//   CCreatureAction_CastWillEnflameSpell ctor 0x008BB500 / ~CCombatActionBase 0x0062E900, CBaseIntelligentPointer ctor
//   0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10, CCombatAction_GenericStrikeResponse ctor
//   0x0069F060 / dtor 0x0072D390, CThingCreatureBase::SetCurrentAction 0x006644F0.
// Layout (local model): CTCSpecialAbilities creature +0x18; CThing TC bitset +0x20 (8 = +0x21 bit 0, 0x46 = +0x28 bit 6),
//   interface map +0x44, flags +0x6c (8 / 0x20), Dead +0x91 bit 0; CPlayer lock flag +0x20a; the cast action's weapon
//   pointer +0x2c; g_13bad30 the strike-response name.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013BAD30 SHEATH_ACTION_NAME (anonymous namespace; initializer 0x0121ECE0), was g_StrikeResponseName.
// Port (NF98, round 8, q11261 / q11262): the local CPlayer / CBaseIntelligentPointer / CIntelligentPointer / CThing / CTCInventory / CTCCombat /
//   CCombatActionBase / CCreatureAction_CastWillEnflameSpell / CCharString / CCombatAction_GenericStrikeResponse / CThingCreatureBase models
//   replaced by the shared headers (the SetupBattleCharge 0x0072C9C0 / SetupBerserk 0x0072CBA0 recipe): SetCurrentAction 0x006644F0 and the
//   temporary's destructor 0x0062E900 (~CCombatActionBase) bind the PDB spellings. Identities read from the callees: the interface-0x46
//   component is CTCCarrying (?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ 0x006A1440, was a CTCInventory stand-in); the
//   interface-8 component is CTCTargetingBase (slot 33 GetPTargetedThing, was a CTCCombat::GetCurrentTarget stand-in); the +0x20A byte
//   is CPlayer::IsZTargeting (FableWin inline); the response action is CCreatureAction_SheatheItemToInventory_ForEnflame (ctor 0x0069F060
//   stores vtable 0x0125C95C, RTTI; NF98 _CLASS opt-in), was a CCombatAction_GenericStrikeResponse stand-in, and its destructor 0x0072D390
//   the implicit COMDAT; the cast action takes Ego_r.pdb's (creature, target, z-targeting, level) ctor 0x008BB500 (NF98 opt-in
//   _TARGETCTOR); the +0x2C store is CCreatureActionBase::SetPItemRemoved expanded in place (NF98 per-TU opt-in
//   FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED_BODY). Own leaf: the private PDB spelling (the landed caller 0x0072CF40 binds it).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED_BODY
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_CASTWILLENFLAMESPELL_CLASS
#define FABLE_RETAIL_CCREATUREACTION_CASTWILLENFLAMESPELL_TARGETCTOR
#define FABLE_RETAIL_CCREATUREACTION_SHEATHEITEMTOINVENTORY_FORENFLAME_CLASS
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CPlayer.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_CastWillEnflameSpell.h"
#include "engine/CCreatureAction_SheatheItemToInventory.h"
#include "engine/CCreatureAction_SheatheItemToInventory_ForEnflame.h"
namespace { extern CCharString SHEATH_ACTION_NAME; }         // 0x013BAD30
class CTCSpecialAbilities : public CTCBase {
private:
    bool SetupEnflameSpell();
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    unsigned char UndeadFollowers[4];           // +0x14 list<CIntelligentPointer<CThingCreatureBase> >
    CThingCreatureBase* Creature;               // +0x18 (Ego_r.pdb +0x20)
};

bool CTCSpecialAbilities::SetupEnflameSpell()
{
    CTCCarrying* carrying;
    Creature->GetTC(&carrying);
    CThing* weapon = carrying->GetThingInPrimarySlot();
    CThing* target = 0;
    bool zTargeting = false;
    const CTCTargetingBase* targeting;
    if (Creature->PeekTC(&targeting)) {
        target = targeting->GetPTargetedThing();
        if (target && !target->IsDead() && (target->GetTypeFlags() & 8) && (Creature->GetTypeFlags() & 0x20))
            zTargeting = Creature->PeekPlayer().IsZTargeting();
    }
    if (weapon && !weapon->IsDead())
    {
        CCreatureAction_CastWillEnflameSpell* action = new CCreatureAction_CastWillEnflameSpell(*Creature, target, zTargeting, 0);
        action->SetPItemRemoved(weapon);
        CCreatureAction_SheatheItemToInventory_ForEnflame response(*Creature, SHEATH_ACTION_NAME, action, 0);
        return Creature->SetCurrentAction(response);
    }
    return Creature->SetCurrentAction(CCreatureAction_CastWillEnflameSpell(*Creature, target, zTargeting, 0));
}
