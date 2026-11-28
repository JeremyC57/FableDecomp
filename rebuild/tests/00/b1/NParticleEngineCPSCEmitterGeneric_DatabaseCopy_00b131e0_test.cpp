#define FABLE_RETAIL_CCARDINALSPLINE_CONTROLPOINT
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

#include <cstdio>
#include <cstdlib>
#include <cstring>

static void check(bool value) {
    if (!value) {
        std::puts("00b131e0_TEST FAIL");
        std::exit(1);
    }
}

// Initialize the actual private spline through the test-only constructor.
// An installed deletion callback records the object's real reference-count transition.
static unsigned int constructorSeed;
static int initialPoints;
static bool reservePoints;
static CCountedPointer<CCardinalSpline>* sharedInitial;
static CCountedPointer<CCardinalSpline>* retainInitial;
static int destroyed;
static void FABLE_FASTCALL destroySpline(void* data) {
    ++destroyed;
    delete static_cast<CCardinalSpline*>(data);
}

static unsigned long word(const void* object, unsigned int offset) {
    unsigned long value;
    std::memcpy(&value, static_cast<const unsigned char*>(object) + offset, sizeof(value));
    return value;
}
static CCPPointerInfo* info(const void* object) {
    CCPPointerInfo* value;
    std::memcpy(&value, static_cast<const unsigned char*>(object) + 0x1c, sizeof(value));
    return value;
}
static CCardinalSpline* spline(const void* object) {
    CCardinalSpline* value;
    std::memcpy(&value, static_cast<const unsigned char*>(object) + 0x18, sizeof(value));
    return value;
}

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}

namespace NParticleEngine {
CPSCEmitterGeneric::CPSCEmitterGeneric() {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(this);
    for (unsigned int offset = 0x20; offset < sizeof(*this); ++offset) {
        if (constructorSeed == 8u || constructorSeed == ~9u)
            bytes[offset] = 0;
        else if (constructorSeed == 9u || constructorSeed == ~8u)
            bytes[offset] = 0xff;
        else
            bytes[offset] = static_cast<unsigned char>(constructorSeed * 157u + offset * 35u);
    }
    if (sharedInitial) {
        Spline = *sharedInitial;
    } else if (initialPoints >= 0) {
        Spline.Reset(new CCardinalSpline());
        info(this)->DeleteFunc = destroySpline;
        if (reservePoints) {
            for (int i = 0; i < 4; ++i)
                Spline->AppendControlPoint(C3DVector(1, 2, 3));
            Spline->DeleteAllControlPoints();
        }
        for (int i = 0; i < initialPoints; ++i)
            Spline->AppendControlPoint(C3DVector(float(i+4), float(i+5), float(i+6)));
    }
    if (retainInitial) *retainInitial = Spline;
}

// Other virtual entries are test-only engine boundaries. A call to an unrelated
// entry fails the test immediately.
template<class K, class V> bool CParticleUpdateParamCallback<K,V>::GetUpdateParam(C3DVector&, V, float, bool, const CParticleUpdateParameters<K,V>&, unsigned long) { std::abort(); }
template<class K, class V> CParticleUpdateParamCallback<K,V>::~CParticleUpdateParamCallback() {}
template<class K, class V> CParticleUpdateParamDesc<K,V>::~CParticleUpdateParamDesc() {}
CPSCBase::~CPSCBase() {}
void CPSCEmitterGeneric::DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&) { std::abort(); }
void CPSCBase::OnCreate() { std::abort(); }
void CPSCBase::StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>&) { std::abort(); }
void CPSCBase::Update(CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,bool&) { std::abort(); }
void CPSCBase::CollectUpdateParameters(CParticleUpdateParameters<unsigned long, unsigned long>&,const unsigned long&) { std::abort(); }
void CPSCBase::TransferUpdateParametersToDialog(const CSymbolMap&) { std::abort(); }
void CPSCBase::UpdateAddParticle(CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,C3DVector,C3DVector,float) { std::abort(); }
void CPSCBase::PreparePrimitives(const float,const CEnginePrimitivePrepareInfo&,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,CEnginePrimitiveHandle&) { std::abort(); }
void CPSCBase::DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&) { std::abort(); }
bool CPSCBase::AddToBoundingSphere(C3DBoundingSphere&,const float,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&) { std::abort(); }
bool CPSCBase::GetParticleLifeSecs(float&) { std::abort(); }
bool CPSCBase::GetNumberOfParticles(float&,const float) { std::abort(); }
bool CPSCBase::GetDecalLifeSecs(float&) { std::abort(); }
bool CPSCBase::GetNumberOfDecals(float&,const float) { std::abort(); }
void CPSCBase::EnterTimelineEnd() { std::abort(); }
long CPSCBase::GetParticleCount() { std::abort(); }
long CPSCBase::GetDecalCount() { std::abort(); }
bool CPSCBase::Initialise(CIEngine*,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,CEnginePrimitiveHandle&) { std::abort(); }
void CPSCBase::ClearAllPrimitives(CIEngine*,CEnginePrimitiveHandle&) { std::abort(); }
bool CPSCBase::GetUserPoint(C3DVector&) const { std::abort(); }
float CPSCBase::GetApproximateSplineLength() { std::abort(); }
CPSCBase* CPSCEmitterGeneric::DatabaseClone() const { std::abort(); }
CCharString CPSCEmitterGeneric::GetClassTypeName() const { std::abort(); }
EPSCType CPSCEmitterGeneric::GetComponentType() const { std::abort(); }
EPSCSubType CPSCEmitterGeneric::GetComponentSubType() const { std::abort(); }
CCharString CPSCEmitterGeneric::GetGUIName() const { std::abort(); }
void CPSCEmitterGeneric::InterpolateTo(const CPSCBase&,const float) { std::abort(); }
void CPSCEmitterGeneric::ReadBinary(CDataInputStream&) { std::abort(); }
void CPSCEmitterGeneric::WriteBinary(CDataOutputStream&) const { std::abort(); }
void CPSCEmitterGeneric::GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float) { std::abort(); }
}

static void capture(unsigned long* words, const void* object) {
    for (unsigned int i = 0; i < 12; ++i) words[i] = word(object, 0x20 + 4*i);
}
static void verifyFields(const unsigned long* before, const unsigned long* source,
                         const NParticleEngine::CPSCEmitterGeneric& result) {
    static const unsigned long masks[] = {
        ~0ul, ~0ul, 0, 0, ~0ul, ~0ul, ~0ul, ~0ul, 0x3ffffffful, ~0ul, 0x7fful, 0
    };
    for (unsigned int i = 0; i < 12; ++i) {
        const unsigned long expected = (source[i] & masks[i]) | (before[i] & ~masks[i]);
        check(word(&result, 0x20 + 4*i) == expected);
    }
}
static void runCase(int points, bool reserve, int oldState, bool selfCopy, unsigned int seed) {
    using namespace NParticleEngine;
    CCountedPointer<CCardinalSpline> sourceHandle, oldHandle;
    constructorSeed = seed;
    sharedInitial = 0; retainInitial = &sourceHandle;
    initialPoints = points; reservePoints = reserve;
    CPSCEmitterGeneric source;
    constructorSeed = ~seed;
    initialPoints = oldState == 0 ? -1 : 1;
    sharedInitial = oldState == 3 ? &sourceHandle : 0;
    retainInitial = oldState == 2 ? &oldHandle : 0;
    CPSCEmitterGeneric destination;
    sharedInitial = 0; retainInitial = 0;
    CPSCEmitterGeneric& receiver = selfCopy ? source : destination;
    unsigned long fromWords[12], beforeWords[12];
    capture(fromWords, &source); capture(beforeWords, &receiver);
    const unsigned long sourceCount = info(&source) ? info(&source)->RefCount : 0;
    const bool same = info(&receiver) == info(&source);
    const bool deleteOld = !same && info(&receiver) && info(&receiver)->RefCount == 1;
    const int earlierDeletions = destroyed;
    receiver.DatabaseCopy(source);
    verifyFields(beforeWords, fromWords, receiver);
    verifyFields(fromWords, fromWords, source);
    check(info(&receiver) == info(&source) && spline(&receiver) == spline(&source));
    check(destroyed == earlierDeletions + deleteOld);
    if (!selfCopy && oldState == 2) check(oldHandle.GetReferenceCount() == 1);
    if (points < 0) {
        check(spline(&receiver) == 0);
    } else {
        check(info(&source)->RefCount == sourceCount + (same ? 0 : 1));
        CCardinalSpline* result = spline(&receiver);
        check(result->GetControlPointCount() == (points < 2 ? 2 : points));
        for (long i = 0; i < result->GetControlPointCount(); ++i) {
            const C3DVector& point = result->GetControlPoint(i);
            check(point.X == (i < points ? float(i+4) : 0.0f));
            check(point.Y == (i < points ? float(i+5) : 0.0f));
            check(point.Z == (i < points ? float(i+6) : 0.0f));
        }
    }
}
int main() {
    for (unsigned int seed = 0; seed < 10; ++seed)
        for (int points = -1; points <= 3; ++points)
            for (int reserve = 0; reserve < 2; ++reserve)
                for (int oldState = 0; oldState < 4; ++oldState)
                    for (int self = 0; self < 2; ++self)
                        runCase(points, reserve != 0, oldState, self != 0, seed);
    std::puts("00b131e0_TEST PASS");
    return 0;
}