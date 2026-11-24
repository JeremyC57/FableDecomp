// 00b16aa0 NParticleEngine::CPSCOrbit::GetUpdateParam
// Leaf: ?GetUpdateParam@CPSCOrbit@NParticleEngine@@UAE_NAAVC3DVector@@KM_NABV?$CParticleUpdateParameters@KK@2@K@Z
// Flags: /d2inlT1000
// Inline-budget: both interpolation helpers expand at the two original call sites in retail. The default retains calls to both.
#define FABLE_RETAIL_CPSCBASE_GETUPDATESTARTED_PASS27
#define FABLE_RETAIL_C3DVECTOR

#define FABLE_RETAIL_C3DVECTOR_INTERPOLATE
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ

#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR
#include "fable_string.h"
#define private public
#include "engine/NParticleEngine_CPSCOrbit.h"
#undef private
#include "engine/CParticleUpdateParameters.h"
#include <stdio.h>

namespace NParticleEngine {
inline C3DVector CPSCOrbit::COrbitState::GetSplinePosition(float t) const
{
    float t2 = t*t;
    float t3 = t2*t;
    float wpos_from = 2.0f*t3 - 3.0f*t2 + 1.0f;
    float wpos_to = -2.0f*t3 + 3.0f*t2;
    float wtan_from = t3 - 2.0f*t2 + t;
    float wtan_to = t3 - t2;
    C3DVector result = FromPosition*wpos_from + ToPosition*wpos_to + FromTangent*wtan_from + ToTangent*wtan_to;
    return result;
}

bool CPSCOrbit::GetUpdateParam(C3DVector& dest, unsigned long index, float interp,
                               bool tangent, const CParticleUpdateParameters<unsigned long,unsigned long>& params,
                               unsigned long nested)
{
    if (!GetUpdateStarted())
        StartUpdate(params);
    COrbitState* orbit_state;
    switch (index) {
    case 0: orbit_state = &OrbitStates[0]; break;
    case 1: orbit_state = &OrbitStates[1]; break;
    case 2: orbit_state = &OrbitStates[2]; break;
    default: return false;
    }
    if (tangent) {
        dest = orbit_state->FromTangent.Interpolate(orbit_state->ToTangent,interp);
        if (dest.IsNonZero())
            dest.Normalise();
    } else {
        dest = orbit_state->GetSplinePosition(interp);
    }
    return true;
}
}

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CCharString::CCharString() : PStringData(0) {}
CCharString::~CCharString() {}


const float DELTA_NOTIONAL_ZERO=0.0001f;
unsigned long GFastOneOverSqrtTable[128];
namespace NParticleEngine {
typedef CParticleUpdateParameters<unsigned long,unsigned long> Params;
typedef CParticleUpdateParamCallback<unsigned long,unsigned long> PositionCallback;
typedef CParticleUpdateParamDesc<unsigned long,unsigned long> PositionDescriptor;
template<> PositionCallback::~CParticleUpdateParamCallback() {}
template<> bool PositionCallback::GetUpdateParam(C3DVector&, unsigned long, float, bool, const Params&, unsigned long)
{
    return false;
}
template<> PositionDescriptor::~CParticleUpdateParamDesc() {}
CPSCBase::~CPSCBase() {}
void CPSCBase::OnCreate() {  }
void CPSCBase::StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>&) {  }
void CPSCBase::Update(CParticleSystem&, const Params&, bool&) {}
void CPSCBase::CollectUpdateParameters(CParticleUpdateParameters<unsigned long, unsigned long>&,const unsigned long&) {  }
void CPSCBase::TransferUpdateParametersToDialog(const CSymbolMap&) {  }
void CPSCBase::UpdateAddParticle(CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,C3DVector,C3DVector,float) {  }
void CPSCBase::PreparePrimitives(const float,const CEnginePrimitivePrepareInfo&,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,CEnginePrimitiveHandle&) {  }
void CPSCBase::DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&) {  }
bool CPSCBase::AddToBoundingSphere(C3DBoundingSphere&,const float,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&) { return false; }
bool CPSCBase::GetParticleLifeSecs(float&) { return false; }
bool CPSCBase::GetNumberOfParticles(float&,const float) { return false; }
bool CPSCBase::GetDecalLifeSecs(float&) { return false; }
bool CPSCBase::GetNumberOfDecals(float&,const float) { return false; }
void CPSCBase::EnterTimelineEnd() {  }
long CPSCBase::GetParticleCount() { return (long)0; }
long CPSCBase::GetDecalCount() { return (long)0; }
bool CPSCBase::Initialise(CIEngine*,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,CEnginePrimitiveHandle&) { return false; }
void CPSCBase::ClearAllPrimitives(CIEngine*,CEnginePrimitiveHandle&) {  }
bool CPSCBase::GetUserPoint(C3DVector&) const { return false; }
float CPSCBase::GetApproximateSplineLength() { return 0.0f; }
CPSCBase* CPSCOrbit::DatabaseClone() const { return (CPSCBase*)0; }
CCharString CPSCOrbit::GetClassTypeName() const { return CCharString(); }
EPSCType CPSCOrbit::GetComponentType() const { return (EPSCType)0; }
EPSCSubType CPSCOrbit::GetComponentSubType() const { return (EPSCSubType)0; }
CCharString CPSCOrbit::GetGUIName() const { return CCharString(); }
void CPSCOrbit::InterpolateTo(const CPSCBase&, const float) {  }
void CPSCOrbit::ReadBinary(CDataInputStream&) {  }
void CPSCOrbit::WriteBinary(CDataOutputStream&) const {  }
void CPSCOrbit::DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&) {  }
void CPSCOrbit::DatabaseCopy(const CPSCBase&) {  }
void CPSCOrbit::GetComponentCost(CParticleSystemCost&, const CParticleSystem&, const float, const float) {  }

CPSCOrbit::CPSCOrbit() {}
CPSCOrbit::~CPSCOrbit() {}
CPSCOrbit::COrbitState::COrbitState() {}
static unsigned startCount;
static const Params* lastParams;
void CPSCOrbit::StartUpdate(const Params& p) { ++startCount; lastParams=&p; UpdateStarted=true; }
void CPSCOrbit::CollectUpdateParameters(Params&,const unsigned long&) {}
}
using namespace NParticleEngine;
int main() {
    Params params;
    CPSCOrbit orbit;
    orbit.UpdateStarted=false;
    for (unsigned i=0;i<3;++i) {
        CPSCOrbit::COrbitState& state=orbit.OrbitStates[i];
        state.FromPosition=C3DVector(float(8*i),4,0);
        state.ToPosition=C3DVector(float(8*i+8),12,0);
        state.FromTangent=C3DVector(0,2,0);
        state.ToTangent=C3DVector(0,-2,0);
    }
    GFastOneOverSqrtTable[64]=255;
    C3DVector dest(1,2,3);
    if(orbit.GetUpdateParam(dest,3,0,false,params,0)||startCount!=1||lastParams!=&params||
       dest.X!=1||dest.Y!=2||dest.Z!=3)return 1;
    for (unsigned i=0;i<3;++i) {
        if(!orbit.GetUpdateParam(dest,i,0.5f,false,params,7)||
            dest.X!=8*i+4||dest.Y!=8.5f||dest.Z!=0)return 2;
        if(!orbit.GetUpdateParam(dest,i,0.5f,true,params,0)||dest.X||dest.Y||dest.Z)return 3;
        if(!orbit.GetUpdateParam(dest,i,0.0f,true,params,0)||dest.X||fabs(dest.Y-1.0f)>0.0001f||dest.Z)return 4;
        if(!orbit.GetUpdateParam(dest,i,1.0f,true,params,0)||dest.X||fabs(dest.Y+1.0f)>0.0001f||dest.Z)return 5;
    }
    if(startCount!=1)return 6;
    puts("00b16aa0_TEST PASS");return 0;
}