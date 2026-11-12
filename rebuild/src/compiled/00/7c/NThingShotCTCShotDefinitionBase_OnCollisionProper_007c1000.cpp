// NThingShot::CTCShotDefinitionBase::OnCollisionProper(bool& killed) -- process projectile collision and resulting hit effects
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Leaf: ?OnCollisionProper@CTCShotDefinitionBase@NThingShot@@IAEXAA_N@Z
// Retail: 0x007C1000, 4510 bytes, thiscall.
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
#include <math.h>
#include "engine/CThingGameObject.h"
#include "engine/CThingBaseDef.h"
#include "engine/CHitParameters.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingMultiArrowShotDef.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventShotStruck.h"
#include "engine/CTCWeaponTrail.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCRotate.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCDExplosion.h"
#include "engine/CThingNoise.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCDExperienceOrb.h"
#include "engine/CTCDeadShotInThingTracker.h"
#include "engine/CTCObjectAugmentations.h"
#include "engine/ThingAlive.h"
#include "engine/CDefClassBase_GetSubDef.h"

float FABLE_FASTCALL GFFloatRandom(float,unsigned long*);
void NThingShot::CTCShotDefinitionBase::OnCollisionProper(bool& killed)
{
    if (BestCollisionType == 0)
        return;
    if (PShotDef->PreventSlidingOnCollision) {
        CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
        physics->SetVelocity(C3DVector(0.0f, 0.0f, 0.0f));
    }
    CThing* pthing = PBestCollisionThing.Get();
    ECollideType type = BestCollisionType;
    const C3DVector& collide_pos = BestCollisionPos;
    const C3DVector& collision_normal = BestCollisionNormal;
    NHitLocations::CHitLocation* hitLocation = PBestCollisionHitLocation;
    bool deflected = false;
    CTCEntityEvents* events;
    if (ParentThing->GetTC((ETCInterfaceType)0x42, &events))
        events->AddEvent(new NEntityEvents::CEventShotStruck(BestCollisionPos));
    if (IS_THING_ALIVE(pthing) && (pthing->GetTypeFlags() & 4) && hitLocation) {
        CDefPointer<const CWeaponDef> weaponDef;
        if (!ParentThing->PeekBaseDef().GetSubDef(&weaponDef)) {
            deflected = true;
        } else {
            CThingGameObject* phit_go = static_cast<CThingGameObject*>(pthing);
            CHitParameters params;
            phit_go->GetShotHitParameters(hitLocation, *ParentThing, collide_pos, params);
            params.SetHitNegated(IsNegatedHit());
            params.SetCreateParticleEffectOnHit(IsCreateParticleEffectOnHit());
            phit_go->OnHit(params);
            if (params.IsBlocked() || params.IsHitNegated())
                deflected = true;
        }
    }
    if (IS_THING_ALIVE(PBestCollisionThing)) {
        HitThingsList.push_back(CIntelligentPointer<const CThing>(PBestCollisionThing));
    }
    if (ParentThing->HasTC((ETCInterfaceType)0x2a))
        ParentThing->RemoveTC((ETCInterfaceType)0x2a);
    if (((type == 3 && PShotDef->BounceOnGround) || deflected) && PShotDef->AllowDeflections) {
        ParentThing->SetCutscenePauseBehaviour(CUTSCENE_BEHAVIOUR_NOT_PAUSED);
        CTCWeaponTrail* trail;
        ParentThing->GetTC((ETCInterfaceType)0x4c, &trail);
        std::vector<std::pair<C3DVector, C3DVector> > points;
        C3DVector direction;
        if (trail->GetLastProjectileWeaponTrailPoints(points) && points.size() >= 2) {
            direction = (points.end() - 1)->first - (points.end() - 2)->first;
            trail->SetLastProjectileWeaponTrailPoint(collide_pos);
        } else {
            CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
            direction = physics->GetPosition() - physics->GetLastPosition();
        }
        direction *= PShotDef->Bounciness * 0.5f;
        C3DVector deflect_line = collision_normal * (float)fabs(collision_normal.Dot(direction));
        C3DVector along_line = direction + deflect_line;
        C3DVector outward_line = deflect_line + along_line;
        CThing* thing = ParentThing;
        CThing* shooter = PShooter.Get();
        thing->GetPhysicsTC()->SetVelocity(C3DVector(0.0f, 0.0f, 0.0f));
        C3DVector up(0.0f, 0.0f, 1.0f);
        C3DVector forward = outward_line;
        C3DVector side = outward_line.CrossWithUp();
        if (side.IsNonZero()) {
            up = side.Cross(outward_line);
            up.AccurateNormalise();
        } else {
            forward.Z = 0.0f;
            if (forward.IsNonZero())
                forward.AccurateNormalise();
        }
        if (!(ParentThing->GetTypeFlags() & 8)) {
            thing->GetPhysicsTC()->ResetRHSet(CRightHandedSet(up,forward));
        }
        CDefPointer<const CWeaponDef> weaponDef;
        C3DVector offset;
        if (thing->PeekBaseDef().GetSubDef(&weaponDef)) {
            C3DVector tip;
            if (!thing->GetTCGraphicAppearance()->GetHelperPoint(weaponDef->ProjectileWeaponHelperNameTip, 0, 0, tip, 1.0f))
                tip = thing->GetPos();
            offset = tip - thing->GetPos();
        } else {
            offset = C3DVector(0.0f, 0.0f, 0.0f);
        }
        const C3DVector& position = collide_pos;
        C3DVector new_pos = position - offset;
        thing->GetPhysicsTC()->ResetPosition(new_pos);
        thing->GetPhysicsTC()->SetVelocity(forward);
        thing->GetPhysicsTC()->EnableGravity(true);
        if (PShotDef->RotateOnDeflection) {
            CTCRotate* rotate = (CTCRotate*)thing->AddTC("CTCRotate", false, 0);
            rotate->SetRotationInfo(collision_normal, 1.0f / CMainGameComponent::GetConstantFPS(), 100.0f);
            rotate->SetRotating(true);
        }
        thing->RemoveTC((ETCInterfaceType)0x77);
        if (!(ParentThing->GetTypeFlags() & 8))
            ParentThing->RemoveTC((ETCInterfaceType)0x7f);
        killed = true;
        if (PShotDef->PrimaryEffect)
            CTCDParticleEmitter::Create(PShotDef->PrimaryEffect, position, false);
        if (MultiArrowShotType) {
            CDefPointer<const CThingMultiArrowShotDef> arrowDef;
            PShotDef->GetSubDef(&arrowDef);
            long effect = arrowDef->MultiArrowHitEffect;
            if (effect && GFFloatRandom(1.0f, GetWorld().GetWorldSeed()) < arrowDef->MultiArrowHitEffectPercentage)
                CTCDParticleEmitter::Create(effect, position, false);
        }
        CTCObjectAugmentations* augmentations;
        if (ParentThing->GetTC((ETCInterfaceType)0xbf, &augmentations)) {
            long effect = augmentations->GetOnHitExplodingEffect();
            if (effect)
                CTCDParticleEmitter::Create(effect, position, false);
        }
        return;
    }
    if (PShotDef->GroundExplosionDef >= 0 && type == COLLIDE_TYPE_MAP) {
        CThing* blast = CTCDExplosion::Create(PShotDef->GroundExplosionDef, collide_pos);
        if (!PShotDef->DoesExplosionDamageCaster && blast) {
            CTCDExplosion* driver;
            blast->GetTCDriver(&driver);
            driver->SetThingAsImmune(*PShooter.Get());
        }
    } else if (PShotDef->ExplosionDef >= 0) {
        CThing* blast = CTCDExplosion::Create(PShotDef->ExplosionDef, collide_pos);
        if (!PShotDef->DoesExplosionDamageCaster && blast) {
            CTCDExplosion* driver;
            blast->GetTCDriver(&driver);
            driver->SetThingAsImmune(*PShooter.Get());
        }
    }
    if (!ParentThing->IsDead()) {
        if (PeekWorldMap().GetMapNumberAt(collide_pos)) {
            long noiseDef = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("SIMPLE_NOISE");
            CThingNoise* noise = CThingNoise::Create(noiseDef, collide_pos, -1.0f);
            if (IS_THING_ALIVE(PShooter))
                noise->SetCreator(*PShooter.Get());
        }
        const CThingShotDef* shotDef = PShotDef.operator->();
        bool finishShot = true;
        bool leaveObject = shotDef->LeavesObjectBehind != 0;
        bool stick_in = true;
        bool keepShot = false;
        if (type == 1) {
            if (IS_THING_ALIVE(pthing)) {
                unsigned long flags = pthing->GetTypeFlags();
                CTCHeroExperience* experience;
                if (pthing->HasTC((ETCInterfaceType)0x69) && IS_THING_ALIVE(PShooter) && PShooter.Get()->GetTC((ETCInterfaceType)0x68, &experience)) {
                    CTCDExperienceOrb* orb;
                    if (pthing->GetTCDriver(&orb)) {
                        experience->AddExperience(orb->GetExperiencePoints());
                        orb->OnCollected(*PShooter.Get());
                    }
                }
                if ((flags & 8) || pthing->HasTC((ETCInterfaceType)0x5a) || pthing->GetPhysicsTC()->IsMoving()) {
                    stick_in = false;
                    leaveObject = false;
                }
            } else {
                stick_in = false;
                leaveObject = false;
            }
            if (shotDef->PrimaryEffect)
                CTCDParticleEmitter::Create(shotDef->PrimaryEffect, collide_pos, false);
            if (MultiArrowShotType) {
                CDefPointer<const CThingMultiArrowShotDef> arrowDef;
                PShotDef->GetSubDef(&arrowDef);
                long effect = arrowDef->MultiArrowHitEffect;
                if (effect && GFFloatRandom(1.0f, GetWorld().GetWorldSeed()) < arrowDef->MultiArrowHitEffectPercentage)
                    CTCDParticleEmitter::Create(effect, collide_pos, false);
            }
            CTCObjectAugmentations* augmentations;
            if (ParentThing->GetTC((ETCInterfaceType)0xbf, &augmentations)) {
                long effect = augmentations->GetOnHitExplodingEffect();
                if (effect)
                    CTCDParticleEmitter::Create(effect, collide_pos, false);
            }
            if (!PShotAbilityDef.IsNull()) {
                keepShot = true;
                finishShot = false;
            }
            if (stick_in && IS_THING_ALIVE(pthing)) {
                ParentThing->AddTC("CTCDeadShotInThingTracker", false, 0);
                CTCDeadShotInThingTracker* tracker;
                ParentThing->GetTC((ETCInterfaceType)0x76, &tracker);
                tracker->SetThingStuckIn(*pthing);
            }
        } else if (type == 3) {
            if (shotDef->PrimaryEffect)
                CTCDParticleEmitter::Create(shotDef->PrimaryEffect, collide_pos, false);
            if (MultiArrowShotType) {
                CDefPointer<const CThingMultiArrowShotDef> arrowDef;
                PShotDef->GetSubDef(&arrowDef);
                long effect = arrowDef->MultiArrowHitEffect;
                if (effect && GFFloatRandom(1.0f, GetWorld().GetWorldSeed()) < arrowDef->MultiArrowHitEffectPercentage)
                    CTCDParticleEmitter::Create(effect, collide_pos, false);
            }
            CTCObjectAugmentations* augmentations;
            if (ParentThing->GetTC((ETCInterfaceType)0xbf, &augmentations)) {
                long effect = augmentations->GetOnHitExplodingEffect();
                if (effect)
                    CTCDParticleEmitter::Create(effect, collide_pos, false);
            }
        } else if (type == 4) {
            leaveObject = false;
        }
        CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
        if (ParentThing->GetTypeFlags() & 8) {
            physics->SetFacingAngleXZ(0.0f);
            physics->SetFacingAngleYZ(0.0f);
            finishShot = false;
        }
        if (finishShot) {
            if (leaveObject) {
                if (IS_THING_ALIVE(PEffect)) {
                    CTCDParticleEmitter* emitter = 0;
                    PEffect.Get()->GetTCDriver(&emitter);
                    if (PShotDef->UseEndTimeLineOnContinousEffect)
                        emitter->EnterTimelineEnd();
                    else
                        PEffect.Get()->Kill(true);
                }
                CTCObjectAugmentations* augmentations;
                if (ParentThing->GetTC((ETCInterfaceType)0xbf, &augmentations))
                    augmentations->StopTrailEffect();
                CDefPointer<const CWeaponDef> weaponDef;
                if (stick_in && ParentThing->PeekBaseDef().GetSubDef(&weaponDef)) {
                    NGraphicAppearance::CTCGraphicAppearance* appearance = ParentThing->GetTCGraphicAppearance();
                    CCharString tipName(weaponDef->ProjectileWeaponHelperNameTip);
                    CCharString baseName(weaponDef->ProjectileWeaponHelperNameBase);
                    C3DVector tip;
                    C3DVector base;
                    if (tipName == baseName) {
                        appearance->GetHelperPoint(tipName, 0, 0, tip, 1.0f);
                        appearance->GetHelperPoint(baseName, 1, 0, base, 1.0f);
                    } else {
                        appearance->GetHelperPoint(tipName, 0, 0, tip, 1.0f);
                        appearance->GetHelperPoint(baseName, 0, 0, base, 1.0f);
                    }
                    C3DVector position = ParentThing->GetPos();
                    C3DVector stick_in_pos = base + (tip - base) * 2.0f / 3.0f;
                    C3DVector line_offset = stick_in_pos - position;
                    C3DVector new_pos = collide_pos - line_offset;
                    physics->ResetPosition(new_pos);
                    appearance->GetHelperPoint(tipName, 0, 0, tip, 1.0f);
                    appearance->GetHelperPoint(baseName, 0, 0, base, 1.0f);
                } else {
                    C3DVector ground = collide_pos;
                    ground.Z = GetWorldMap().GetGroundSizeZAt(ground);
                    physics->ResetPosition(ground);
                    C3DVector facing = physics->GetFacingVector();
                    facing.Z = 0.0f;
                    if (facing.IsNonZero()) {
                        facing.AccurateNormalise();
                        physics->ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), facing));
                    }
                }
                physics->SetAsMoving(false);
                physics->SetInteractionFlags(0x20);
                ParentThing->RemoveTC((ETCInterfaceType)0x77);
                if (!(ParentThing->GetTypeFlags() & 8))
                    ParentThing->RemoveTC((ETCInterfaceType)0x7f);
                killed = true;
            } else {
                if (IS_THING_ALIVE(PEffect)) {
                    CTCDParticleEmitter* emitter = 0;
                    PEffect.Get()->GetTCDriver(&emitter);
                    if (PShotDef->UseEndTimeLineOnContinousEffect)
                        emitter->EnterTimelineEnd();
                    else
                        PEffect.Get()->Kill(true);
                }
                CTCObjectAugmentations* augmentations;
                if (ParentThing->GetTC((ETCInterfaceType)0xbf, &augmentations))
                    augmentations->StopTrailEffect();
                ParentThing->Kill(true);
                killed = true;
            }
        } else if (!keepShot) {
            if (IS_THING_ALIVE(PEffect)) {
                CTCDParticleEmitter* emitter = 0;
                PEffect.Get()->GetTCDriver(&emitter);
                if (PShotDef->UseEndTimeLineOnContinousEffect)
                    emitter->EnterTimelineEnd();
                else
                    PEffect.Get()->Kill(true);
            }
            CTCObjectAugmentations* augmentations;
            if (ParentThing->GetTC((ETCInterfaceType)0xbf, &augmentations))
                augmentations->StopTrailEffect();
            physics->SetAsMoving(false);
            physics->EnableGravity(true);
            physics->ReturnToDefaultInteractionFlags();
            ParentThing->RemoveTC((ETCInterfaceType)0x77);
            if (!(ParentThing->GetTypeFlags() & 8))
                ParentThing->RemoveTC((ETCInterfaceType)0x7f);
            killed = true;
        }
    }
    BestCollisionType = COLLIDE_TYPE_NONE;
    PBestCollisionThing = CIntelligentPointer<CThing>(0);
    PBestCollisionHitLocation = 0;
}