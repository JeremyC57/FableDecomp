// CCreatureAction_TrollThrowProjectile::Initialise() -- name the animation PROJECTILE_ATTACK_THROW_ROCK and run the combat base
//   Initialise; create the object of the shot def the creature's weapon def names at the creature's position, keep it in PShot and
//   (when the creature can carry) put it in the CARRY_SLOT_TROLL_THROW_ROCK(_LEFT) slot by the animation's start handedness, hidden
//   (ShotAlpha 0); then roll 0..99 against the creature's troll battle def PercentageChanceToUseSecondaryShotType and set the combat
//   component's ShotOverrideType to its SecondaryShotType
// Leaf: ?Initialise@CCreatureAction_TrollThrowProjectile@@UAEXXZ
// Flags: none
// Inferred: the handedness component pointer lives in an inner block that ends before the slot-name temporaries -- retail shares one
//   frame word between it and the CCharString temporary (0x008C6638 `lea eax,[esp+1Ch]` out-pointer, 0x008C664C `lea ecx,[esp+1Ch]` string,
//   frame 0x18); declared at the GetTC block scope the out-pointer keeps its own word (graded DIFFER 682v682, frame 0x1C). The named
//   start_handedness carries the value out of the block; ruled out: nothing else changes the frame.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C64F0, 688 bytes, __thiscall virtual (vtable 0x01281694), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CCombatActionBase::SetCombatAnimation 0x008574D0 and
//   Initialise 0x00857520 (qualified); CBaseIntelligentPointer::GetPItem 0x00A01B50; CDefClassBase::GetSubDef<CWeaponDef> 0x0042B1A6 and
//   <CTrollBattleDef> 0x008C78F0 (out of line); CCreatureActionBase::GetWorld 0x00692940 / PeekDefinitionManager 0x00692910;
//   CDefinitionManager::GetDef<CThingShotDef> 0x0078B230; CThing::GetPos 0x004C73D0; CThingObject::Create 0x00703210 (static
//   __fastcall); CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (the inline CIntelligentPointer<CThing>
//   temporary and assignment); CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound 0x0040F020 (the expanded GetTC<CTCCarrying> /
//   <CTCCombat>); CAnimationEntry::GetComponent<CAnimComponentCombatHandedness> 0x00669CB0; CDefinitionManager::GetDefGlobalIndexFromName
//   0x009AD410; CTCCarrying::PutThingInCarrySlot 0x006A1F80; NGraphicAppearance::CTCGraphicAppearance::SetAlpha 0x004BFAC0;
//   CWorld::GetWorldSeed 0x0049D810; GFROR13 0x00497890 (the inline GFRandom).
// Port (NF101, 2026-11-19, q11260): moved off its 23-class local model (stand-in GetSubDef / GetDef / GetWeaponHand bodies) onto the
//   shared headers (0x008C6860 define set); Create now binds the PDB
//   ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z (was a by-pointer CThingObject::Create stand-in).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTHINGSHOTDEF_DEFSTRING
#define FABLE_RETAIL_CTROLLBATTLEDEF_MEMBERS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_ANIMCOMPONENT
#define FABLE_RETAIL_CANIMATIONENTRY_GETCOMPONENT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCREATUREACTION_TROLLTHROWPROJECTILE
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_SHOTOVERRIDE
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETALPHA
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#include "rebuild_abi.h"
#include "fable_string.h"
#include "fable_definition_table.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/ThingAlive.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingObject.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCombat.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingShotDef.h"
#include "engine/CTrollBattleDef.h"
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CWEAPONDEF_SIZE_BODY   // NF357: GetSubDef<CWeaponDef> pick-any copy = size-mode owner 0x0042B1A6 (scaffolding, not retail)
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimComponentCombatHandedness.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_TrollThrowProjectile.h"

void CCreatureAction_TrollThrowProjectile::Initialise()
{
    SetCombatAnimation("PROJECTILE_ATTACK_THROW_ROCK");
    CCombatActionBase::Initialise();
    CDefPointer<const CWeaponDef> pweapon_def;
    PCreature->PeekBaseDef().GetSubDef(&pweapon_def);
    CDefPointer<const CThingShotDef> pshot_def;
    GetWorld().PeekDefinitionManager().GetDef(pweapon_def->Shot, &pshot_def);
    CThingObject* pobject = CThingObject::Create(pshot_def->ObjectDef, PCreature->GetPos(), PCreature->GetPlayerNumber(), 0, 0, "");
    if (pobject) {
        PShot = CIntelligentPointer<CThing>(pobject);
        CTCCarrying* ptc_carrying;
        if (PCreature->GetTC(&ptc_carrying)) {
            long start_handedness;
            {
                const CAnimComponentCombatHandedness* phandedness;
                PAnimEntry->GetComponent(&phandedness);
                start_handedness = phandedness->StartHandedness;
            }
            long slot;
            if (start_handedness == 1)
                slot = PeekDefinitionManager().GetDefGlobalIndexFromName("CARRY_SLOT_TROLL_THROW_ROCK_LEFT");
            else
                slot = PeekDefinitionManager().GetDefGlobalIndexFromName("CARRY_SLOT_TROLL_THROW_ROCK");
            ptc_carrying->PutThingInCarrySlot(*pobject, slot, true);
        }
        ShotAlpha = 0;
        pobject->GetTCGraphicAppearance()->SetAlpha((unsigned char)ShotAlpha);
    }
    CDefPointer<const CTrollBattleDef> ptroll_battle_def;
    if (PCreature->PeekBaseDef().GetSubDef(&ptroll_battle_def)) {
        unsigned long chance = GFRandom(100, GetWorld().GetWorldSeed());
        CTCCombat* ptc_combat;
        if ((float)chance < ptroll_battle_def->PercentageChanceToUseSecondaryShotType && PCreature->GetTC(&ptc_combat)
            && ptroll_battle_def->SecondaryShotType > 0)
            ptc_combat->ShotOverrideType = ptroll_battle_def->SecondaryShotType;
    }
}
