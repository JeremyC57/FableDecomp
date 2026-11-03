// CWorld::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info) -- once per world, when the environment needs weather
//   masks, send one persistent weather-mask primitive per weather-masked building (its bounding sphere, mesh box and the inverse of
//   its scaled placement matrix); then let the thing manager, the particle emitters and the current game camera prepare theirs
// Grade: DIFFER(1331v1330) -- 72 diff lines: the inlined cross product's x87 operand order (Y second / Z first products), the
//   SetInverse translation terms' order (retail X,Y,Z / Z,Y,X / X,Z,Y; VC7.1 canonicalises every spelling tried to Z,Y,X) and the
//   six-dword mesh-box copy's load/store interleave
// Leaf: ?PreparePrimitivesForEngine@CWorld@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A1CB0, 1330 bytes, thiscall, `ret 4`; frame 0x184.
// Callees: CEnvironment::IsEnvironmentRequiringWeatherMasks 0x006B2C00, GetAllLeafThings<CThingBuilding> 0x004AD700 (same TU),
//   vector<CEnginePrimitiveHandle>::resize 0x00443F90, CDefinitionManager::GetDef<CThingBuildingDef> 0x004AB840 (same TU),
//   CTCPhysicsBase virtuals GetPhysicsMeshInfo (+0x174) / GetBoundingInfo 0x006AF640 / GetRHSet (+0x120), GetPhysicsMeshObject
//   0x006B0850 + ~CPhysicsMeshObject 0x00A127D0, CEnginePrimitiveWeatherMask(...) 0x0049D000, CIEngine::SetPersistentPrimitive
//   (+0x5C), CThingManager::PreparePrimitivesForEngine 0x0051EAB0, NParticleEngine::GetParticleEmitterManager 0x00AEAAF0 +
//   PreparePrimitives 0x00AEB990, CAIGameCameraBase::PreparePrimitivesForRendering (virtual).
// FableWin.pdb inline order after the row (0x01B408C0..0x01B41390): C3DBoundingSphere(const C3DVector&, float), C3DBoundingBox copy,
//   the weather-mask ctor, GetRight, C3DVector::operator^, CMatrix3x4(EMatrix34Init), SetInverse, GetDeterminant, PreScale,
//   PostTranslate, GetDefGlobalIndex, GetBoundingBox, GetMesh, GetPosition, HasPhysicsMeshObject.
// Shape: the same-TU GetAllLeafThings / GetDef bodies keep the vector in registers and drop the CDefPointer null check; the
//   translation row is assigned first from a position reference (retail loads it onto the x87 stack before the negations).
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#include "engine/CWorld.h"
#include "engine/CThingManager.h"
#include "engine/CEnvironment.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_None.h"
#include "engine/CThingBuilding.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefPointer.h"
#include "engine/CMatrix3x4.h"
#include "engine/CRightHandedSet.h"
#include "engine/C3DBoundingBox.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CEnginePrimitiveWeatherMask.h"

// Classes without a shared header (Ego_r.pdb layouts; retail offsets as read by this row).
class CThingPhysicsMeshInfo {                                // Ego_r.pdb, 28 bytes, no STL member
public:
    unsigned short BankIndex;                                // +0x00
    float          RenderSizeX;                              // +0x04
    C3DVector      ToCentre;                                 // +0x08
    float          BoundingRadius;                           // +0x14
    float          BoundingCircleRadius;                     // +0x18
};
class CPhysicsMesh : public CIVCountedPointeeBase {          // Ego_r.pdb CPhysicsMesh; retail STLport sizes put BoundingBox at +0x84
public:
    const C3DBoundingBox& GetBoundingBox() const { return BoundingBox; }   // Ego_r.pdb ?GetBoundingBox@CPhysicsMesh@@QBEABVC3DBoundingBox@@XZ (inline here)
private:
    unsigned char     _pad_0x08[0x74 - 0x08];                // ReferenceCount .. ContainmentVolumes (not used here)
    C3DBoundingSphere BoundingSphere;                        // +0x74
    C3DBoundingBox    BoundingBox;                           // +0x84
};
class CThingBuildingDef : public CDefPointeeBase {           // Ego_r.pdb CThingBuildingDef; retail STLport sizes put the flag byte at +0xA0
public:
    unsigned char _pad_0x1c[0xa0 - 0x1c];                    // CThingGameObjectDef .. CameraManagerSetIndex (not used here)
    bool          UseHighDetailQuadTree : 1;                 // +0xA0 bit 0
    bool          IsSelfIlluminating : 1;                    // bit 1
    bool          IsPartOfHeroGuild : 1;                     // bit 2
    bool          HasWeatherMask : 1;                        // bit 3
};
namespace NParticleEngine {
class CParticleEmitterManager {
public:
    void PreparePrimitives(const CEnginePrimitivePrepareInfo& info);   // 0x00AEB990 ?PreparePrimitives@CParticleEmitterManager@NParticleEngine@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
};
CParticleEmitterManager& __fastcall GetParticleEmitterManager(); // 0x00AEAAF0
extern bool RenderingEnabled;                                    // 0x013756EC (descriptive: no PDB data symbol; initialised true)
inline bool __fastcall IsRenderingEnabled() { return RenderingEnabled; }   // Ego_r.pdb ?IsRenderingEnabled@NParticleEngine@@YI_NXZ (expanded here)
}

// CThingSearchTools::GetAllLeafThings<CThingBuilding, std::vector<CThingBuilding*>, CThingFilter_None> is instantiated in this
// TU (retail 0x004AD700, right after the CWorld bodies; landed): its body is visible here, so VC7.1 knows the result vector is
// not captured and keeps its begin pointer and size in registers across the loop's calls (0x004A1CFA..0x004A1D20).
#pragma auto_inline(off)
template<class T,class Container,class Filter>
void CThingSearchTools::GetAllLeafThings(const Filter& filter,Container& result) const
{
    const std::list<CClass*>& things = PeekTypeList(T::StatGetType());
    for (std::list<CClass*>::const_iterator it = things.begin(); it != things.end(); ++it) {
        T* thing = static_cast<T*>(*it);
        if (thing->IsIncludedInThingScans() && filter(*thing))
            result.push_back(thing);
    }
}
// CDefinitionManager::GetDef<CThingBuildingDef> is instantiated in this TU as well (retail 0x004AB840, landed: index > 0, the def
// from GetPDefFromGlobalIndex, assigned to *out when not null). With its body visible VC7.1 tracks the CDefPointer local.
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
#pragma auto_inline(on)

void CWorld::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info)
{
    if (!WeatherMaskingPrimitivesSent && PEnvironment_ptr->IsEnvironmentRequiringWeatherMasks()) {
        std::vector<CThingBuilding*> buildings;
        PThingSearchTools_ptr->GetAllLeafThings<CThingBuilding>(CThingFilter_None(), buildings);
        WeatherMaskingPrimitiveHandles.resize(buildings.size());
        for (unsigned long i = 0; i < buildings.size(); ++i) {
            CDefPointer<const CThingBuildingDef> pdef;
            DefinitionManager->GetDef(buildings[i]->GetDefGlobalIndex(), &pdef);
            if (pdef->HasWeatherMask) {
                if (buildings[i]->GetPhysicsTC()->GetPhysicsMeshInfo()) {
                    C3DVector centre;
                    float radius = 0.0f;
                    buildings[i]->GetPhysicsTC()->GetBoundingInfo(centre, radius);
                    const CRightHandedSet& rhset = buildings[i]->GetPhysicsTC()->GetRHSet();
                    C3DVector up = rhset.GetUp();
                    C3DVector forward = rhset.GetForward();
                    C3DVector right = rhset.GetRight();
                    float scale = 1.0f;
                    const CThingPhysicsMeshInfo* meshInfo = buildings[i]->GetPhysicsTC()->GetPhysicsMeshInfo();
                    if (meshInfo)
                        scale = meshInfo->RenderSizeX;
                    CMatrix3x4 transform;
                    const C3DVector& position = buildings[i]->GetPhysicsTC()->GetPosition();
                    transform.E41 = position.X;
                    transform.E42 = position.Y;
                    transform.E43 = position.Z;
                    transform.E11 = -right.X; transform.E12 = -right.Y; transform.E13 = -right.Z;
                    transform.E21 = -forward.X; transform.E22 = -forward.Y; transform.E23 = -forward.Z;
                    transform.E31 = up.X; transform.E32 = up.Y; transform.E33 = up.Z;
                    transform.PreScale(scale * 0.01f);
                    CMatrix3x4 inverse;
                    inverse.SetInverse(transform);
                    C3DBoundingBox box = buildings[i]->GetPhysicsTC()->GetPhysicsMeshObject().GetMesh()->GetBoundingBox();
                    CEnginePrimitiveWeatherMask weatherMask(C3DBoundingSphere(centre, radius), box, inverse, scale, 0, (EEngineInterpolationSet)0);
                    info.Engine->SetPersistentPrimitive(WeatherMaskingPrimitiveHandles[i], weatherMask, 0);
                }
            }
        }
        WeatherMaskingPrimitivesSent = true;
    }
    PThingManager_ptr->PreparePrimitivesForEngine(info);
    if (NParticleEngine::IsRenderingEnabled())
        NParticleEngine::GetParticleEmitterManager().PreparePrimitives(info);
    PCurrentGameCamera->PreparePrimitivesForRendering(info);
}
