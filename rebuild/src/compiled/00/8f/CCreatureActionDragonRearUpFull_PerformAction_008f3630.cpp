// CCreatureAction_DragonRearUpFull::PerformAction() -- ground slam: look up JACK_DRAGON_GROUND_SLAM's CThingShotDef, create the
// shot thing at the dragon's position dropped to ground height + 0.25, keep it in PShot, add its shot TC (named by the def's
// TCName) and Init it toward position + flat facing * 10 with the dragon's accuracy, exclude the dragon from its hits, then
// spawn the GROUND_SLAM_EXPLOSION effect at the dragon and release the defs.
// Leaf: ?PerformAction@CCreatureAction_DragonRearUpFull@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F3630, 880 bytes, plain `ret`.
// Port (NF74, round 7; nf41:e110): every class from the shared headers (the 29 local models and the stand-in GetSubDef<CWeaponDef> /
//   GetDef<CThingShotDef> bodies gone) -- the TrollThrowProjectile 0x008C6860 block (CTCShotDefinitionBase INIT / INIT_POSITION, GetSubDef,
//   CGameDefinitionManager, CWorld), engine/CCreatureAction_DragonRearUpFull.h (PShot +0x114), Ego_d.pdb locals (start_pos, pshot_def,
//   def_num, pobject, pweapon_def, acc, ptc_shot, facing_direction, target_pos, pspecial_fx, fx_index, p_emitter; the `creature_pos`
//   reference is not an Ego_d local: FITTED, it gives retail's one GetPos call and the (X, Y, Z + 10) construction). The special-effects
//   path binds GetSubDef<CSpecialEffectsDef> 0x00669580 as the queued nf41:e110 spelling. NF74 overlay opt-ins
//   FABLE_RETAIL_CCREATUREACTION_DRAGONREARUPFULL_PERFORMACTION and FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ADDTHINGTOAVOID (both PDBs'
//   ?AddThingToAvoid@CTCShotDefinitionBase@NThingShot@@QAEXABVCThing@@@Z = the landed descriptive AddHitThing 0x007C0FB0). The existing
//   no-code PAREN opt-in FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN places VC7.1's scheduling-region cut (the X*r store below the Init
//   pushes) as in the landed text's note; NEWTON_GROUPED / SQUARE_PAREN / STEP_PAREN and the default leave one interleave off.
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
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_DRAGONREARUPFULL
#define FABLE_RETAIL_CCREATUREACTION_DRAGONREARUPFULL_PERFORMACTION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT_POSITION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ADDTHINGTOAVOID
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_SKYBEAM_ORIGINAL
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTCPHYSICSBASE
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
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingObject.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingShotDef.h"
#include "engine/CSpecialEffectsDef.h"
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CWEAPONDEF_SIZE_BODY   // NF357: GetSubDef<CWeaponDef> pick-any copy = size-mode owner 0x0042B1A6 (scaffolding, not retail)
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_DragonRearUpFull.h"

void CCreatureAction_DragonRearUpFull::PerformAction()
{
    long def_num = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("JACK_DRAGON_GROUND_SLAM");
    CDefPointer<const CThingShotDef> pshot_def;
    GetWorld().PeekDefinitionManager().GetDef(def_num, &pshot_def);
    const C3DVector& creature_pos = PCreature->GetPos();
    C3DVector start_pos(creature_pos.X, creature_pos.Y, creature_pos.Z + 10.0f);
    start_pos.Z = GetWorld().GetWorldMap().GetGroundSizeZAt(start_pos) + 0.25f;
    CThing* pobject = CThingObject::Create(pshot_def->ObjectDef, start_pos, PCreature->GetPlayerNumber(), 0, 0, "");
    if (pobject) {
        PShot = CIntelligentPointer<CThing>(pobject);
        CDefPointer<const CWeaponDef> pweapon_def;
        PCreature->PeekBaseDef().GetSubDef(&pweapon_def);
        long acc = PCreature->GetShotAccuracyPercentage();
        NThingShot::CTCShotDefinitionBase* ptc_shot;
        {
            CCharString name = pshot_def->ShotDefinitionClassName.GetString();
            ptc_shot = static_cast<NThingShot::CTCShotDefinitionBase*>(PShot->AddTC(name, true, 0));
        }
        C3DVector facing_direction = PCreature->GetPhysicsTC()->GetFacingVector();
        facing_direction.Z = 0.0f;
        facing_direction.Normalise();
        C3DVector target_pos = start_pos + facing_direction * 10.0f;
        ptc_shot->Init(pshot_def, PCreature.Get(), 0, target_pos, (float)acc, 1.0f, 0.0f, CDefPointer<const CMeleeCombatAbilityDef>());
        ptc_shot->AddThingToAvoid(*PCreature);
    }
    CDefPointer<const CSpecialEffectsDef> pspecial_fx;
    long fx_index = 0;
    PCreature->PeekBaseDef().GetSubDef(&pspecial_fx);
    if (!pspecial_fx.IsNull()) {
        if (pspecial_fx->GetEffectFromString("GROUND_SLAM_EXPLOSION", fx_index)) {
            CThing* p_emitter = CTCDParticleEmitter::Create(fx_index, PCreature->GetPos(), false);
        }
    }
}
