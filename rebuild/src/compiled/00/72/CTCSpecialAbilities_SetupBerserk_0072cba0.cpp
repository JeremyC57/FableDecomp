// CTCSpecialAbilities::SetupBerserk() -- start the strength-berserk cast on the creature; when its primary-slot weapon is the item it holds in hand, first sheathe it with the berserk cast queued as the follow-up action; false when the creature is barred (flag +0x30 bit 4)
// Leaf: ?SetupBerserk@CTCSpecialAbilities@@AAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072CBA0, 352 bytes (`ret`).
// Callees: CVectorMap::LowerBound const 0x004365B0 (CTCCarrying 0x46, interface 0x13); CTCCarrying::GetThingInPrimarySlot
//          0x006A1440; CBaseIntelligentPointer::GetPItem 0x00A01B50; operator new 0x00BFEA1A; CCreatureAction_CastStrengthBerserk
//          ctor 0x008E6880 (vtable 0x01287384; catalog "CCreatureAction_BattleChargeTurnLeft", a twin) and dtor 0x0062E900;
//          CIntelligentPointer<CThing>(CThing*) 0x004A96F0 (vtable 0x01238C6C, RTTI CIntelligentPointer<CThing>); the berserk
//          action's item setter 0x0072D360 (by-value
//          intelligent pointer; catalog "CTCInventoryBase::GetItemDescription", a twin; descriptive name);
//          CCreatureAction_SheatheItemToInventory ctor 0x0069EF80 / dtor 0x0048C950 (catalog twin label); global CCharString
//          0x013BAD30 (descriptive name); CThingCreatureBase::SetCurrentAction 0x006644F0.
// Layout: retail CTCSpecialAbilities::Creature +0x18 (PDB +0x20); CThing interface bitset +0x20, flags +0x30, map +0x44; the
//          interface-0x13 component's held-item intelligent pointer at +0x141 (descriptive); the weapon's flag byte +0x91.
// Shape: the barred test wraps the body (the `xor al,al` arm sits last); the default cast is a temporary of the
//          SetCurrentAction call, the sheathe action a named local; both lookups are the ternary GetInterface (this in ebp).
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013BAD30 SHEATH_ACTION_NAME (anonymous namespace; initializer 0x0121ECE0), was g_sheatheForAbilityName.
// Port (NF98, round 8, q11261 / q11262): the local CThing / CBaseIntelligentPointer / CIntelligentPointer / CTCCarrying / CTCHeldItem /
//   CCreatureActionBase / CCreatureAction_CastStrengthBerserk / CCharString / CCreatureAction_SheatheItemToInventory / CThingCreatureBase
//   models replaced by the shared headers (the SetupBattleCharge 0x0072C9C0 recipe): SetCurrentAction 0x006644F0 and the temporary's
//   destructor 0x0062E900 (~CCombatActionBase) bind the PDB spellings; the berserk action is the retail class on CCombatActionBase (NF98
//   opt-in FABLE_RETAIL_CCREATUREACTION_CASTSTRENGTHBERSERK_CLASS, ctor (CThingCreatureBase&) 0x008E6880); the barred test is
//   HasTC(TCI_BERSERK) (bit 132: +0x30 bit 4); the interface-0x13 component is CTCInventoryWeapons and its +0x141 pointer
//   PActiveRangedWeapon read through FableWin's inline GetActiveRangedWeapon (NF98 opt-in FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVERANGED);
//   the sheathe action is engine/CCreatureAction_SheatheItemToInventory.h (4-argument ctor 0x0069EF80, virtual dtor 0x0048C950); the item
//   setter is ?SetPItemRemoved@CCreatureActionBase@@QAEXV?$CIntelligentPointer@VCThing@@@@@Z (0x0072D360) with the out-of-line
//   CIntelligentPointer<CThing>(CThing*) 0x004A96F0. Own leaf: Ego_r.pdb / FableWin.pdb private ?SetupBerserk@CTCSpecialAbilities@@AAE_NXZ
//   (the landed caller 0x0072CF40 binds it). CTCSpecialAbilities stays row-local as in its landed siblings (0x0072C9C0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_SETPITEMREMOVED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_CASTSTRENGTHBERSERK_CLASS
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVEMELEE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVERANGED
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
#include "engine/CTCCarrying.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_CastStrengthBerserk.h"
#include "engine/CCreatureAction_SheatheItemToInventory.h"
namespace { extern CCharString SHEATH_ACTION_NAME; }   // 0x013BAD30
class CTCSpecialAbilities : public CTCBase {
private:
    bool SetupBerserk();
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    unsigned char UndeadFollowers[4];           // +0x14 list<CIntelligentPointer<CThingCreatureBase> >
    CThingCreatureBase* Creature;               // +0x18 (Ego_r.pdb +0x20)
};

bool CTCSpecialAbilities::SetupBerserk()
{
    if (!Creature->HasTC(TCI_BERSERK)) {
        const CTCCarrying* carrying;
        if (Creature->PeekTC(&carrying)) {
            CThing* weapon = carrying->GetThingInPrimarySlot();
            if (weapon && !weapon->IsDead()) {
                const CTCInventoryWeapons* weapons;
                Creature->PeekTC(&weapons);
                CThing* ranged = weapons->GetActiveRangedWeapon();
                if (weapon == ranged) {
                    CCreatureAction_CastStrengthBerserk* berserk = new CCreatureAction_CastStrengthBerserk(*Creature);
                    berserk->SetPItemRemoved(ranged);
                    CCreatureAction_SheatheItemToInventory action(*Creature, SHEATH_ACTION_NAME, berserk, 0);
                    return Creature->SetCurrentAction(action);
                }
            }
        }
        return Creature->SetCurrentAction(CCreatureAction_CastStrengthBerserk(*Creature));
    }
    return false;
}
