// NThingShot::CTCShotDefinitionBase::Init(CDefPointer<const CThingShotDef>, CThing* shooter, CThing* weapon, const C3DVector& target,
//   float accuracy, float damageMultiplier, float buildupTime, const CDefPointer<const CMeleeCombatAbilityDef>& ability)
// -- the target-POSITION overload: store the shot def / ability def, shooter and weapon, the three shot parameters and reset state
// (DieOnFrame -1, PassedTarget/KnockDown/Decapitate false, KnockDown true when an ability def is given), copy the target position,
// return the parent's physics to its default interaction flags, run InitAttachedEffects / InitMovement, copy a live weapon's
// augmentations onto the shot, make shot and live shooter not solid to each other (adding a CTCSolidToThing to the shot when it
// has none), force the graphic to resend every frame, seed BestCollisionPos with the shot's position, update its weapon trails and
// stamp the start frame.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BF790, 762 bytes, thiscall `ret 0x20`, esp frame 8 (one temp CIntelligentPointer<CThing>).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10,
//   CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound 0x0040F020 (GetTC expanded), CTCObjectAugmentations::
//   CopyAugmentationDataFromThing 0x00768210, CTCSolidToThing::AddThingNotSolidTo 0x00770FA0, CThing::AddTC 0x004C9D60
//   ("CTCSolidToThing" 0x0123B47C; CCharString 0x0099EBF0 / ~ 0x0099EAE0), CThing::GetPos 0x004C73D0,
//   CTCWeaponTrail::UpdateWeaponTrails 0x0076EFA0, CTCBase::GetWorld 0x00686A70, CWorld::GetFrame 0x0049D870;
//   CTCPhysicsBase::ReturnToDefaultInteractionFlags (vslot 44, +0xB0), own InitAttachedEffects (+0x88) / InitMovement (+0x84).
// Shape: the AddTC path is `solid = 0` inside the failed-GetTC branch plus a named CCharString in its own block: the name is built
//   before the zero arguments are pushed (both from the zeroed solid register, `xor edi,edi ; push edi ; push edi`) and destroyed
//   right after AddTC; the two GetTC copies are cross-jumped by the compiler. StatGetInterfaceType of CTCObjectAugmentations /
//   CTCWeaponTrail are inline (overlay opt-ins), so the HasTC tests fold to in-place bit tests.
// Leaf: ?Init@CTCShotDefinitionBase@NThingShot@@QAEXV?$CDefPointer@$$CBVCThingShotDef@@@@PAVCThing@@1ABVC3DVector@@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_CUTSCENEPAUSE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_VIRTUALS
#define FABLE_RETAIL_CTHINGPHYSICAL_VIRTUALS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED
#define FABLE_RETAIL_CTCWEAPONTRAIL_HASPOINTS
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT_POSITION
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETFORCERESEND
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATEWEAPONTRAILS
#define FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_COPYFROMTHING
#include "engine/CThing.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CTCWeaponTrail.h"
#include "engine/CTCSolidToThing.h"
#include "engine/CTCObjectAugmentations.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"

void NThingShot::CTCShotDefinitionBase::Init(CDefPointer<const CThingShotDef> def, CThing* shooter, CThing* weapon, const C3DVector& target,
                                             float accuracy, float damageMultiplier, float buildupTime,
                                             const CDefPointer<const CMeleeCombatAbilityDef>& ability)
{
    PShotDef = def;
    PShotAbilityDef = ability;
    PShooter = CIntelligentPointer<CThing>(shooter);
    PWeapon = CIntelligentPointer<CThing>(weapon);
    ShotBuildupTime = buildupTime;
    DamageMultiplier = damageMultiplier;
    AccuracyPercentage = accuracy;
    DieOnFrame = -1;
    PassedTarget = false;
    KnockDown = false;
    Decapitate = false;
    if (!PShotAbilityDef.IsNull())
        KnockDown = true;
    *(C3DVector*)TargetPos = target;
    ParentThing->GetPhysicsTC()->ReturnToDefaultInteractionFlags();
    InitAttachedEffects();
    InitMovement();
    CTCObjectAugmentations* augmentations;
    if (IS_THING_ALIVE(weapon) && ParentThing->GetTC(&augmentations))
        augmentations->CopyAugmentationDataFromThing(*weapon);
    if (IS_THING_ALIVE(PShooter)) {
        CTCSolidToThing* shooterSolid;
        if (PShooter->GetTC(&shooterSolid))
            shooterSolid->AddThingNotSolidTo(ParentThing);
        CTCSolidToThing* solid;
        if (!ParentThing->GetTC(&solid)) {
            solid = 0;
            {
                CCharString name("CTCSolidToThing");
                ParentThing->AddTC(name, false, 0);
            }
            ParentThing->GetTC(&solid);
        }
        solid->AddThingNotSolidTo(PShooter.Get());
    }
    ParentThing->GetTCGraphicAppearance()->SetForceResendEveryRenderFrame(true);
    BestCollisionPos = ParentThing->GetPos();
    BestCollisionType = (ECollideType)0;
    CTCWeaponTrail* trail;
    if (ParentThing->GetTC(&trail))
        trail->UpdateWeaponTrails();
    StartFrame = GetWorld().GetFrame();
}