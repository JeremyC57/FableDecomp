// Fixture for the reconstructed camera allocation body.
// Leaf: ?Alloc@CLookAtFrontInit@CLookAtFront@NCameraModes@@UBEPAVCBase@3@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED

#define FABLE_RETAIL_NCAMERAMODES_LOOKATFRONT_PASS27
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GETOBSERVEDTHING
#define FABLE_RETAIL_CCAMERAMODEDEF_CYLINDER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_PASS22_STATICFOCAL
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CDEFPOINTER_ORIGINAL_DIRECT_DEREF_PASS26
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_ORIGINAL_CTOR_PASS23
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_BASE_OBJECT
#define protected public
#include "rebuild_abi.h"
#include "fable_assert.h"
#include "engine/ThingAlive.h"
#define private public
#include "engine/NCameraModes_CLookAtFront.h"
#undef private
#undef protected
#include "engine/CCameraModeDef.h"
#include "engine/CGameCameraManager.h"
#define protected public
#include "engine/CThing.h"
#undef protected
#include "engine/CThingGameObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

NCameraModes::CBase* NCameraModes::CLookAtFront::CLookAtFrontInit::Alloc(
        CGameCameraManager& cam_man, const CDefPointer<const CCameraModeDef>& cam_mode_def) const
{
    const CThingGameObject* pobs = cam_man.GetObservedThing();
    FABLE_RETAIL_ASSERT(IS_THING_ALIVE(pobs));

    C3DVector facing = pobs->GetPhysicsTC()->GetFacingVector();
    C3DVector start_pos = pobs->GetPos() + facing * cam_mode_def->StringLengthXY +
                         C3DVector(0.0f, 0.0f, cam_mode_def->HeightOffsetZ);
    C3DVector look_at = pobs->GetTCGraphicAppearance()->GetStaticFocalPos();

    return new CLookAtFront(cam_man, cam_mode_def, start_pos, look_at, true);
}

inline NCameraModes::CLookAtFront::CLookAtFront(CGameCameraManager& cam_man,
        const CDefPointer<const CCameraModeDef>& cam_mode_def,
        const C3DVector& start_pos, const C3DVector& look_at, bool allow_look_around)
    : CSimplePointBase(cam_man, cam_mode_def, start_pos, look_at, allow_look_around)
{}

// Same-TU constructor at 00873A10. Its visible body keeps the temporary vector slots reusable.
NCameraModes::CSimplePointBase::CSimplePointBase(CGameCameraManager& cam_man,
        const CDefPointer<const CCameraModeDef>& cam_mode_def,
        const C3DVector& start_pos, const C3DVector& look_at, bool allow_look_around)
    : CBase(cam_man, cam_mode_def), Position(start_pos), LookAtPos(look_at),
      AllowLookAround(allow_look_around)
{}

// Fixture boundaries and unused vtable entries are defined here.
#include "engine/CRightHandedSet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

template <class T>
union Storage {
    double alignment;
    unsigned char bytes[sizeof(T)];
    T* Get() { return reinterpret_cast<T*>(bytes); }
};

static const CThingGameObject* observed;
static const NGraphicAppearance::CTCGraphicAppearance* appearance;
static const CTCPhysicsBase* physical;
static C3DVector position, facing, focal_point;
static const CGameCameraManager* received_manager;
static const CDefPointer<const CCameraModeDef>* received_def;
static bool allow_allocation, trace_enabled;
static unsigned constructions;
static unsigned trace_length;
static char trace[16];

static void Require(bool condition)
{
    if (!condition)
        abort();
}

static void Trace(char code)
{
    Require(trace_length < sizeof(trace)-1);
    trace[trace_length++] = code;
}

void* __cdecl operator new(size_t size)
{
    if (trace_enabled && size == sizeof(NCameraModes::CLookAtFront)) {
        Trace('A');
        if (!allow_allocation)
            return 0;
    }
    return malloc(size);
}

void __cdecl operator delete(void* ptr)
{
    free(ptr);
}

CBaseObject* CBaseIntelligentPointer::GetPItem() const
{
    Trace('O');
    return const_cast<CThingGameObject*>(observed);
}

const C3DVector& CThing::GetPos() const
{
    Require(this == observed);
    Trace('P');
    return position;
}

C3DVector NGraphicAppearance::CTCGraphicAppearance::GetStaticFocalPos() const
{
    Require(this == appearance);
    Trace('L');
    return focal_point;
}

NCameraModes::CBase::CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def)
    : CameraManager(manager), PCameraModeDef(def)
{
    ++constructions;
    Trace('C');
    received_manager = &manager;
    received_def = &def;
}

NCameraModes::CBase::~CBase() { }

void NCameraModes::CSimplePointBase::OnInit() { abort(); }
void NCameraModes::CSimplePointBase::Update() { abort(); }
void NCameraModes::CSimplePointBase::OnDie() { abort(); }
bool NCameraModes::CSimplePointBase::IsUseRightStickLookAround() const { return AllowLookAround; }
NCameraModes::EMode NCameraModes::CLookAtFront::GetModeType() const { abort(); }

class FacingPhysics : public CTCPhysicsBase {
public:
    explicit FacingPhysics(CThing& owner) : CTCPhysicsBase(owner) { }
    CCharString GetClassTypeName() const { abort(); }
    virtual C3DVector GetFacingVector() const
    {
        Require(this == physical);
        Trace('F');
        return facing;
    }
};
void NCameraModes::CBase::OnCameraInsideObservedThing() { abort(); }
bool NCameraModes::CBase::IsUseRightStickLookAround() const { abort(); }
void NCameraModes::CBase::PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0) const { abort(); }
bool NCameraModes::CBase::IsShortenOnOcclusion() const { abort(); }
bool NCameraModes::CBase::IsRaiseUpOnHeroNearby() const { abort(); }
bool NCameraModes::CBase::IsStretchAroundHero() const { abort(); }
bool NCameraModes::CBase::IsAvoidLandscape() const { abort(); }
bool NCameraModes::CBase::IsFOVChange() const { abort(); }
bool NCameraModes::CBase::IsTrackMeleeOpponents() const { abort(); }
bool NCameraModes::CBase::IsTrackMeleeOpponentsAutoFOVChange() const { abort(); }
bool NCameraModes::CBase::IsFinished() const { abort(); }
bool NCameraModes::CBase::IsStringAndCage() const { abort(); }
float NCameraModes::CBase::GetObservedThingCurrentMeshHeight() const { abort(); }
void NCameraModes::CBase::Recentre() { abort(); }
void NCameraModes::CBase::UpdateSubComponents() { abort(); }
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseObject::CBaseObject() : IntelligentPointer(0) {}
CBaseObject::~CBaseObject() {}
void CBaseObject::Validate() const {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CTCBase::CTCBase(CThing& thing) : ParentThing(&thing) {}
CTCBase::~CTCBase() {}
void CTCBase::OnSerialise(CPersistContext&, bool) {}
void CTCBase::OnRemove() {}
void CTCBase::ConstructFromParams(const CTCParamsBase&) {}
void CTCBase::OnCreate() {}
void CTCBase::InitialActivate() {}
void CTCBase::OnInitialActivate() {}
void CTCBase::RenderUpdate(float) {}
bool CTCBase::HasFrameUpdate() const { return false; }
void CTCBase::FrameUpdate() {}
void CTCBase::BulletTimeFrameUpdate() {}
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) {}
void CTCBase::Copy(const CTCBase&) {}
void CTCBase::OnDie() {}
void CTCBase::OnKill() {}
void CTCBase::OnLevelUnload() {}
void CTCBase::OnSetInLimbo(bool) {}
bool CTCBase::Validate(CCharString&) const { return true; }
bool CTCBase::IsFreeForDeletion() const { return true; }
bool CTCBase::IsSerialiseCreatable() const { return false; }
bool CTCBase::IsSerialisableForHeroState() const { return false; }
bool CTCBase::IsComponentSerialisable() const { return false; }
bool CTCBase::IsDriver() const { return false; }
bool CTCBase::IsCopyable() const { return false; }
void CTCBase::GlobalUpdate(bool) {}
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) {}
CTCPhysicsBase::CTCPhysicsBase(CThing& thing) : CTCBase(thing), Radius(1.0f) {}
CTCPhysicsBase::~CTCPhysicsBase() {}
ETCInterfaceType CTCPhysicsBase::GetInterfaceType() const { return TCI_PHYSICS; }
C2DVector CTCPhysicsBase::GetOffsetVector() const { abort(); }
C3DVector CTCPhysicsBase::GetOffsetVector3D() const { abort(); }
void CTCPhysicsBase::SetPosition(const C3DVector &) { abort(); }
void CTCPhysicsBase::ResetPosition(const C3DVector &) { abort(); }
void CTCPhysicsBase::ResetRHSet(const CRightHandedSet &) { abort(); }
void CTCPhysicsBase::SetPositionToNavigateTo(const C3DVector &) { abort(); }
void CTCPhysicsBase::SetForcedToMovePos(const C3DVector &,bool) { abort(); }
void CTCPhysicsBase::ResetMovementState() { abort(); }
void CTCPhysicsBase::EnablePhysics(bool) { abort(); }
bool CTCPhysicsBase::IsPhysicsEnabled() const { abort(); }
void CTCPhysicsBase::EnableGravity(bool) { abort(); }
bool CTCPhysicsBase::IsGravityEnabled() const { abort(); }
void CTCPhysicsBase::EnableNavigator(bool) { abort(); }
bool CTCPhysicsBase::IsNavigationEnabled() const { abort(); }
bool CTCPhysicsBase::HasNavigationLayers() const { abort(); }
void CTCPhysicsBase::SetInteractionFlags(long) { abort(); }
void CTCPhysicsBase::ReturnToDefaultInteractionFlags() { abort(); }
void CTCPhysicsBase::OnDroppedFromUpdateList() { abort(); }
void CTCPhysicsBase::OnAddedToUpdateList() { abort(); }
bool CTCPhysicsBase::IsAlwaysFacingMovementDirection() const { abort(); }
bool CTCPhysicsBase::IsAffectedByGravity() const { abort(); }
bool CTCPhysicsBase::IsAffectedByAir() const { abort(); }
bool CTCPhysicsBase::IsAffectedByWind() const { abort(); }
bool CTCPhysicsBase::IsAngledWithGround() const { abort(); }
bool CTCPhysicsBase::IsImmoveable() const { abort(); }
bool CTCPhysicsBase::IsOnAnySurface() const { abort(); }
bool CTCPhysicsBase::IsOnAnyStationarySurface() const { abort(); }
bool CTCPhysicsBase::IsOnMeshSurface() const { abort(); }
bool CTCPhysicsBase::IsOnGroundSurface() const { abort(); }
bool CTCPhysicsBase::CanCollideWithThings() const { abort(); }
bool CTCPhysicsBase::CanCollideWithThingsOneWay() const { abort(); }
bool CTCPhysicsBase::CanCollideWithLandscape() const { abort(); }
bool CTCPhysicsBase::CanInteractWithThings() const { abort(); }
bool CTCPhysicsBase::IsExcludingCreatures() const { abort(); }
bool CTCPhysicsBase::IsPhysicsMeshImpassable() const { abort(); }
float CTCPhysicsBase::GetFacingAngleXY() const { abort(); }
float CTCPhysicsBase::GetFacingAngleYZ() const { abort(); }
float CTCPhysicsBase::GetFacingAngleXZ() const { abort(); }
void CTCPhysicsBase::SetFacingAngleXY(float) { abort(); }
void CTCPhysicsBase::SetFacingAngleYZ(float) { abort(); }
void CTCPhysicsBase::SetFacingAngleXZ(float) { abort(); }
C3DVector CTCPhysicsBase::GetFacingVector() const { abort(); }
C3DVector CTCPhysicsBase::GetFacingVector(float) const { abort(); }
const CRightHandedSet & CTCPhysicsBase::GetRHSet() const { abort(); }
CRightHandedSet CTCPhysicsBase::GetRHSet(float) const { abort(); }
const CRightHandedSet & CTCPhysicsBase::GetLastRHSet() const { abort(); }
void CTCPhysicsBase::SetFacingAnglesFromRHSet(const CRightHandedSet &) { abort(); }
void CTCPhysicsBase::SetRHSet(const CRightHandedSet &) { abort(); }
C3DVector CTCPhysicsBase::GetImpulse() const { abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnNormal(const C3DVector &) { abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnGround() { abort(); }
float CTCPhysicsBase::GetRenderZOffset(float) const { abort(); }
void CTCPhysicsBase::ResetInterpolation() { abort(); }
void CTCPhysicsBase::ApplyControlledImpulse(const C3DVector &) { abort(); }
void CTCPhysicsBase::ApplyControlledAcceleration(const C3DVector &,float) { abort(); }
bool CTCPhysicsBase::IsPushableByHero() const { abort(); }
void CTCPhysicsBase::SetAsPushableByHero(bool) { abort(); }
void CTCPhysicsBase::FrameUpdate() { abort(); }
bool CTCPhysicsBase::HasFrameUpdate() const { abort(); }
void CTCPhysicsBase::SetPhysicsMeshBankIndex(long) { abort(); }
void CTCPhysicsBase::SetScaleFromOwner() { abort(); }
void CTCPhysicsBase::ConstructFromParams(const CTCParamsBase &) { abort(); }
void CTCPhysicsBase::OnCreate() { abort(); }
void CTCPhysicsBase::InitialActivate() { abort(); }
void CTCPhysicsBase::Move() { abort(); }
bool CTCPhysicsBase::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) const { abort(); }
void CTCPhysicsBase::OnSerialise(CPersistContext &,bool) { abort(); }
bool CTCPhysicsBase::IsComponentSerialisable() const { abort(); }
bool CTCPhysicsBase::IsSerialisableForHeroState() const { abort(); }
bool CTCPhysicsBase::IsCopyable() const { abort(); }
void CTCPhysicsBase::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithThingAt(const CTCPhysicsBase &,const C3DVector &,CPhysicsCollideResponse &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithLandscapeAt(const C3DVector &,CPhysicsCollideResponse &) const { abort(); }
const CThingPhysicsMeshInfo * CTCPhysicsBase::GetPhysicsMeshInfo() const { abort(); }
void CTCPhysicsBase::UpdateInterpolation() { abort(); }
void CTCPhysicsBase::UpdateOldRHSet() { abort(); }
void CTCPhysicsBase::UpdateOldPos() { abort(); }


void NCameraModes::CBase::OnReset() { abort(); }
float NCameraModes::CBase::GetDefaultObservedPositionZOffset() const { abort(); }
float NCameraModes::CBase::GetDefaultRollAngle() const { abort(); }
float NCameraModes::CBase::GetDefaultFOV() const { abort(); }
bool CTCPhysicsBase::HasCollidedWithThingFromTo(const CTCPhysicsBase&, const C3DVector&,
    const C3DVector&, CPhysicsCollideResponse&) const { abort(); }

static bool Equal(const C3DVector& vector, float x, float y, float z)
{
    return vector.X == x && vector.Y == y && vector.Z == z;
}

static void Run(const NCameraModes::CLookAtFront::CLookAtFrontInit& init,
    CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def,
    bool allocate, const C3DVector& expected_pos)
{
    trace_length = 0;
    memset(trace, 0, sizeof(trace));
    constructions = 0;
    allow_allocation = allocate;
    trace_enabled = true;
    NCameraModes::CBase* result = init.Alloc(manager, def);
    trace_enabled = false;
    Require((result != 0) == allocate);
    Require(constructions == (allocate ? 1u : 0u));
    Require(strcmp(trace, allocate ? "OFPLAC" : "OFPLA") == 0);
    if (allocate) {
        Require(received_manager == &manager && received_def == &def);
        const NCameraModes::CSimplePointBase& mode = *static_cast<NCameraModes::CSimplePointBase*>(result);
        Require(mode.AllowLookAround && result->IsUseRightStickLookAround());
        Require(Equal(mode.Position, expected_pos.X, expected_pos.Y, expected_pos.Z));
        Require(Equal(mode.LookAtPos, focal_point.X, focal_point.Y, focal_point.Z));
        delete result;
    }
}

int main()
{
    Storage<CGameCameraManager> manager_store;
    Storage<CThingGameObject> thing_store;
    Storage<NGraphicAppearance::CTCGraphicAppearance> appearance_store;
    Storage<CCameraModeDef> def_store;
    memset(&manager_store, 0, sizeof(manager_store));
    memset(&thing_store, 0, sizeof(thing_store));
    memset(&appearance_store, 0, sizeof(appearance_store));
    memset(&def_store, 0, sizeof(def_store));

    CThingGameObject* thing = thing_store.Get();
    FacingPhysics physics(*thing);
    observed = thing;
    physical = &physics;
    appearance = appearance_store.Get();
    thing->PhysicsTC = &physics;
    thing->PTCGraphicAppearance = appearance_store.Get();

    // The parent definition's lifetime belongs to the fixture. Retain a
    // reference across the calls so the actual counted handle can release it.
    CCameraModeDef* settings = def_store.Get();
    settings->IVRefCount = 1;
    CDefPointer<const CCameraModeDef> def;
    def.Reset(settings);
    NCameraModes::CLookAtFront::CLookAtFrontInit init;
    CGameCameraManager& manager = *manager_store.Get();
    settings->StringLengthXY = 4.0f;
    settings->HeightOffsetZ = 1.5f;
    position = C3DVector(10.0f, 20.0f, 1.0f);
    facing = C3DVector(-1.0f, 2.0f, -0.5f);
    focal_point = C3DVector(1.0f, 2.0f, 3.0f);
    Run(init, manager, def, true, C3DVector(6.0f, 28.0f, 0.5f));

    settings->StringLengthXY = -2.0f;
    settings->HeightOffsetZ = -3.0f;
    facing = C3DVector(0.0f, 2.0f, 1.0f);
    focal_point = C3DVector(-1.0f, 0.0f, 10.0f);
    Run(init, manager, def, true, C3DVector(10.0f, 16.0f, -4.0f));
    Run(init, manager, def, false, C3DVector());
    Require(settings->IVRefCount == 2);
    puts("00873930_TEST PASS");
    return 0;
}