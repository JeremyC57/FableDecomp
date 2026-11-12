// NParticleEngine::CPSCUpdateNormal::Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, bool& keep_alive) -- advance active particles and their effects
// Grade: DIFFER(5604v5602) -- 5604/5602; exact2DC frame. First velocity/position copy schedule, collision-position copy, final matrix copy schedule and one extra orientation-destination LEA plus subsequent scratch rotation. No byte match.
// Flags: /d2inlT200
// Leaf: ?Update@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AA_N@Z
// Retail: 0x00B0DC20, 5602 bytes, thiscall.
// Identity: both original PDBs; native vtable0129FC1C slot9; donor02F3F4C0.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_C3DVECTOR_CROSS_XY_GROUP
#define FABLE_RETAIL_C3DVECTOR_CLEAR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_KILL
#define FABLE_RETAIL_CPARTICLEEMITTER_UPDATE
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#define FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#define FABLE_RETAIL_CQUATERNION_INLINE_TOMATRIX
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR
#define FABLE_RETAIL_CDECAL
#define FABLE_RETAIL_CDECAL_UPDATE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_DATA_UNION
#define FABLE_RETAIL_CMATRIX3X4_PARTICLE_POINT
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANDANGLE
#include "engine/CPSCUpdateNormal.h"
#include "engine/CParticle.h"
#include "engine/CDecal.h"
#include "engine/CParticleSystem.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/CParticleEmitterDatabase.h"
#include "engine/CAEnvironmentInteraction.h"
#include <math.h>
#include "engine/GFPower.h"
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(fabs)
#endif
long FABLE_FASTCALL GFROR13(unsigned long);
// Original donor018CF370: two seed updates, rotate, then zero-range guard.
inline unsigned long FABLE_FASTCALL GFRandom(unsigned long range,unsigned long* seed)
{
    *seed *= 0x24a1;
    *seed += 0x24df;
    *seed = GFROR13(*seed);
    if (range) return *seed % range;
    return 0;
}
// Original0261C580 delegates to the four-vector Set overload.
inline void CMatrix3x4::Set(const CRightHandedSet& orientation,const C3DVector& translation)
{Set(orientation.GetRight(),orientation.GetForward(),orientation.GetUp(),translation);}
namespace NParticleEngine {
float FABLE_FASTCALL GetOneOverTickRate();
long FABLE_FASTCALL GetTickRate();
unsigned long& FABLE_FASTCALL GetRandomSeed();
CAEnvironmentInteraction& FABLE_FASTCALL GetEnvironmentInteraction();
CParticleEmitterDatabase& FABLE_FASTCALL GetParticleEmitterDatabase();
CParticleEmitterManager& FABLE_FASTCALL GetParticleEmitterManager();
void CPSCUpdateNormal::Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, bool& keep_alive)
{
    const float timestep = GetOneOverTickRate();
    CAEnvironmentInteraction& environment = GetEnvironmentInteraction();
    float time_secs = LifeTimer * timestep;
    float normalised_time = 0.0f;
    if (UseSystemLifeSecs) {
        if (time_secs >= SystemLifeSecs) {
            keep_alive = false;
            return;
        }
        if (SystemLifeSecs > 0.0f) {
            normalised_time = time_secs / SystemLifeSecs;
            if (normalised_time < 0.0f)
                normalised_time = 0.0f;
            else if (normalised_time > 1.0f)
                normalised_time = 1.0f;
        }
        if (SystemAlphaFadeEnable) {
            float system_alpha = GetCurrentFadeValue() * (1.0f - SystemAlphaFadeMinimum) + SystemAlphaFadeMinimum;
            system.SetGlobalAlpha(system_alpha);
        }
    }
    system.SetNormalisedTime(UseSystemLifeSecs, normalised_time);
    float speed_preserve_factor;
    if (AirResistance > 0.0f)
        speed_preserve_factor = GFPower(1.0f - AirResistance, timestep);
    else
        speed_preserve_factor = 1.0f;
    C3DVector global_acceleration(0.0f, 0.0f, 0.0f);
    if (fabs(GravityFactor) > 0.0f)
        global_acceleration = environment.GetGravityVector() * GravityFactor * timestep;
    if (fabs(WindFactor) > 0.0f && environment.IsWindActive())
        global_acceleration += environment.GetWindVelocity() * WindFactor * timestep;
    if (system.GetParentEmitter().IsEmitter2D()) {
        CMatrix3x4 trans;
        trans.SetFromVectorAndAngle(C3DVector(1.0f, 0.0f, 0.0f), -0.25f);
        trans.TransformVector(global_acceleration);
    }
    C3DVector system_accelecration(0.0f, 0.0f, 0.0f);
    if (!UseAccelerationParam || !params.GetVectorParam(system_accelecration, AccelerationParam))
        system_accelecration = ParticleAcceleration;
    system_accelecration *= ParticleAccelerationScale;
    long max_particle_life = (long)(GetTickRate() * ParticleLifeSecs);
    unsigned long& seed = GetRandomSeed();
    for (std::list<CParticle*>::iterator it_particle = system.GetParticleList().begin();
         it_particle != system.GetParticleList().end();) {
        CParticle& particle = **it_particle;
        if (!particle.IsActive())
            continue;
        if (UseParticleLifeSecs && particle.GetLifetimer() >= max_particle_life) {
            it_particle = system.DeleteParticle(it_particle);
            continue;
        }
        C3DVector local_acceleration = system_accelecration + particle.GetAcceleration();
        C3DVector velocity = particle.GetVelocity();
        local_acceleration *= timestep;
        C3DVector pos = particle.GetPosition();
        C3DVector offset;
        if (RandomisePosEnable) {
            if (!particle.HaveRandomAccelerationData())
                particle.CreateRandomAccelerationData(seed);
            if (particle.HaveRandomAccelerationData()) {
                float factor = 1.0f;
                const NFractal::CPerlinNoise& perlin_noise = GetParticleEmitterManager().PeekPerlinNoise();
                C2DVector noise_lookup(time_secs * RandomisePosSpeed.X + particle.GetRandomAccelerationTimeOffsetX(), particle.GetRandomAccelerationIndexX());
                offset.X = factor * RandomisePosScale.X * perlin_noise.GetGaussian2DNoise(noise_lookup);
                noise_lookup = C2DVector(time_secs * RandomisePosSpeed.Y + particle.GetRandomAccelerationTimeOffsetY(), particle.GetRandomAccelerationIndexY());
                offset.Y = factor * RandomisePosScale.Y * perlin_noise.GetGaussian2DNoise(noise_lookup);
                noise_lookup = C2DVector(time_secs * RandomisePosSpeed.Z + particle.GetRandomAccelerationTimeOffsetZ(), particle.GetRandomAccelerationIndexZ());
                offset.Z = factor * RandomisePosScale.Z * perlin_noise.GetGaussian2DNoise(noise_lookup);
                local_acceleration -= particle.GetOldRandomOffset();
                local_acceleration += offset;
            }
        }
        system.TransformVectorIfLocal(local_acceleration);
        C3DVector acceleration = local_acceleration + global_acceleration;
        if (particle.GetLifetimer() >= 0) {
            velocity += acceleration;
            pos += velocity;
            velocity *= speed_preserve_factor;
        } else {
            float fraction = 1.0f - particle.GetCreationTime();
            velocity += acceleration * fraction;
            pos += velocity * fraction;
            offset *= fraction;
            velocity *= GFPower(speed_preserve_factor, fraction);
        }
        if (RandomisePosEnable && particle.HaveRandomAccelerationData())
            particle.SetOldRandomOffset(offset);
        C3DVector collide_pos;
        C3DVector collide_normal;
        C3DVector test_pos = particle.GetPosition();
        system.TransformPosIfLocal(test_pos);
        bool landscape_collision = true;
        if (ParticleCollideWithGround && environment.IsParticleCollidingAt(test_pos, velocity, false, collide_pos, collide_normal, landscape_collision)) {
            if (GetParticleEmitterManager().GetEnableDecals() && ParticleCreateDecal) {
                CDecal* decal = system.AllocateDecal();
                if (decal != 0) {
                    decal->SetLandscapeOnlyDecal(landscape_collision);
                    decal->SetMaxLife(-1);
                    decal->SetLifetimer(0);
                    decal->SetPosition(collide_pos);
                    CRightHandedSet rhset;
                    rhset.SetUp(collide_normal);
                    if (landscape_collision) {
                        C3DVector cross_vec(0.0f,0.0f,1.0f);
                    if (fabs(collide_normal.Z)>0.99f) {
                        if (fabs(collide_normal.X)>=fabs(collide_normal.Y)) cross_vec.Initialise(0.0f,1.0f,0.0f);
                        else cross_vec.Initialise(1.0f,0.0f,0.0f);
                    }
                    rhset.SetForward((collide_normal ^ cross_vec) ^ collide_normal);
                        rhset.MakePerpendicular();
                    }
                    decal->SetRightHandedSet(rhset);
                    decal->Activate();
                    decal->SetFlickerIndex(GFRandom(256, &seed));
                    decal->SetFlickerTimeOffset(GFRandom(256, &seed));
                }
            }
            if (ParticleCreateDecalEmitter && !DecalEmitterName.IsEmpty()) {
                long type = GetParticleEmitterDatabase().GetEmitterTemplateHandleFromName(DecalEmitterName);
                if (type != 0) {
                    CParticleEmitter* pemitter = GetParticleEmitterManager().CreateDecalEmitter(type, collide_pos, true,
                        CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
                    if (pemitter != 0)
                        DecalEmitterList.push_back(pemitter);
                }
            }
            if (ParticleDieOnCollision) {
                it_particle = system.DeleteParticle(it_particle);
                continue;
            }
            system.InverseTransformPosIfLocal(collide_pos);
            pos = collide_pos;
            if (landscape_collision) {
                velocity.X *= ParticleBounce;
                velocity.Y *= ParticleBounce;
                velocity.Z = -(velocity.Z * ParticleBounce);
            } else {
                CQuaternion rot;
                rot.SetFromAxisAngle(collide_normal, 0.5f);
                CMatrix3x4 rot_mat;
                rot.ToMatrix(rot_mat, C3DVector(0.0f, 0.0f, 0.0f));
                rot_mat.TransformPoint(velocity);
                velocity = velocity * ParticleBounce;
            }
        }
        if (SetOrientationFromDirection || SetOrientationFromGame) {
            C3DVector movement_direction(0.0f, 0.0f, 0.0f);
            if (SetOrientationFromGame)
                params.GetVectorParam(movement_direction, OrientFromGameParam);
            else {
                if (velocity.IsNonZero())
                    movement_direction = velocity;
                if (StayWithEmitter)
                    movement_direction += system.PeekEmitterVelocity();
            }
            if (movement_direction.IsNonZero()) {
                movement_direction.AccurateNormalise();
                CRightHandedSet old_rhs;
                particle.GetOrientation().ToRightHandedSet(old_rhs);
                CRightHandedSet rhs;
                rhs.SetForward(movement_direction.AccurateNormalise());
                C3DVector cross_vec(0.0f,0.0f,1.0f);
                    if (fabs(movement_direction.Z)>0.99f) {
                        if (fabs(movement_direction.X)>fabs(movement_direction.Y)) cross_vec.Initialise(0.0f,1.0f,0.0f);
                        else cross_vec.Initialise(1.0f,0.0f,0.0f);
                    }
                    rhs.SetUp((movement_direction ^ cross_vec) ^ movement_direction);
                rhs.MakePerpendicular();
                CMatrix3x4 rot_mat;
                rot_mat.Set(rhs, C3DVector(0.0f, 0.0f, 0.0f));
                CQuaternion orient;
                orient.SetFromRotationMatrix(rot_mat);
                particle.SetOrientation(orient);
                CQuaternion id;
                id.SetIdentity();
                particle.SetOrientationChange(id);
            }
        } else {
            particle.UpdateOrientation();
        }
        particle.UpdateAngle();
        particle.SetPosition(pos);
        particle.SetVelocity(velocity);
        particle.ClearAcceleration();
        particle.IncrementLifetimer();
        ++it_particle;
        keep_alive = true;
    }
    for (std::list<CIntelligentPointer<CParticleEmitter> >::iterator emitterIterator = DecalEmitterList.begin();
         emitterIterator != DecalEmitterList.end();) {
        if ((*emitterIterator).Get() != 0 && !(*emitterIterator).Get()->IsDead()) {
            (*emitterIterator).Get()->Update();
            ++emitterIterator;
        } else
            emitterIterator = DecalEmitterList.erase(emitterIterator);
    }
    ++LifeTimer;
}
}
