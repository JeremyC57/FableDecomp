// NParticleEngine::CPSCEmitterGeneric::GenerateParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, float creationTime) -- emit one particle: pick a position / normal on the emitter shape, scale it, choose its direction and speed, move both into world space and hand them to the system's update component
// Leaf: ?GenerateParticle@CPSCEmitterGeneric@NParticleEngine@@AAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@M@Z
// Flags: /d2inlT158
// Inline-budget: /d2inlT158 -- CMatrix3x4::TransformVector (FableWin ?TransformVector@CMatrix3x4@@QBEXAAVC3DVector@@@Z, no retail out-of-line copy) is
//   expanded at both world / local sites in retail (0x00B119A1, 0x00B11A2B); VC7.1 calls it out of line at the default threshold and at T<=157
//   (3952 / 4228 bytes); T158 is the smallest threshold that expands both (T158..T200 give the same RELOCATION_MATCH).
// Inferred: 11 dead release asserts (fable_assert.h FABLE_RETAIL_ASSERT, device d) after the decodes. They emit nothing; their dead arms re-key
//   VC7.1's record numbering, which picks the x87 term order of every reassociated three-product sum here: with 0..4 the world / local
//   TransformVector expansions come out identical and are tail-merged (4156 bytes), with 5..10 the expansion-1 Y / Z rows (0x00B119CF..
//   0x00B11A12), the TransformPoint X row (0x00B11ABB..) and the dot / normalise sites (0x00B114B1, 0x00B11517, 0x00B117F0) differ;
//   11..13 match, 14 merges again (tmp/b9/s12 / s14 sweeps; unused-local device c alone, k = 1..40, never matches). Evidence: the same
//   inline TransformVector body is summed in two different orders by retail (0x00B119A1 v 0x00B11A2B), which only the record numbering
//   explains; landed lane rows 0x00ACA830 / 0x00E3BB70 / 0x00E50FD0 close the same way. Conditions are plausible parameter checks (not
//   recoverable: the release build drops them), count is what is evidenced.
// FITTED: the parenthesised `return (GFFloatToLongNear(scaled) / scale);` in the row-local RoundToDecimals (one no-code PAREN tuple per decode
//   moves c2's 81-tuple scheduling-region cut so the radius `fstp [esp+54h]` is issued before the switch's `cmp eax, 3`, 0x00B11052);
//   GetScaled's named-result body (FABLE_RETAIL_C3DVECTOR_GETSCALED_NAMED_RESULT, FableWin 34i/90b v this 32i/84b, the plain body 28i/70b).
// EVIDENCED: GetScaled (FableWin ?GetScaled@C3DVector@@QBE?BV1@ABV1@@Z) for the non-uniform scaling (X operand order 0x00B1132E / 0x00B1136D);
//   the dot products as inline arguments (retail overwrites the dot with the last product, `fxch st(2); fmul [X]`, 0x00B114ED / 0x00B1179A);
//   the param-direction block as nested ifs with the custom copy in both else arms (seed reload before the je, 0x00B1169A); default
//   TransformVector text (FableWin 55i/150b SAME) and the donor TransformPoint text (FABLE_RETAIL_CMATRIX3X4_PARTICLE_POINT, 61i/168b SAME);
//   RoundToDecimals' loop as the landed sibling GetRandomRadius 0x00B102C0 writes it; ChangeOrientation 0x00B10360 defined above the row with
//   the landed row's text (retail calls it; its body visible un-escapes the locals passed by const reference).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B10B10, 4276 bytes incl. the 4-entry jump table, thiscall, `ret 0Ch`. Private: FableWin.pdb
//   ?GenerateParticle@CPSCEmitterGeneric@NParticleEngine@@AAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@M@Z.
// Body: decode the quantised emitter fields (RoundToDecimals(unit * range [- offset], 2)), switch on EmitterType (0 point, 1 random 2D
//   ring, 2 random 3D sphere, 3 spline point), radius scaled by GetRandomRadius when Solid, ChangeOrientation of direction / normal /
//   up, non-uniform scaling, direction from Random2D / Random3D / Outward / Forward (param callback) / Param (VectorParams) / custom,
//   OppositeDirection, angular perturbation, speed in [MinSpeed, MaxSpeed], emitter-position offset callback, world transforms,
//   then GetComponentOfType(update, 0)->UpdateAddParticle (CPSCBase slot 12).
// Callees: GetRandomSeed 0x00AEAAD0, C2DVector::GetRandomUnitVector 0x00A13EB0, C3DVector::GetRandomUnitVector 0x00A145C0,
//   GetRandomPointWithinUnitSphere 0x00A146C0, CCardinalSpline::GetSplinePosition 0x00A567A0, GFFloatRandom 0x004978A0,
//   GetRandomRadius 0x00B102C0, ChangeOrientation 0x00B10360, CVectorMap::LowerBound 0x00B05890, GetSystemTransform 0x00AF2250,
//   GetInverseSystemTransform 0x00AF22C0, GetComponentOfType 0x00AF4360.
// Descriptive names (no original found): GetUnitFloat (row-local decode helper), RoundToDecimals (the landed siblings' helper name).
// Retail-asm: 0x00B11009 `fld dword ptr [esp+1Ch]` + 0x00B1100D `fistp dword ptr [esp+30h]` (and every other decode of this body) -- the engine's
//   inline-assembly float-to-long helper GFFloatToLongNear (engine/GFFloatToLongNear.h, inside RoundToDecimals; the scaled value stored to the
//   helper's by-value parameter copy and reloaded for a bare dword fistp); VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`.
//   Same helper as the landed siblings GetRandomRadius 0x00B102C0 / InterpolateTo. This row carries no asm of its own.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_EMITTER
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_C3DVECTOR_CLEAR
#define FABLE_RETAIL_CMATRIX3X4_PARTICLE_POINT
#define FABLE_RETAIL_C3DVECTOR_GETSCALED_NAMED_RESULT
#define FABLE_RETAIL_C3DVECTOR_GETSCALED
#define FABLE_RETAIL_C3DVECTOR_RANDOMPOINTINSPHERE
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_GETRANDOMUNITVECTOR
#define FABLE_RETAIL_CCARDINALSPLINE_GETSPLINEPOSITION
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#define FABLE_RETAIL_CPARTICLESYSTEM_GENERATEPARTICLE
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_ISNULL
#define FABLE_RETAIL_CPSCEMITTERGENERIC_GENERATEPARTICLE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/C2DVector.h"
#include "engine/CCardinalSpline.h"
#include "engine/CParticleUpdateParameters.h"
#include "engine/CParticleSystem.h"
#include "engine/CPSCEmitterGeneric.h"
#include "engine/GFFloatToLongNear.h"
float FABLE_FASTCALL GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0 ?GFFloatRandom@@YIMMPAK@Z
static FABLE_FORCEINLINE float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    for (long i = decimals; i; --i)
        scale *= 10.0f;
    float scaled = value * scale;
    return (GFFloatToLongNear(scaled) / scale);
}
namespace NParticleEngine {
unsigned long& FABLE_FASTCALL GetRandomSeed();                               // 0x00AEAAD0 ?GetRandomSeed@NParticleEngine@@YIAAKXZ
static FABLE_FORCEINLINE float GetUnitFloat(const unsigned long& value, float oneOverMax) { return (float)value * oneOverMax; }

C3DVector CPSCEmitterGeneric::ChangeOrientation(const C3DVector& v)
{
    C3DVector result;
    if (OrientationXZ)
        result = C3DVector(v.X, -v.Z, v.Y);
    else if (OrientationYZ)
        result = C3DVector(-v.Z, v.X, v.Y);
    else
        result = v;
    return result;
}

void CPSCEmitterGeneric::GenerateParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, float creationTime)
{
    float emitterTimelineSecsUnit = GetUnitFloat(EmitterTimelineSecs, 1.0f / 32767.0f);
    float emitterTimelineSecs = RoundToDecimals(emitterTimelineSecsUnit * 300.0f, 2);
    float emitterSizeUnit = GetUnitFloat(EmitterSize, 1.0f / 1023.0f);
    float emitterSize = RoundToDecimals(emitterSizeUnit * 10.0f, 2);
    float radialBiasUnit = GetUnitFloat(RadialBias, 1.0f / 1023.0f);
    float radialBias = RoundToDecimals(radialBiasUnit * 10.0f - 5.0f, 2);
    float emitterLifeSecsUnit = GetUnitFloat(EmitterLifeSecs, 1.0f / 32767.0f);
    float emitterLifeSecs = RoundToDecimals(emitterLifeSecsUnit * 300.0f, 2);
    float emitterStartTimeUnit = GetUnitFloat(EmitterStartTime, 1.0f / 32767.0f);
    float emitterStartTime = RoundToDecimals(emitterStartTimeUnit * 300.0f, 2);
    float particlesPerSecondUnit = GetUnitFloat(ParticlesPerSecond, 1.0f / 16383.0f);
    float particlesPerSecond = RoundToDecimals(particlesPerSecondUnit * 100.0f, 2);
    float minSpeedUnit = GetUnitFloat(MinSpeed, 1.0f / 1023.0f);
    float minSpeed = RoundToDecimals(minSpeedUnit * 10.0f, 2);
    float maxSpeedUnit = GetUnitFloat(MaxSpeed, 1.0f / 1023.0f);
    float maxSpeed = RoundToDecimals(maxSpeedUnit * 10.0f, 2);
    float customZUnit = GetUnitFloat(CustomDirectionZ, 1.0f / 255.0f);
    float customZ = RoundToDecimals(customZUnit * 2.0f - 1.0f, 2);
    float customYUnit = GetUnitFloat(CustomDirectionY, 1.0f / 255.0f);
    float customY = RoundToDecimals(customYUnit * 2.0f - 1.0f, 2);
    float customXUnit = GetUnitFloat(CustomDirectionX, 1.0f / 255.0f);
    float customX = RoundToDecimals(customXUnit * 2.0f - 1.0f, 2);
    C3DVector customDirection(customX, customY, customZ);
    float scalingZUnit = GetUnitFloat(NonUniformScalingZ, 1.0f / 2047.0f);
    float scalingZ = RoundToDecimals(scalingZUnit * 20.0f - 10.0f, 2);
    float scalingYUnit = GetUnitFloat(NonUniformScalingY, 1.0f / 2047.0f);
    float scalingY = RoundToDecimals(scalingYUnit * 20.0f - 10.0f, 2);
    float scalingXUnit = GetUnitFloat(NonUniformScalingX, 1.0f / 2047.0f);
    float scalingX = RoundToDecimals(scalingXUnit * 20.0f - 10.0f, 2);
    C3DVector scaling(scalingX, scalingY, scalingZ);
    // Inferred (device d): eleven dead release asserts; see the header comment.
    FABLE_RETAIL_ASSERT(EmitterType < 4);
    FABLE_RETAIL_ASSERT(EmitterType != 3 || Spline.Get() != 0);
    FABLE_RETAIL_ASSERT(emitterSize >= 0.0f);
    FABLE_RETAIL_ASSERT(minSpeed <= maxSpeed);
    FABLE_RETAIL_ASSERT(emitterLifeSecs >= 0.0f);
    FABLE_RETAIL_ASSERT(particlesPerSecond >= 0.0f);
    FABLE_RETAIL_ASSERT(emitterStartTime >= 0.0f);
    FABLE_RETAIL_ASSERT(emitterTimelineSecs >= 0.0f);
    FABLE_RETAIL_ASSERT(radialBias >= -5.0f && radialBias <= 5.0f);
    FABLE_RETAIL_ASSERT(AngularPerturbationInteger <= 1000);
    FABLE_RETAIL_ASSERT(scalingX != 0.0f);
    C3DVector direction(0.0f, 0.0f, 0.0f);
    unsigned long* seed = &GetRandomSeed();
    float radius = emitterSize * 0.5f;
    C3DVector position;
    C3DVector normal;
    C3DVector up(0.0f, 0.0f, 1.0f);
    switch (EmitterType) {
    case 0:
        position.Clear();
        normal.Clear();
        break;
    case 1: {
        C2DVector random = C2DVector::GetRandomUnitVector(seed);
        position = C3DVector(random.X, random.Y, 0.0f);
        normal = position;
        float r = Solid ? GetRandomRadius(seed) * radius : radius;
        position *= r;
        direction = ChangeOrientation(direction);
        normal = ChangeOrientation(normal);
        up = ChangeOrientation(up);
        break;
    }
    case 2: {
        position = C3DVector::GetRandomUnitVector(seed);
        normal = position;
        up = position;
        float r = Solid ? GetRandomRadius(seed) * radius : radius;
        position *= r;
        direction = ChangeOrientation(direction);
        normal = ChangeOrientation(normal);
        up = ChangeOrientation(up);
        break;
    }
    case 3:
        Spline->GetSplinePosition(position, GFFloatRandom(1.0f, seed));
        normal = position;
        up = position;
        direction = ChangeOrientation(direction);
        normal = ChangeOrientation(normal);
        up = ChangeOrientation(up);
        break;
    }
    bool hasParamDirection = false;
    position = position.GetScaled(scaling);
    normal = normal.GetScaled(scaling);
    if (normal.IsNonZero())
        normal.Normalise();
    up = up.GetScaled(scaling);
    if (UseRandom2DDirection) {
        direction = C3DVector::GetRandomUnitVector(seed);
        direction -= up * up.Dot(direction);
        if (direction.IsNonZero())
            direction.Normalise();
    } else if (UseRandom3DDirection)
        direction = C3DVector::GetRandomUnitVector(seed);
    else if (UseOutwardDirection)
        direction = normal;
    else if (UseForwardDirection) {
        if (EmitterPosParamDesc.IsNull() || !EmitterPosParamDesc.GetUpdateParam(direction, creationTime, true, params, 1))
            direction.Clear();
    } else {
        if (UseParamDirection) {
            CVectorMap<unsigned long, C3DVector, CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> >::const_iterator it = params.VectorParams.Find(DirectionParamName);
            if (it != params.VectorParams.end()) {
                direction = it->second;
                hasParamDirection = true;
            } else
                direction = customDirection;
        } else
            direction = customDirection;
    }
    if (OppositeDirection)
        direction = -direction;
    if (AngularPerturbationInteger != 0) {
        float length = direction.GetMagnitude();
        float perturbation = (float)AngularPerturbationInteger * 0.001f;
        C3DVector random = C3DVector::GetRandomPointWithinUnitSphere(seed);
        random -= direction * random.Dot(direction);
        direction += random * perturbation;
        if (direction.IsNonZero()) {
            direction.Normalise();
            direction *= length;
        }
    }
    float speedRange = maxSpeed - minSpeed > 0.0f ? maxSpeed - minSpeed : 0.0f;
    direction *= GFFloatRandom(speedRange, seed) + minSpeed;
    if (!EmitterPosParamDesc.IsNull()) {
        C3DVector offset;
        if (EmitterPosParamDesc.GetUpdateParam(offset, creationTime, false, params, 1))
            position += offset;
    }
    if (hasParamDirection)
        system.InverseTransformVectorIfLocal(direction);
    else
        system.TransformVectorIfWorld(direction);
    system.TransformPosIfWorld(position);
    CPSCBase* update = system.GetComponentOfType((EPSCType)4, 0);
    if (update != 0)
        update->UpdateAddParticle(system, params, position, direction, creationTime);
}
}
