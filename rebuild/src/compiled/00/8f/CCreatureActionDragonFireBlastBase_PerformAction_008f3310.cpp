// CCreatureAction_DragonFireBlastBase::PerformAction() -- fire the weapon's shot from the creature's DRAGON_MOUTH_DUMMY toward a point level with the target, distance-matched along the mouth heading
// Leaf: ?PerformAction@CCreatureAction_DragonFireBlastBase@@UAEXXZ
// Inline-budget: FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (existing opt-in: the GetDef<T> body visible but __declspec(noinline))
//   -- retail CALLS the COMDAT CDefinitionManager::GetDef<CThingShotDef> 0x0078B230; without the opt-in DIFFER(823v796) (measured, audit e12);
//   the landed same-TU sibling 0x008F3630 defines the same opt-in.
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x008F3310, 796 bytes, plain `ret` (virtual; Ego_r.pdb / FableWin.pdb
//   ?PerformAction@CCreatureAction_DragonFireBlastBase@@UAEXXZ). Ego_d.pdb (combat_action_dragon.obj) gives every local: rhs, pobject,
//   pweapon_def, pshot_def, mouth_pos at function scope; target_pos, fire_direction, acc, line, ptc_shot, norm in the if block.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CDefClassBase::GetSubDef<CWeaponDef> 0x0042B1A6 (COMDAT called),
//   CCreatureActionBase::GetWorld 0x00692940, CDefinitionManager::GetDef<CThingShotDef> 0x0078B230, CThing::GetPos 0x004C73D0,
//   CCharString(const char*) 0x0099EBF0 / dtor 0x0099EAE0, NGraphicAppearance::CTCGraphicAppearance::GetDummyObject 0x004C08C0,
//   CreateThingObject 0x00703210 (CThingObject::Create, fastcall), CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor
//   0x00A01C10, CThingCreatureBase::GetShotAccuracyPercentage 0x00663B10, CDefStringTable::GetString 0x009D49B0, CThing::AddTC 0x004C9D60,
//   NThingShot::CTCShotDefinitionBase::Init(..., const C3DVector&, ...) 0x007BF790. Data: "DRAGON_MOUTH_DUMMY" 0x01285B4C, "" 0x0122D70E,
//   -1.0f 0x0122DEE0, the GFFastOneOverSqrt table 0x013CE558.
// Shape: the mouth heading is rhs.Up negated in place (`*= -1.0f`: X then Y, 0x008F348A..0x008F34A3) with Z cleared afterwards (the
//   normalise multiplies Z by a +0.0 constant, 0x008F351F); norm is scaled in place by the target distance (X, Y stored back into norm's
//   slots, 0x008F3563..0x008F3573) -- the by-value `norm * length` form evaluates Z first (8 hunks, measured). Same-TU sibling
//   CCreatureAction_DragonRearUpFull::PerformAction 0x008F3630 (landed) gives the include / opt-in set; its GFFastOneOverSqrt text
//   (NEWTON_PAREN) is kept for one text per TU (this row also matches the default text). The shot class name is a temporary (Ego_d lists
//   no local for it; audit e12: RELOCATION_MATCH as one full expression).
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBLASTBASE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT_POSITION
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "rebuild_abi.h"
#include "fable_string.h"
#include "fable_definition_table.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingObject.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingShotDef.h"
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CWEAPONDEF_SIZE_BODY   // NF357: GetSubDef<CWeaponDef> pick-any copy = size-mode owner 0x0042B1A6 (scaffolding, not retail)
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_DragonFireBlastBase.h"

void CCreatureAction_DragonFireBlastBase::PerformAction()
{
    CDefPointer<const CWeaponDef> pweapon_def;
    PCreature->PeekBaseDef().GetSubDef(&pweapon_def);
    CDefPointer<const CThingShotDef> pshot_def;
    GetWorld().PeekDefinitionManager().GetDef(pweapon_def->Shot, &pshot_def);
    C3DVector mouth_pos = PCreature->GetPos();
    CRightHandedSet rhs;
    PCreature->GetTCGraphicAppearance()->GetDummyObject("DRAGON_MOUTH_DUMMY", 0, 0, mouth_pos, rhs, 1.0f);
    CThingObject* pobject = CThingObject::Create(pshot_def->ObjectDef, mouth_pos, PCreature->GetPlayerNumber(), 0, 0, "");
    if (pobject) {
        PShot = CIntelligentPointer<CThing>(pobject);
        long acc = PCreature->GetShotAccuracyPercentage();
        NThingShot::CTCShotDefinitionBase* ptc_shot = static_cast<NThingShot::CTCShotDefinitionBase*>(PShot->AddTC(pshot_def->ShotDefinitionClassName.GetString(), true, 0));
        C3DVector fire_direction = rhs.GetUp();
        fire_direction *= -1.0f;
        fire_direction.Z = 0.0f;
        C3DVector norm = fire_direction;
        norm.Normalise();
        C3DVector line = PTarget->GetPos() - mouth_pos;
        norm *= line.GetMagnitude();
        C3DVector target_pos = mouth_pos + norm;
        target_pos.Z = PTarget->GetPos().Z;
        ptc_shot->Init(pshot_def, PCreature.Get(), 0, target_pos, (float)acc, 1.0f, 0.0f, CDefPointer<const CMeleeCombatAbilityDef>());
    }
}