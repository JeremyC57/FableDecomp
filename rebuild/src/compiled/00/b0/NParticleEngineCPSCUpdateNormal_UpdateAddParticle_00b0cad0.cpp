// 00b0cad0 NParticleEngine::CPSCUpdateNormal::UpdateAddParticle
// NParticleEngine::CPSCUpdateNormal::UpdateAddParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector position, C3DVector velocity, float interpolate) -- vtable slot 12: spawn one pparticle (emission fade gate, lifetime, position history, creation time, initial initial_orientation from the fixed / random rotation or from the velocity / game vector, rotation axis and speed, flicker bytes)
// Leaf: ?UpdateAddParticle@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@VC3DVector@@2M@Z
// Inline-budget: natural leaves Activate, Matrix::Set and ToRightHandedSet called; T200 leaves ToRightHandedSet; T400 expands all three as retail.
// Flags: /d2inlT400
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0CAD0, 2191 bytes, thiscall, `ret 24h`.
// Identity: FableWin.pdb ?UpdateAddParticle@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@VC3DVector@@2M@Z;
//   slot 12 of the CPSCUpdateNormal vtable 0x0129FC1C (CPSCBase slot 12 0x00AF2A00 is the empty base hook; ret 24h = 4+4+12+12+4 bytes).
// Callees: NParticleEngine::GetRandomSeed 0x00AEAAD0, CPSCUpdateNormal::GetCurrentFadeValue 0x00B0A750 (landed), GFFloatRandom 0x004978A0,
//   CParticleSystem::AllocateParticle 0x00AF5010, NParticleEngine::GetTickRate 0x00AEA9F0, memmove (0x00BFEAE6 thunk), __ftol2,
//   C3DVector::GetRandomUnitVector 0x00A145C0, CQuaternion::SetFromAxisAngle 0x00A88D10, CQuaternion::operator* 0x00A88B60,
//   CParticleUpdateParameters<K,V>::GetVectorParam 0x00B059F0, CParticleSystem::PeekEmitterVelocity 0x00AF3DB0,
//   C3DVector::AccurateNormalise 0x00A14440, CRightHandedSet::MakePerpendicular 0x00A22E10, CQuaternion::SetFromRotationMatrix 0x00A88DE0,
//   GFROR13 0x00497890 (inside the inline GFRandom).
// Retail-asm: 00B0CBD0 and00B0D310 fistp dword ptr[edx]; canonical shared GFFastFloatToLong macro with portable twin. See ROUNDING_ATTEMPTS.md.
// Helpers: GFFastFloatToLong (engine/GFFastFloatToLong.h) in the inline CParticle::SetCreationTime / SetAngleChange (out-of-line copies
//   0x00B0A4A0 / 0x00B0A420 landed): retail 0x00B0CBD0 / 0x00B0D310 `fistp dword ptr [edx]`, a bare dword fistp no VC7.1 C++ form emits.
// Original fixed-X argument: donor02F42899..02F428AF assigns initial_angle then reloads InitialRotationX for SetFromAxisAngle; this restores the native local weight and homes.
// Layout: CParticle (Ego_r.pdb): Orientation +0x04, OrientationChange +0x14, Angle +0x24, AngleChange +0x28, PositionBuffer[4] +0x29,
//   Velocity +0x59, Acceleration +0x65 (1-aligned C3DVector: the vectors under pack(1)), Lifetimer +0x72, CreationTime +0x74, MaxLife +0x76,
//   FlickerIndex / FlickerTimeOffset +0x78 / +0x79, Active +0x7a, ClosestAttractorDistance +0x7c. CPSCUpdateNormal as the landed ctor.
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_CQUATERNION_PARTICLE_ADD_SHARED_SQUARE
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_C3DVECTOR_CROSS_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_CLEAR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_GFFASTFLOATTOLONG_PARTICLE_MACRO
#define FABLE_RETAIL_CPARTICLE_UPDATE_ADD
#define FABLE_RETAIL_CPSCUPDATENORMAL_UPDATE_ADD
#define FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#define FABLE_RETAIL_CQUATERNION_PARTICLE_ADD_FLOAT_SCALE
#define FABLE_RETAIL_CQUATERNION_PARTICLE_ADD
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4_DATA_UNION
#define FABLE_RETAIL_CMATRIX3X4_SET_ORIGINAL_NAMES
#define FABLE_RETAIL_CMATRIX3X4_PARTICLE_ADD_SET
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CPersistContext.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CRightHandedSet.h"
#include "engine/CParticleSystem.h"
#include "engine/GFFastFloatToLong.h"
#include <math.h>
#include <string.h>
#include <float.h>
#pragma intrinsic(fabs)
#include "engine/CPSCUpdateNormal.h"
#include "engine/CParticle.h"
#include "engine/CParticleUpdateParameters.h"
#include "engine/GFFloatRandom.h"
#include "engine/GFRandom.h"
#include "engine/GFGetInterpolatedValue.h"
namespace NParticleEngine {
long __fastcall GetTickRate();
unsigned long& __fastcall GetRandomSeed();
void CPSCUpdateNormal::UpdateAddParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                         C3DVector position, C3DVector velocity, float interpolate)
{
    unsigned long& seed = GetRandomSeed();
    if (EmissionFadeEnable) {
        float fade_val = GetCurrentFadeValue();
        fade_val = EmissionFadeMinimum + (1.0f - EmissionFadeMinimum) * fade_val;
        if (GFFloatRandom(1.0f, &seed) > fade_val)
            return;
    }
    CParticle* pparticle = system.AllocateParticle();
    if (pparticle == 0)
        return;
    float tick_rate = (float)GetTickRate();
    pparticle->SetClosestAttractorDistance(FLT_MAX);
    pparticle->SetPosition(position);
    pparticle->ClearAcceleration();
    pparticle->SetMaxLife((short)(tick_rate * ParticleLifeSecs));
    pparticle->SetLifetimer(-1);
    pparticle->SetCreationTime(interpolate);
    pparticle->Activate();
    pparticle->SetVelocity(velocity);

    CQuaternion initial_orientation;
    float initial_angle = 0.0f;
    if (SetOrientationFromDirection || SetOrientationFromGame) {
        C3DVector movement_direction(0.0f, 0.0f, 0.0f);
        if (SetOrientationFromGame) {
            params.GetVectorParam(movement_direction, OrientFromGameParam);
        } else {
            if (velocity.IsNonZero())
                movement_direction = velocity;
            if (StayWithEmitter)
                movement_direction += system.PeekEmitterVelocity();
        }
        if (movement_direction.IsNonZero()) {
            movement_direction.AccurateNormalise();
            CRightHandedSet rhs;
            rhs.SetForward(movement_direction.AccurateNormalise());
            C3DVector cross_vec(0.0f,0.0f,1.0f);
            if(fabs(movement_direction.Z)>0.99f) {
                if(fabs(movement_direction.X)>fabs(movement_direction.Y)) cross_vec.Initialise(0.0f,1.0f,0.0f);
                else cross_vec.Initialise(1.0f,0.0f,0.0f);
            }
            rhs.SetUp((movement_direction ^ cross_vec) ^ movement_direction);
            rhs.MakePerpendicular();
            CMatrix3x4 rot_mat;
            rot_mat.Set(rhs, C3DVector(0.0f, 0.0f, 0.0f));
            initial_orientation.SetFromRotationMatrix(rot_mat);
        } else {
            initial_orientation.SetIdentity();
        }
    } else {
        if (UseRandomInitialRotation) {
            C3DVector axis = C3DVector::GetRandomUnitVector(&seed);
            initial_angle = GFFloatRandom(1.0f, &seed);
            initial_orientation.SetFromAxisAngle(axis, initial_angle);
        } else {
            CQuaternion rot_x,rot_y,rot_z;
            initial_angle = InitialRotationX;
            rot_x.SetFromAxisAngle(C3DVector(1.0f, 0.0f, 0.0f), InitialRotationX);
            rot_y.SetFromAxisAngle(C3DVector(0.0f, 1.0f, 0.0f), InitialRotationY);
            rot_z.SetFromAxisAngle(C3DVector(0.0f, 0.0f, 1.0f), InitialRotationZ);
            initial_orientation = rot_x * rot_y * rot_z;
        }
        }
    CQuaternion orientation_change;
    float angle_change = GFGetInterpolatedValue(RotationMinAngleSpeed,RotationMaxAngleSpeed,GFFloatRandom(1.0f,&seed));
    C3DVector rotation_axis;
    if (SetOrientationFromDirection || SetOrientationFromGame) {
        orientation_change.SetIdentity();
        CRightHandedSet rhs;
        initial_orientation.ToRightHandedSet(rhs);
        rotation_axis = rhs.GetForward();
    }
    if (UseRandomRotationAxis) {
        rotation_axis = C3DVector::GetRandomUnitVector(&seed);
        orientation_change.SetFromAxisAngle(rotation_axis, angle_change);
    } else if (RotationAxis.IsNonZero()) {
        rotation_axis = RotationAxis;
        rotation_axis.Normalise();
        orientation_change.SetFromAxisAngle(rotation_axis, angle_change);
    } else {
        orientation_change.SetIdentity();
    }
    pparticle->SetOrientation(initial_orientation);
    pparticle->SetOrientationChange(orientation_change);
    pparticle->SetAngle(initial_angle);
    pparticle->SetAngleChange(angle_change);
    pparticle->SetFlickerIndex((unsigned char)GFRandom(256, &seed));
    pparticle->SetFlickerTimeOffset((unsigned char)GFRandom(256, &seed));
}
}