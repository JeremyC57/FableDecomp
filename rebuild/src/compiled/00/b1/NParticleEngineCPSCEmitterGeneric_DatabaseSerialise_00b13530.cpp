// NParticleEngine::CPSCEmitterGeneric::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo&) -- serialize emitter database fields
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Leaf: ?DatabaseSerialise@CPSCEmitterGeneric@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Retail: 0x00B13530, 5268 bytes, thiscall.
// Identity: both PDB owner records; native vtable012A002C slot17 and neighbouring binary readers/writers.
// Retail-asm: shared GFFloatToLongNear implements original dword FPU rounding;
// portable branch retained in existing header; no new machine-code body here.
#define FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT
#define FABLE_RETAIL_GFUNPACKFLOAT_GROUP_PRODUCT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#include "engine/CCRC.h"
#include "fable_string.h"
#include "engine/CPersistContext.h"
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/GFPackFloat.h"
#include "engine/CCardinalSpline.h"
#include "engine/CPSCEmitterGeneric.h"
CCharString FABLE_FASTCALL GFIntToCharString(long);
template<> inline CCharString FABLE_FASTCALL CPersistContext::GetDefaultVal<CCharString>() {CCharString temp;return temp;}
template<> inline float FABLE_FASTCALL CPersistContext::GetDefaultVal<float>() {return 0.0f;}
template<> inline long FABLE_FASTCALL CPersistContext::GetDefaultVal<long>() {return 0;}
namespace NParticleEngine {
// Both original names and donor02F18500/02F36290: same CRC computation, distinct functions.
inline unsigned long FABLE_FASTCALL MAKE_UPDATE_PARAM_FROM_BASE(unsigned long base,const CCharString& text){return CCRC::Calc(base,text.GetAsCharArray(),text.GetLength());}
inline unsigned long FABLE_FASTCALL MAKE_VECTOR_PARAM_FROM_BASE(unsigned long base,const CCharString& text){return CCRC::Calc(base,text.GetAsCharArray(),text.GetLength());}
#define TRANSFER_BOOL_BIT(name, member) { bool value = member; context.Transfer(name, value, false); member = value; }
#define TRANSFER_LONG_BITS(name, member) { long value = member; context.Transfer(name, value, 0L); member = value; }
void CPSCEmitterGeneric::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo&)
{
    {
        float temp = GFUnpackFloat(EmitterSize, 10, 0.0f, 10.0f, 2);
        context.Transfer("EmitterSize", temp, 1.0f);
        EmitterSize = GFPackFloat(temp, 10, 0.0f, 10.0f, 2);
    }
    {
        float temp = GFUnpackFloat(RadialBias, 10, -5.0f, 5.0f, 2);
        context.Transfer("RadialBias", temp, 0.0f);
        RadialBias = GFPackFloat(temp, 10, -5.0f, 5.0f, 2);
    }
    {
        float temp = GFUnpackFloat(EmitterLifeSecs, 15, 0.0f, 300.0f, 2);
        context.Transfer("EmitterLifeSecs", temp, 10.0f);
        EmitterLifeSecs = GFPackFloat(temp, 15, 0.0f, 300.0f, 2);
    }
    {
        float temp = GFUnpackFloat(EmitterStartTime, 15, 0.0f, 300.0f, 2);
        context.Transfer("EmitterStartTime", temp, 0.0f);
        EmitterStartTime = GFPackFloat(temp, 15, 0.0f, 300.0f, 2);
    }
    {
        float temp = GFUnpackFloat(EmitterTimelineSecs, 15, 0.0f, 300.0f, 2);
        context.Transfer("EmitterTimelineSecs", temp, 30.0f);
        EmitterTimelineSecs = GFPackFloat(temp, 15, 0.0f, 300.0f, 2);
    }
    {
        float temp = GFUnpackFloat(ParticlesPerSecond, 14, 0.0f, 100.0f, 2);
        context.Transfer("ParticlesPerSecond", temp, 3.0f);
        ParticlesPerSecond = GFPackFloat(temp, 14, 0.0f, 100.0f, 2);
    }
    {
        C3DVector temp;
        temp.X = GFUnpackFloat(NonUniformScalingX, 11, -10.0f, 10.0f, 2);
        temp.Y = GFUnpackFloat(NonUniformScalingY, 11, -10.0f, 10.0f, 2);
        temp.Z = GFUnpackFloat(NonUniformScalingZ, 11, -10.0f, 10.0f, 2);
        context.Transfer("NonUniformScaling", temp, C3DVector(1.0f, 1.0f, 1.0f));
        NonUniformScalingX = GFPackFloat(temp.X, 11, -10.0f, 10.0f, 2);
        NonUniformScalingY = GFPackFloat(temp.Y, 11, -10.0f, 10.0f, 2);
        NonUniformScalingZ = GFPackFloat(temp.Z, 11, -10.0f, 10.0f, 2);
    }
    {
        float temp = GFUnpackFloat(MinSpeed, 10, 0.0f, 10.0f, 2);
        context.Transfer("MinSpeed", temp, 0.0f);
        MinSpeed = GFPackFloat(temp, 10, 0.0f, 10.0f, 2);
    }
    {
        float temp = GFUnpackFloat(MaxSpeed, 10, 0.0f, 10.0f, 2);
        context.Transfer("MaxSpeed", temp, 0.0f);
        MaxSpeed = GFPackFloat(temp, 10, 0.0f, 10.0f, 2);
    }
    {
        C3DVector temp;
        temp.X = GFUnpackFloat(CustomDirectionX, 8, -1.0f, 1.0f, 2);
        temp.Y = GFUnpackFloat(CustomDirectionY, 8, -1.0f, 1.0f, 2);
        temp.Z = GFUnpackFloat(CustomDirectionZ, 8, -1.0f, 1.0f, 2);
        context.Transfer("CustomDirection", temp, C3DVector(0.0f, 0.0f, 1.0f));
        CustomDirectionX = GFPackFloat(temp.X, 8, -1.0f, 1.0f, 2);
        CustomDirectionY = GFPackFloat(temp.Y, 8, -1.0f, 1.0f, 2);
        CustomDirectionZ = GFPackFloat(temp.Z, 8, -1.0f, 1.0f, 2);
    }
    TRANSFER_LONG_BITS("EmitterType", EmitterType);
    TRANSFER_BOOL_BIT("Solid", Solid);
    TRANSFER_BOOL_BIT("UseEmitterLifeSecs", UseEmitterLifeSecs);
    TRANSFER_BOOL_BIT("UseEmitterTimelineSecs", UseEmitterTimelineSecs);
    TRANSFER_LONG_BITS("NoParticlesToStart", NoParticlesToStart);
    TRANSFER_LONG_BITS("NoParticlesToStartRand", NoParticlesToStartRand);
    TRANSFER_BOOL_BIT("OrientationXY", OrientationXY);
    TRANSFER_BOOL_BIT("OrientationXZ", OrientationXZ);
    TRANSFER_BOOL_BIT("OrientationYZ", OrientationYZ);
    TRANSFER_BOOL_BIT("UseCustomDirection", UseCustomDirection);
    TRANSFER_BOOL_BIT("UseOutwardDirection", UseOutwardDirection);
    TRANSFER_BOOL_BIT("UseForwardDirection", UseForwardDirection);
    TRANSFER_BOOL_BIT("UseRandom2DDirection", UseRandom2DDirection);
    TRANSFER_BOOL_BIT("UseRandom3DDirection", UseRandom3DDirection);
    TRANSFER_BOOL_BIT("UseParamDirection", UseParamDirection);
    TRANSFER_BOOL_BIT("OppositeDirection", OppositeDirection);
    TRANSFER_LONG_BITS("AngularPerturbationInteger", AngularPerturbationInteger);
    if (context.IsSaving()) {
        context.Transfer("EmitterPosParam", EmitterPosParam, 0UL);
        context.Transfer("DirectionParamName", DirectionParamName, 0UL);
        if (!Spline.IsNull()) {
            float tension = Spline->GetTension();
            context.Transfer("EmitterSplineTension", tension);
            long count = Spline->GetControlPointCount();
            context.Transfer("EmitterSplineUserPointsArraySize_BugFixed", count);
            CArray<C3DVector> points = Spline->GetControlPoints();
            long i = 0;
            if (i < count) {
                C3DVector zero(0.0f, 0.0f, 0.0f);
                do {
                    context.Transfer((CCharString("EmitterSplineUserPointsArray_") + GFIntToCharString(i)).GetAsCharArray(), points[i], zero);
                } while (++i < count);
            }
        }
    } else {
        CCharString name;
        context.Transfer("EmitterPosParam", name);
        EmitterPosParam = MAKE_UPDATE_PARAM_FROM_BASE(0,name);
        context.Transfer("DirectionParamName", name);
        DirectionParamName = MAKE_VECTOR_PARAM_FROM_BASE(0,name);
        if (EmitterType == 3) {
            Spline.Reset(new CCardinalSpline);
            float tension;
            context.Transfer("EmitterSplineTension", tension);
            Spline->SetTension(tension);
            CArray<C3DVector> points;
            long count;
            context.Transfer("EmitterSplineUserPointsArraySize_BugFixed", count);
            if (count != 0) {
                points.resize(count);
                long i = 0;
                if (i < count) {
                    C3DVector zero(0.0f, 0.0f, 0.0f);
                    do {
                        context.Transfer((CCharString("EmitterSplineUserPointsArray_") + GFIntToCharString(i)).GetAsCharArray(), points[i], zero);
                    } while (++i < count);
                }
            } else {
                context.Transfer("EmitterSplineUserPointsArraySize", count);
                if (count > 20)
                    count = 20;
                points.resize(count);
                long i = 0;
                if (i < count) {
                    C3DVector zero(0.0f, 0.0f, 0.0f);
                    do {
                        context.Transfer("EmitterSplineUserPointsArray" + i, points[i], zero);
                    } while (++i < count);
                }
            }
            C3DVector zero(0.0f, 0.0f, 0.0f);
            while (points.size() < 2)
                points.push_back(zero);
            Spline->SetControlPoints(points);
        }
    }
}
}