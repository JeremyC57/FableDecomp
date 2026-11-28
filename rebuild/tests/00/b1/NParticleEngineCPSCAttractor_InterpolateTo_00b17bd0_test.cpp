// 00b17bd0 NParticleEngine::CPSCAttractor::InterpolateTo
// Leaf: ?InterpolateTo@CPSCAttractor@NParticleEngine@@UAEXABVCPSCBase@2@M@Z
// Flags: /Gr
// Retail-asm-helper: GFFloatToLongNear
// Retail-asm: the integer interpolation at 00B17CB2 uses the shared rounding helper. The editor's long overload calls GFFloatToLongNear.
#define FABLE_RETAIL_CPSCATTRACTOR_INTERPOLATE_PASS30
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#include "rebuild_abi.h"
#include "engine/CPSCAttractor.h"
#include "engine/NParticleEngine_ParticleInterpolation.h"
#include "engine/GFDebugPDynamicCast.h"

namespace NParticleEngine {
void CPSCAttractor::InterpolateTo(const CPSCBase& to, const float interpolate)
{
    const CPSCAttractor* psc = GFDebugPDynamicCast(&to, static_cast<const CPSCAttractor*>(0));
    AttractorEnabled = GFInterpolate(AttractorEnabled, psc->AttractorEnabled, interpolate);
    AttractorPositionParam = GFInterpolateUpdateParams(AttractorPositionParam, psc->AttractorPositionParam, interpolate);
    AttractorUseParamPosition = GFInterpolate(AttractorUseParamPosition, psc->AttractorUseParamPosition, interpolate);
    AttractorPositionParamName = GFInterpolateVectorParams(AttractorPositionParamName, psc->AttractorPositionParamName, interpolate);
    AttractorInfluenceRadius = GFInterpolate(AttractorInfluenceRadius, psc->AttractorInfluenceRadius, interpolate);
    AttractorInfluenceForce = GFInterpolate(AttractorInfluenceForce, psc->AttractorInfluenceForce, interpolate);
    AttractorInfluenceFallOff = GFInterpolate(AttractorInfluenceFallOff, psc->AttractorInfluenceFallOff, interpolate);
    unsigned long size = AttractorUserPointsArray.size();
    for (long ci = 0; ci < size; ++ci) {
        AttractorUserPointsArray[ci] = GFInterpolate(AttractorUserPointsArray[ci], psc->AttractorUserPointsArray[ci], interpolate);
    }
}
}

#include <cstdio>
#include <cstdlib>
#include "fable_string.h"
static unsigned int pointCount;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
namespace NParticleEngine {
CPSCAttractor::CPSCAttractor() : AttractorUserPointsArray(pointCount, C3DVector(0,0,0)) {}
CPSCAttractor::~CPSCAttractor() {}
void CPSCAttractor::DatabaseCopy(const CPSCBase&) { std::abort(); }
void CPSCAttractor::Update(CParticleSystem&, const CParticleUpdateParameters<unsigned long,unsigned long>&, bool&) { std::abort(); }
template<class K, class V> bool CParticleUpdateParamCallback<K,V>::GetUpdateParam(C3DVector&, V, float, bool, const CParticleUpdateParameters<K,V>&, unsigned long) { std::abort(); }
template<class K, class V> CParticleUpdateParamCallback<K,V>::~CParticleUpdateParamCallback() {}
CPSCBase::~CPSCBase() {}
void CPSCAttractor::DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&) { std::abort(); }
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
CPSCBase* CPSCAttractor::DatabaseClone() const { std::abort(); }
CCharString CPSCAttractor::GetClassTypeName() const { std::abort(); }
EPSCType CPSCAttractor::GetComponentType() const { std::abort(); }
EPSCSubType CPSCAttractor::GetComponentSubType() const { std::abort(); }
CCharString CPSCAttractor::GetGUIName() const { std::abort(); }
void CPSCAttractor::ReadBinary(CDataInputStream&) { std::abort(); }
void CPSCAttractor::WriteBinary(CDataOutputStream&) const { std::abort(); }
void CPSCAttractor::GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float) { std::abort(); }
}
static void check(bool value)
{
    if (!value) std::abort();
}
static long nearestEven(float x)
{
    const long n = static_cast<long>(x);
    const float fraction = x - n;
    if (fraction > 0.5f || (fraction == 0.5f && n % 2 != 0)) return n + 1;
    if (fraction < -0.5f || (fraction == -0.5f && n % 2 != 0)) return n - 1;
    return n;
}
static void run(unsigned int count, unsigned int bits, float t, bool self)
{
    using namespace NParticleEngine;
    pointCount = count;
    CPSCAttractor from;
    pointCount = count + 3;
    CPSCAttractor other;
    from.AttractorEnabled = (bits & 1) != 0;
    from.AttractorUseParamPosition = (bits & 2) != 0;
    other.AttractorEnabled = (bits & 4) != 0;
    other.AttractorUseParamPosition = (bits & 8) != 0;
    from.AttractorPositionParam = 0xf2345678ul;
    other.AttractorPositionParam = 0x1234567ful;
    from.AttractorPositionParamName = 17;
    other.AttractorPositionParamName = 31;
    from.AttractorInfluenceRadius = -1.75f;
    from.AttractorInfluenceForce = 4.5f;
    other.AttractorInfluenceRadius = 9.25f;
    other.AttractorInfluenceForce = 1.5f;
    from.AttractorInfluenceFallOff = static_cast<long>(bits % 7) - 3;
    other.AttractorInfluenceFallOff = from.AttractorInfluenceFallOff + 1;
    for (unsigned int i=0; i<count+3; ++i) {
        float x = static_cast<float>(i);
        other.AttractorUserPointsArray[i] = C3DVector(x+7, 2*x-3, -x);
        if (i<count) from.AttractorUserPointsArray[i] = C3DVector(x-5, 3-x, x+4);
    }
    const bool takeOther = !self && t > 0.5f;
    from.InterpolateTo(self ? from : other,t);
    check(from.AttractorEnabled == ((bits & (takeOther ? 4 : 1)) != 0));
    check(from.AttractorUseParamPosition == ((bits & (takeOther ? 8 : 2)) != 0));
    check(from.AttractorPositionParam == (takeOther ? 0x1234567ful : 0xf2345678ul));
    check(from.AttractorPositionParamName == (takeOther ? 31ul : 17ul));
    const float k=self ? 0.0f : t;
    check(from.AttractorInfluenceRadius == -1.75f + 11.0f*k);
    check(from.AttractorInfluenceForce == 4.5f - 3*k);
    check(from.AttractorInfluenceFallOff == nearestEven(static_cast<float>(static_cast<long>(bits%7)-3) + k));
    check(from.AttractorUserPointsArray.size()==count && other.AttractorUserPointsArray.size()==count+3);
    for (unsigned int i=0; i<count+3; ++i) {
        float x = static_cast<float>(i);
        const C3DVector& v = other.AttractorUserPointsArray[i];
        check(v.X==x+7 && v.Y==2*x-3 && v.Z==-x);
        if (i<count) {
            const C3DVector& a = from.AttractorUserPointsArray[i];
            check(a.X==x-5+12*k && a.Y==3-x+(3*x-6)*k && a.Z==x+4+(-2*x-4)*k);
        }
    }
}
int main()
{
    const float weights[] = {-1.0f, 0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.5f};
    for (unsigned int count=0; count<=11; ++count)
        for (unsigned int bits=0; bits<16; ++bits)
            for (unsigned int i=0; i<sizeof(weights)/sizeof(weights[0]); ++i) {
                run(count,bits,weights[i],false);
                run(count,bits,weights[i],true);
            }
    std::puts("00b17bd0_TEST PASS");
    return 0;
}