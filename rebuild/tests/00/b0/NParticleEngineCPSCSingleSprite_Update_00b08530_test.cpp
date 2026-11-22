// NParticleEngine::CPSCSingleSprite::Update(CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, bool&) -- advances sprite trails and keeps active timelines alive.
// Leaf: ?Update@CPSCSingleSprite@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AA_N@Z
// Flags: default
// Inline-budget: The two retail world-space adjustments expand at the caller. The end wrapper stays out of line without its scoped opt-in.
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TOC3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALE
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALEPARTICLEDIMENSIONS_C2DVECTOR_INLINE
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#define FABLE_RETAIL_CPARTICLESYSTEM_BOUNDING_TRANSFORM_PASS24
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_PARTICLE_POINT
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_ADDTOBOUNDS
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM_NESTED
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_GETUPDATEPARAM
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSINGLESPRITE_ADDTOBOUNDINGSPHERE
#define FABLE_RETAIL_CPSCSINGLESPRITE_MEMBERS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CPARTICLESYSTEM_TRANSFORMPOSIFWORLDEND
#define FABLE_RETAIL_CPARTICLESYSTEM_TRANSFORMPOSIFWORLDEND_INLINE
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define private public
#define protected public
#include "engine/CParticleSystem.h"
#include "engine/CParticleUpdateParameters.h"
#include "engine/CPSCSingleSprite.h"

#undef private
#undef protected
#include "engine/CEnginePrimitiveEnums.h"
#include <stdio.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CCharString::CCharString() : PStringData(0) {}
CCharString::~CCharString() {}

CEnginePrimitiveSpriteTrail::CEnginePrimitiveSpriteTrail()
    : CEnginePrimitive(ENGINE_PRIMITIVE_SPRITE_TRAIL, (EEngineInterpolationSet)0) {}

namespace NParticleEngine {
typedef CParticleUpdateParameters<unsigned long, unsigned long> Params;
typedef CParticleUpdateParamCallback<unsigned long, unsigned long> PositionCallback;
typedef CParticleUpdateParamDesc<unsigned long, unsigned long> PositionDescriptor;

static float CurveLength;
static float TrailPositions[8];
static float TrailWidthUsed;
static unsigned int PositionCalls;
static unsigned int SplineCalls;
static unsigned int StartTransformCalls;
static unsigned int EndTransformCalls;
static unsigned int CallbackCalls;
static bool StartPosAvailable;
static bool EndPosAvailable;
static bool CallbackArgsCorrect;

CParticleSystem::CParticleSystem() {}
CParticleSystem::~CParticleSystem() {}
void CParticleSystem::GetSystemTransform(CMatrix3x4& matrix) const
{
    ++StartTransformCalls;
    matrix.SetIdentity();
    matrix.E41 = 10.0f;
}
void CParticleSystem::GetSystemEndTransform(CMatrix3x4& matrix) const
{
    ++EndTransformCalls;
    matrix.SetIdentity();
    matrix.E41 = 20.0f;
}
template<> PositionCallback::~CParticleUpdateParamCallback() {}
template<> bool PositionCallback::GetUpdateParam(C3DVector&, unsigned long, float, bool, const Params&, unsigned long)
{
    return false;
}
template<> PositionDescriptor::~CParticleUpdateParamDesc() {}
CPSCBase::~CPSCBase() {}
CPSCSplineBase::CSplinePosParamData::CSplinePosParamData(const CCardinalSpline& spline) : Spline(spline) {}

CPSCSingleSprite::CPSCSingleSprite() {}
CPSCSingleSprite::~CPSCSingleSprite() {}
void CPSCSingleSprite::AddTrailPosition(const C3DVector& pos, float width)
{
    TrailPositions[PositionCalls++] = pos.X;
    TrailWidthUsed = width;
    TrailUpdatePrimitive.Trail.push_back(CEnginePrimitiveSpriteTrail::CSpriteTrailEntry(pos, width));
}
void CPSCSingleSprite::SetTrailPositionsFromSpline(CParticleSystem&, const Params&, float width, float time)
{
    SplineCalls += (width == TrailWidth && time == 0.0f) ? 1 : 100;
}
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
float CPSCBase::GetApproximateSplineLength() { return CurveLength; }
bool CPSCSingleSprite::AddToBoundingSphere(C3DBoundingSphere& bsphere, const float interpolate, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params) { return false; }
CPSCBase* CPSCSingleSprite::DatabaseClone() const { return (CPSCBase*)0; }
CCharString CPSCSingleSprite::GetClassTypeName() const { return CCharString(); }
EPSCType CPSCSingleSprite::GetComponentType() const { return (EPSCType)0; }
EPSCSubType CPSCSingleSprite::GetComponentSubType() const { return (EPSCSubType)0; }
CCharString CPSCSingleSprite::GetGUIName() const { return CCharString(); }
void CPSCSingleSprite::PreparePrimitives(const float, const CEnginePrimitivePrepareInfo&, CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&) {  }
void CPSCSingleSprite::InterpolateTo(const CPSCBase&, const float) {  }
void CPSCSingleSprite::ReadBinary(CDataInputStream&) {  }
void CPSCSingleSprite::WriteBinary(CDataOutputStream&) const {  }
void CPSCSingleSprite::DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&) {  }
void CPSCSingleSprite::DatabaseCopy(const CPSCBase&) {  }
void CPSCSingleSprite::GetComponentCost(CParticleSystemCost&, const CParticleSystem&, const float, const float) {  }
void CPSCSingleSprite::EnterTimelineEnd() {  }
bool CPSCSingleSprite::Initialise(CIEngine*, CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&) { return false; }
void CPSCSingleSprite::ClearAllPrimitives(CIEngine*, CEnginePrimitiveHandle&) {  }


class TestPositionCallback : public PositionCallback {
public:
    virtual bool GetUpdateParam(C3DVector& pos, unsigned long index, float time, bool tangent,
                                const Params&, unsigned long depth)
    {
        ++CallbackCalls;
        if (index != 29 || tangent || depth != 1)
            CallbackArgsCorrect = false;
        pos = C3DVector(1.0f + time, 2.0f, 3.0f);
        return time == 0.0f ? StartPosAvailable : EndPosAvailable;
    }
};
}
namespace NParticleEngine {
void CPSCSingleSprite::Update(CParticleSystem& system,
    const CParticleUpdateParameters<unsigned long, unsigned long>& params, bool& keep_alive)
{
    if (!EnteringTimelineEnd || GetApproximateSplineLength() > 0.0f)
        keep_alive = true;

    C3DVector pos;
    if (Lifetimer == 0 && UsePosition) {
        TrailUpdatePrimitive.Trail.clear();
        if (params.GetUpdateParam(pos, PositionParam, 0, false, 0)) {
            system.TransformPosIfWorld(pos);
            AddTrailPosition(pos, TrailWidth);
        }
    }
    if (UsePosition) {
        if (params.GetUpdateParam(pos, PositionParam, 1, false, 0)) {
            system.TransformPosIfWorldEnd(pos);
            AddTrailPosition(pos, TrailWidth);
        }
    } else if ((SplineRandomOffsetData.IsNull() && !SplinePosParamData.IsNull())
               || TrailUpdatePrimitive.Trail.size() == 0) {
        SetTrailPositionsFromSpline(system, params, TrailWidth, 0);
    }
    ++Lifetimer;
}
}

using namespace NParticleEngine;

int main()
{
    CPSCSingleSprite sprite;
    CParticleSystem system;
    Params params;
    TestPositionCallback callback;
    std::pair<unsigned long, PositionDescriptor> entry;
    entry.first = 5;
    entry.second.Callback = &callback;
    entry.second.Index = 29;
    params.UpdateParams.push_back(entry);
    sprite.PositionParam = 5;
    sprite.TrailWidth = 0.5f;
    sprite.Lifetimer = 0;
    sprite.EnteringTimelineEnd = false;
    sprite.UsePosition = true;
    system.StayWithEmitter = false;
    StartPosAvailable = EndPosAvailable = CallbackArgsCorrect = true;
    bool keep_alive = false;
    sprite.Update(system, params, keep_alive);
    if (!keep_alive || sprite.Lifetimer != 1 || !CallbackArgsCorrect || CallbackCalls != 2
        || PositionCalls != 2 || TrailPositions[0] != 11.0f || TrailPositions[1] != 22.0f
        || TrailWidthUsed != 0.5f || StartTransformCalls != 1 || EndTransformCalls != 1
        || sprite.TrailUpdatePrimitive.Trail.size() != 2)
        return 1;

    sprite.EnteringTimelineEnd = true;
    CurveLength = 0.0f;
    keep_alive = false;
    system.StayWithEmitter = true;
    sprite.Update(system, params, keep_alive);
    if (keep_alive || sprite.Lifetimer != 2 || CallbackCalls != 3 || PositionCalls != 3
        || TrailPositions[2] != 2.0f || EndTransformCalls != 1)
        return 2;

    CurveLength = 2.0f;
    sprite.Lifetimer = 0;
    StartPosAvailable = EndPosAvailable = false;
    sprite.Update(system, params, keep_alive);
    if (!keep_alive || CallbackCalls != 5 || PositionCalls != 3 || !sprite.TrailUpdatePrimitive.Trail.empty())
        return 3;

    sprite.UsePosition = false;
    sprite.Update(system, params, keep_alive);
    if (SplineCalls != 1)
        return 4;
    sprite.AddTrailPosition(C3DVector(0.0f, 0.0f, 0.0f), 0.5f);
    sprite.Update(system, params, keep_alive);
    if (SplineCalls != 1)
        return 5;

    CCardinalSpline spline;
    CPSCSplineBase::CSplinePosParamData spline_params(spline);
    sprite.SplinePosParamData.Data = &spline_params;
    sprite.Update(system, params, keep_alive);
    if (SplineCalls != 2)
        return 6;

    // Its counted pointer only tests nullness in Update. Nothing accesses the pointee.
    sprite.SplineRandomOffsetData.Data =
        reinterpret_cast<CPSCSplineBase::CSplineRandomOffsetData*>(&spline_params);
    sprite.Update(system, params, keep_alive);
    if (SplineCalls != 2)
        return 7;

    sprite.UsePosition = true;
    sprite.PositionParam = 9;
    sprite.Update(system, params, keep_alive);
    if (CallbackCalls != 5 || PositionCalls != 4 || SplineCalls != 2)
        return 7;

    sprite.PositionParam = 3;
    sprite.Update(system, params, keep_alive);
    sprite.PositionParam = 0;
    sprite.Update(system, params, keep_alive);
    if (CallbackCalls != 5 || PositionCalls != 4 || SplineCalls != 2)
        return 8;

    puts("00b08530_TEST PASS");
    return 0;
}