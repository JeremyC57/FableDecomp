// CCreatureAction_TrollThrowProjectile::PerformAction() -- the troll lets go of the rock: when the creature has a carrying component
//   and the shot thing (PShot) is alive, the shot is taken out of the carrying component, the creature's CWeaponDef sub-def names the
//   CThingShotDef (CWeaponDef::Shot), the shot thing gets the shot component named by CThingShotDef::ShotDefinitionClassName and that
//   component is initialised with the creature as shooter, no weapon, the creature's shot accuracy, damage multiplier 1.0, build-up
//   time 0.0 and a null melee ability -- aimed at the stored TargetPos when UseTargetPos is set, else at the PTarget thing.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C6860, 482 bytes, virtual (vtable 0x01281694 slot 3), plain `ret`, frame 0x10, no EH frame.
// Leaf: ?PerformAction@CCreatureAction_TrollThrowProjectile@@UAEXXZ
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound 0x0040F020 (inside the
//   expanded CThing::GetTC<CTCCarrying>, TCI_CARRYING 0x46); CTCCarrying::RemoveThing 0x006A19B0; CDefClassBase::GetSubDef<CWeaponDef>
//   0x0042B1A6; CCreatureActionBase::GetWorld 0x00692940 (CWorld::PeekDefinitionManager inline, +0x10); CDefinitionManager::
//   GetDef<CThingShotDef> 0x0078B230; CThingCreatureBase::GetShotAccuracyPercentage 0x00663B10; CDefStringTable::GetString 0x009D49B0
//   on GDefStringTable 0x013CA828 (CDefString::GetString inline); CThing::AddTC 0x004C9D60; CCharString::~CCharString 0x0099EAE0;
//   NThingShot::CTCShotDefinitionBase::Init 0x007BF790 (target position) / 0x007C0690 (target thing); def pointee Delete (slot 1).
// Shape (why the source is spelled this way):
//   - the GetSubDef<T> and GetDef<T> template bodies are visible in the TU but called out of line (FABLE_RETAIL_CDEFCLASSBASE_
//     GETSUBDEF_NOINLINE, FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY): the compiler then reads the shot def back once into
//     ebp and reuses its frame word for the name string and the melee-ability pointers; the weapon def is released with no null
//     test (it was dereferenced for Shot), the shot def keeps its null tests because its only dereference goes through
//     CDefString::GetString on the member (CThingShotDef::ShotDefinitionClassName as a real CDefString);
//   - the class name is a block-scoped CCharString (GetString runs before AddTC's `push 0 ; push 1`, its destructor before the
//     UseTargetPos test);
//   - the null melee-ability pointer is a local of EACH branch (one symbol per branch): retail zeroes the shared frame word once
//     ahead of the branch and releases it in one merged tail; a single pointer declared ahead of the `if` swaps the frame words of
//     the weapon def and the shot def and the edi/ebp roles;
//   - in the target-thing branch the shooter is read into a local first (GetPItem ahead of the fild, as the sibling rows 0x008C0900 /
//     0x008F3100 / 0x008F3630 do).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTHINGSHOTDEF_DEFSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETSHOTACCURACYPERCENTAGE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_TROLLTHROWPROJECTILE
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT_POSITION
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
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
#include "engine/CThingBaseDef.h"
#include "engine/CTCCarrying.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingShotDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_TrollThrowProjectile.h"

void CCreatureAction_TrollThrowProjectile::PerformAction()
{
    CTCCarrying* carrying;
    if (PCreature->GetTC(&carrying)) {
        if (IS_THING_ALIVE(PShot)) {
            carrying->RemoveThing(*PShot);
            CDefPointer<const CWeaponDef> weaponDef;
            PCreature->PeekBaseDef().GetSubDef(&weaponDef);
            CDefPointer<const CThingShotDef> shotDef;
            GetWorld().PeekDefinitionManager().GetDef(weaponDef->Shot, &shotDef);
            long accuracy = PCreature->GetShotAccuracyPercentage();
            NThingShot::CTCShotDefinitionBase* shot;
            {
                CCharString name = shotDef->ShotDefinitionClassName.GetString();
                shot = static_cast<NThingShot::CTCShotDefinitionBase*>(PShot->AddTC(name, true, 0));
            }
            if (UseTargetPos) {
                CDefPointer<const CMeleeCombatAbilityDef> ability;
                shot->Init(shotDef, PCreature.Get(), 0, TargetPos, (float)accuracy, 1.0f, 0.0f, ability);
            }
            else {
                CDefPointer<const CMeleeCombatAbilityDef> ability;
                CThing* shooter = PCreature.Get();
                shot->Init(shotDef, shooter, 0, PTarget.Get(), (float)accuracy, 1.0f, 0.0f, ability);
            }
        }
    }
}