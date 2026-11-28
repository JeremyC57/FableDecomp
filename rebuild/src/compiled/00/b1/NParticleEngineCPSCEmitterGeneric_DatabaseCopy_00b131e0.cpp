// Leaf: ?DatabaseCopy@CPSCEmitterGeneric@NParticleEngine@@UAEXABVCPSCBase@2@@Z
// Flags: /Gr
// FableWin 02F4B7D0 and Ego_r 00AD3780 identify the DatabaseCopy override.
// Retail 00B131E0 is slot 18 of the CPSCEmitterGeneric vtable at 012A002C.
#define FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR
#define FABLE_RETAIL_CPSCEMITTERGENERIC_ALLOC
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CCARDINALSPLINE_APPENDCONTROLPOINT
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCardinalSpline.h"
#include "engine/CPSCEmitterGeneric.h"
#include "engine/GFDebugPDynamicCast.h"

namespace NParticleEngine {
void CPSCEmitterGeneric::DatabaseCopy(const CPSCBase& copy_from)
{
    const CPSCEmitterGeneric* psc =
        GFDebugPDynamicCast(&copy_from, static_cast<const CPSCEmitterGeneric*>(0));

    EmitterType = psc->EmitterType;
    EmitterPosParam = psc->EmitterPosParam;
    EmitterSize = psc->EmitterSize;
    Solid = psc->Solid;
    RadialBias = psc->RadialBias;
    UseEmitterLifeSecs = psc->UseEmitterLifeSecs;
    EmitterLifeSecs = psc->EmitterLifeSecs;
    EmitterStartTime = psc->EmitterStartTime;
    UseEmitterTimelineSecs = psc->UseEmitterTimelineSecs;
    EmitterTimelineSecs = psc->EmitterTimelineSecs;
    NoParticlesToStart = psc->NoParticlesToStart;
    NoParticlesToStartRand = psc->NoParticlesToStartRand;
    ParticlesPerSecond = psc->ParticlesPerSecond;
    OrientationXY = psc->OrientationXY;
    OrientationXZ = psc->OrientationXZ;
    OrientationYZ = psc->OrientationYZ;
    NonUniformScalingX = psc->NonUniformScalingX;
    NonUniformScalingY = psc->NonUniformScalingY;
    NonUniformScalingZ = psc->NonUniformScalingZ;
    MinSpeed = psc->MinSpeed;
    MaxSpeed = psc->MaxSpeed;
    UseCustomDirection = psc->UseCustomDirection;
    UseOutwardDirection = psc->UseOutwardDirection;
    UseForwardDirection = psc->UseForwardDirection;
    UseRandom2DDirection = psc->UseRandom2DDirection;
    UseRandom3DDirection = psc->UseRandom3DDirection;
    UseParamDirection = psc->UseParamDirection;
    OppositeDirection = psc->OppositeDirection;
    DirectionParamName = psc->DirectionParamName;
    CustomDirectionX = psc->CustomDirectionX;
    CustomDirectionY = psc->CustomDirectionY;
    CustomDirectionZ = psc->CustomDirectionZ;
    AngularPerturbationInteger = psc->AngularPerturbationInteger;

    Spline = psc->Spline;
    if (!Spline.IsNull()) {
        while (Spline->GetControlPointCount() < 2) {
            C3DVector new_pt(0.0f, 0.0f, 0.0f);
            Spline->AppendControlPoint(new_pt);
        }
    }
}
}