// NParticleEngine::CPSCEmitterGeneric::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info) -- vtable slot 17: persist every emitter field by name (the quantised floats rounded to two decimals, the bool / long bit-fields, the spline control points, with the legacy point-array names on load)
// Grade: DIFFER(5268v5268) -- 16 masked lines: the bool-default byte stores (`mov byte [esp+13h],bl`) of UseEmitterTimelineSecs and OrientationXZ are scheduled one Transfer earlier in retail, the NonUniformScalingY top-field store swaps the OR operands (retail `and edx,1FFFFFh` 6 bytes v our `and eax` 5, shifting one alignment nop), and the OppositeDirection `and ecx,7FFFFFFFh` sits elsewhere. Next idea: give the two early bools their own scope shape (a named `const bool defaultValue = false` declared before the previous field), and spell NonUniformScalingY's encode as a separate unsigned long local before the bit-field store.
// Leaf: ?DatabaseSerialise@CPSCEmitterGeneric@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B13530, 5268 bytes, thiscall, `ret 8`. FableWin.pdb
//   ?DatabaseSerialise@CPSCEmitterGeneric@NParticleEngine@@UAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@2@@Z; slot 17 of the
//   CPSCEmitterGeneric vtable 0x012A002C (slot 17 = DatabaseSerialise on the landed CPSCSpline 0x00B1B2E0 / CPSCAttractor 0x00B18110).
// Body: each quantised float is decoded (unit = Field / max; RoundToDecimals(unit * range [- offset], 2)), transferred with its default
//   and re-encoded (GFFloatToLongNear((RoundToDecimals(v, 2) - min) / range * max)); bool / long bit-fields go through a local; on save
//   the emitter / direction param names and the spline (tension, EmitterSplineUserPointsArraySize_BugFixed, one
//   EmitterSplineUserPointsArray_<i> per point); on load the CRC names, a new CCardinalSpline for EmitterType 3, the legacy
//   EmitterSplineUserPointsArraySize (clamped to 20) / EmitterSplineUserPointsArray<i>, and at least two control points.
// Callees: CPersistContext::Transfer overloads, CCharString ctors / dtor / operator+ 0x0099F570, GFIntToCharString 0x0099F830,
//   CCRC::Calc 0x004014A0, CCountedPointer<CCardinalSpline>::Reset 0x0075FA30, CArray<C3DVector> copy / operator= 0x0075B8D0.
// Helpers: GFFloatToLongNear (engine/GFFloatToLongNear.h) in RoundToDecimals and the encodes -- bare `fistp dword`, proven impossible
//   from C++ (same helper as the landed CPSCEmitterGeneric rows).
// Header overlay: FABLE_RETAIL_STLP_VECTOR_C (visible _vector.c operator=) and FABLE_RETAIL_STLP_VECTOR (_M_fill_insert) in
//   stlport/fable_containers.h; local CCharString / CCardinalSpline models (header debt).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/CPersistContext.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/GFFloatToLongNear.h"
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
class CCharStringData { public:
    const char* PData;                                  // +0
    unsigned long StringLength;                         // +4
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                                  // 0x0099E4B0
    CCharString(const char* string, long length = -1);              // 0x0099EBF0
    CCharString(const CCharString& other);                          // 0x0099EC30
    ~CCharString();                                                 // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    unsigned long GetCRC() const { return CCRC::Calc(0, GetPString(), GetLength()); }
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
static inline CCharString EmptyCCharString()
{
    CCharString empty;
    return empty;
}
class CCardinalSpline { public:
    CArray<C3DVector> ControlPoints;                    // +0x00 (12 bytes in retail)
    float Tension;                                      // +0x0c
    bool EndPointsConnected;                            // +0x10
    CCardinalSpline() : Tension(0.5f), EndPointsConnected(false) { }
};
static __forceinline float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    long i = decimals;
    do
        scale *= 10.0f;
    while (--i);
    float scaled = value * scale;
    return GFFloatToLongNear(scaled) / scale;
}
namespace NParticleEngine {
class CParticleSystem;
class CEmitterDatabaseSerialiseInfo;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&) = 0;
};
class CParticleUpdateParamDesc { public: unsigned long Param; unsigned long Name; unsigned long Type; };
class CPSCEmitterGeneric : public CPSCBase { public:
    CParticleUpdateParamDesc EmitterPosParamDesc;       // +0x0c
    CCountedPointer<CCardinalSpline> Spline;            // +0x18
    unsigned long EmitterPosParam;                      // +0x20
    unsigned long DirectionParamName;                   // +0x24
    long LifeTimer;                                     // +0x28
    float NextGenerateTime;                             // +0x2c
    unsigned long NoParticlesToStart : 10;              // +0x30
    unsigned long NoParticlesToStartRand : 10;
    unsigned long EmitterType : 2;
    unsigned long Solid : 1;
    unsigned long UseEmitterLifeSecs : 1;
    unsigned long UseEmitterTimelineSecs : 1;
    unsigned long OrientationXY : 1;
    unsigned long OrientationXZ : 1;
    unsigned long OrientationYZ : 1;
    unsigned long UseCustomDirection : 1;
    unsigned long UseOutwardDirection : 1;
    unsigned long UseParamDirection : 1;
    unsigned long OppositeDirection : 1;
    unsigned long AngularPerturbationInteger : 10;      // +0x34
    unsigned long CustomDirectionX : 8;
    unsigned long ParticlesPerSecond : 14;
    unsigned long EmitterSize : 10;                     // +0x38
    unsigned long RadialBias : 10;
    unsigned long MinSpeed : 10;
    unsigned long UseForwardDirection : 1;
    unsigned long UseRandom2DDirection : 1;
    unsigned long CustomDirectionY : 8;                 // +0x3c
    unsigned long CustomDirectionZ : 8;
    unsigned long EmitterTimelineSecs : 15;
    unsigned long UseRandom3DDirection : 1;
    unsigned long EmitterLifeSecs : 15;                 // +0x40
    unsigned long EmitterStartTime : 15;
    unsigned long MaxSpeed : 10;                        // +0x44
    unsigned long NonUniformScalingX : 11;
    unsigned long NonUniformScalingY : 11;
    unsigned short NonUniformScalingZ : 11;             // +0x48
    unsigned long StartedTimer : 1;                     // +0x4c
    unsigned long FinishedGenerating : 1;
    virtual void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info);
};
#define TRANSFER_BOOL_BIT(name, member) { bool value = member; context.Transfer(name, value, false); member = value; }
#define TRANSFER_LONG_BITS(name, member) { long value = member; context.Transfer(name, value, 0L); member = value; }
void CPSCEmitterGeneric::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo&)
{
    {
        float unit = (float)EmitterSize * (1.0f / 1023.0f);
        float value = RoundToDecimals(unit * 10.0f, 2);
        context.Transfer("EmitterSize", value, 1.0f);
        float scaled = RoundToDecimals(value, 2) * (1.0f / 10.0f);
        EmitterSize = GFFloatToLongNear(scaled * 1023.0f);
    }
    {
        float unit = (float)RadialBias * (1.0f / 1023.0f);
        float value = RoundToDecimals(unit * 10.0f - 5.0f, 2);
        context.Transfer("RadialBias", value, 0.0f);
        float scaled = (RoundToDecimals(value, 2) - -5.0f) * (1.0f / 10.0f);
        RadialBias = GFFloatToLongNear(scaled * 1023.0f);
    }
    {
        float unit = (float)EmitterLifeSecs * (1.0f / 32767.0f);
        float value = RoundToDecimals(unit * 300.0f, 2);
        context.Transfer("EmitterLifeSecs", value, 10.0f);
        float scaled = RoundToDecimals(value, 2) * (1.0f / 300.0f);
        EmitterLifeSecs = GFFloatToLongNear(scaled * 32767.0f);
    }
    {
        float unit = (float)EmitterStartTime * (1.0f / 32767.0f);
        float value = RoundToDecimals(unit * 300.0f, 2);
        context.Transfer("EmitterStartTime", value, 0.0f);
        float scaled = RoundToDecimals(value, 2) * (1.0f / 300.0f);
        EmitterStartTime = GFFloatToLongNear(scaled * 32767.0f);
    }
    {
        float unit = (float)EmitterTimelineSecs * (1.0f / 32767.0f);
        float value = RoundToDecimals(unit * 300.0f, 2);
        context.Transfer("EmitterTimelineSecs", value, 30.0f);
        float scaled = RoundToDecimals(value, 2) * (1.0f / 300.0f);
        EmitterTimelineSecs = GFFloatToLongNear(scaled * 32767.0f);
    }
    {
        float unit = (float)ParticlesPerSecond * (1.0f / 16383.0f);
        float value = RoundToDecimals(unit * 100.0f, 2);
        context.Transfer("ParticlesPerSecond", value, 3.0f);
        float scaled = RoundToDecimals(value, 2) * (1.0f / 100.0f);
        ParticlesPerSecond = GFFloatToLongNear(scaled * 16383.0f);
    }
    {
        C3DVector value;
        float unitX = (float)NonUniformScalingX * (1.0f / 2047.0f);
        value.X = RoundToDecimals(unitX * 20.0f - 10.0f, 2);
        float unitY = (float)NonUniformScalingY * (1.0f / 2047.0f);
        value.Y = RoundToDecimals(unitY * 20.0f - 10.0f, 2);
        float unitZ = (float)NonUniformScalingZ * (1.0f / 2047.0f);
        value.Z = RoundToDecimals(unitZ * 20.0f - 10.0f, 2);
        context.Transfer("NonUniformScaling", value, C3DVector(1.0f, 1.0f, 1.0f));
        float scaledX = (RoundToDecimals(value.X, 2) - -10.0f) * (1.0f / 20.0f);
        NonUniformScalingX = GFFloatToLongNear(scaledX * 2047.0f);
        float scaledY = (RoundToDecimals(value.Y, 2) - -10.0f) * (1.0f / 20.0f);
        NonUniformScalingY = GFFloatToLongNear(scaledY * 2047.0f);
        float scaledZ = (RoundToDecimals(value.Z, 2) - -10.0f) * (1.0f / 20.0f);
        NonUniformScalingZ = GFFloatToLongNear(scaledZ * 2047.0f);
    }
    {
        float unit = (float)MinSpeed * (1.0f / 1023.0f);
        float value = RoundToDecimals(unit * 10.0f, 2);
        context.Transfer("MinSpeed", value, 0.0f);
        float scaled = RoundToDecimals(value, 2) * (1.0f / 10.0f);
        MinSpeed = GFFloatToLongNear(scaled * 1023.0f);
    }
    {
        float unit = (float)MaxSpeed * (1.0f / 1023.0f);
        float value = RoundToDecimals(unit * 10.0f, 2);
        context.Transfer("MaxSpeed", value, 0.0f);
        float scaled = RoundToDecimals(value, 2) * (1.0f / 10.0f);
        MaxSpeed = GFFloatToLongNear(scaled * 1023.0f);
    }
    {
        C3DVector value;
        float unitX = (float)CustomDirectionX * (1.0f / 255.0f);
        value.X = RoundToDecimals(unitX * 2.0f - 1.0f, 2);
        float unitY = (float)CustomDirectionY * (1.0f / 255.0f);
        value.Y = RoundToDecimals(unitY * 2.0f - 1.0f, 2);
        float unitZ = (float)CustomDirectionZ * (1.0f / 255.0f);
        value.Z = RoundToDecimals(unitZ * 2.0f - 1.0f, 2);
        context.Transfer("CustomDirection", value, C3DVector(0.0f, 0.0f, 1.0f));
        float scaledX = (RoundToDecimals(value.X, 2) - -1.0f) * (1.0f / 2.0f);
        CustomDirectionX = GFFloatToLongNear(scaledX * 255.0f);
        float scaledY = (RoundToDecimals(value.Y, 2) - -1.0f) * (1.0f / 2.0f);
        CustomDirectionY = GFFloatToLongNear(scaledY * 255.0f);
        float scaledZ = (RoundToDecimals(value.Z, 2) - -1.0f) * (1.0f / 2.0f);
        CustomDirectionZ = GFFloatToLongNear(scaledZ * 255.0f);
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
        if (Spline.Data != 0) {
            float tension = Spline->Tension;
            context.Transfer("EmitterSplineTension", tension, 0.0f);
            long count = Spline->ControlPoints.size();
            context.Transfer("EmitterSplineUserPointsArraySize_BugFixed", count, 0L);
            CArray<C3DVector> points(Spline->ControlPoints);
            long i = 0;
            if (i < count) {
                C3DVector zero(0.0f, 0.0f, 0.0f);
                do {
                    context.Transfer((CCharString("EmitterSplineUserPointsArray_") + GFIntToCharString(i)).GetPString(), points[i], zero);
                } while (++i < count);
            }
        }
    } else {
        CCharString name;
        context.Transfer("EmitterPosParam", name, EmptyCCharString());
        EmitterPosParam = name.GetCRC();
        context.Transfer("DirectionParamName", name, EmptyCCharString());
        DirectionParamName = name.GetCRC();
        if (EmitterType == 3) {
            Spline.Reset(new CCardinalSpline);
            float tension;
            context.Transfer("EmitterSplineTension", tension, 0.0f);
            Spline->Tension = tension;
            CArray<C3DVector> points;
            long count;
            context.Transfer("EmitterSplineUserPointsArraySize_BugFixed", count, 0L);
            if (count != 0) {
                points.resize(count);
                long i = 0;
                if (i < count) {
                    C3DVector zero(0.0f, 0.0f, 0.0f);
                    do {
                        context.Transfer((CCharString("EmitterSplineUserPointsArray_") + GFIntToCharString(i)).GetPString(), points[i], zero);
                    } while (++i < count);
                }
            } else {
                context.Transfer("EmitterSplineUserPointsArraySize", count, 0L);
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
            Spline->ControlPoints = points;
        }
    }
}
}
