// 007bd527 NThingShot::CTCShotDefinitionProjectilePredictive::OnSubShotTrigger
// Leaf: ?OnSubShotTrigger@CTCShotDefinitionProjectilePredictive@NThingShot@@MAEXXZ
// Flags: none
// Inline-budget: adjacent OnSubShotTrigger 007BD2D1 uses the same size mode and string comparison.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_CLASSTYPENAME
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTHINGSHOTDEF_DEFSTRING
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_GETNUMBER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT_POSITION
#define FABLE_RETAIL_PREDICTIVE_SUBSHOT_PASS26
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_PREDICTIVE_RHS_COPY_PASS26
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ADDTHINGTOAVOID
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefString.h"
#include "engine/CThingShotDef.h"
#include "engine/CThingObject.h"
#include "engine/NThingShot_CTCShotDefinitionProjectilePredictive.h"
#include "engine/ThingAlive.h"
// The native GetDef instance and the right-handed copy both omit an EBP frame.
// The neighboring projectile methods use size optimization and an EBP frame.
template bool CDefinitionManager::GetDef<CThingShotDef>(long,CDefPointer<const CThingShotDef>*) const;
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
namespace NThingShot {
void CTCShotDefinitionProjectilePredictive::OnSubShotTrigger()
{
    long def_index = GetShotDef().SubShotType;
    if (def_index >= 0) {
        CDefPointer<const CThingShotDef> pshot_def;
        CMainGameComponent::Get()->PeekDefinitionManager().GetDef(def_index, &pshot_def);
        long obj_def_index = pshot_def->ObjectDef;
        const CCharString& class_type_name = CMainGameComponent::Get()->PeekDefinitionManager().GetDefClassTypeNameFromGlobalIndex(obj_def_index);
        CRightHandedSet main_rhs = GetParentThing().GetPhysicsTC()->GetRHSet();
        long nshots = GetShotDef().NumSubShots;
        if (nshots > 0) {
            float rotate_angle = GetShotDef().SubShotMaxAngle * -1.0f;
            float rotate_step = 2.0f * GetShotDef().SubShotMaxAngle / (nshots - 1);
            for (long n = 0; n < nshots; ++n) {
                if (class_type_name == "OBJECT") {
                    CThingObject* pobject;
                    {
                        CRightHandedSet rhs_to_use = GetParentThing().GetPhysicsTC()->GetRHSet();
                        long player_number = GetParentThing().GetPlayerNumber();
                        if (IS_THING_ALIVE(GetPShooter())) player_number = GetPShooter()->GetPlayerNumber();
                        pobject = CThingObject::Create(obj_def_index, GetParentThing().GetPos(), player_number, &rhs_to_use, 0, "");
                    }
                    if (pobject) {
                        CTCShotDefinitionBase* ptc_shot = (CTCShotDefinitionBase*)pobject->AddTC(pshot_def->ShotDefinitionClassName, true, 0);
                        CRightHandedSet rhs = main_rhs;
                        rhs.Rotate(main_rhs.GetUp(), rotate_angle);
                        C3DVector target_direction = rhs.GetForward();
                        C3DVector target_pos = GetParentThing().GetPos() + target_direction * 30.0f;
                        ptc_shot->Init(pshot_def, GetPShooter(), GetPWeapon(), target_pos, (float)GetShotAccuracyPercentage(), DamageMultiplier, ShotBuildupTime, PShotAbilityDef);
                        for (long hit_thing_index = 0; hit_thing_index < HitThingsList.size(); ++hit_thing_index)
                            ptc_shot->AddThingToAvoid(*HitThingsList[hit_thing_index]);
                    }
                }
                rotate_angle += rotate_step;
            }
        }
    }
}
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif