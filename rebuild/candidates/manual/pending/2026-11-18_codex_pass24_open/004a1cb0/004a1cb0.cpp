// CWorld::PreparePrimitivesForEngine sends persistent weather masks and prepares the thing manager, particle manager and camera.
// Grade: DIFFER(1331v1330). The remaining differences are the cross-product loads, inverse translation term order and bounding-box copy scheduling.
// Leaf: ?PreparePrimitivesForEngine@CWorld@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A1CB0, 1330 bytes, thiscall, `ret 4`; frame 0x184.

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
#define FABLE_RETAIL_CTHINGPHYSICSMESHINFO
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGBUILDINGDEF
#define FABLE_RETAIL_CPHYSICSMESH_BOUNDS
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_BULLETTIME_PASS23
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_PREPAREPRIMITIVES_PASS24
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
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

#include "engine/CThingPhysicsMeshInfo.h"
#include "engine/CThingBuildingDef.h"
#include "engine/CPhysicsMesh.h"
#include "engine/CParticleEmitterManager.h"

void CWorld::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info)
{
    if (!WeatherMaskingPrimitivesSent && PEnvironment_ptr->IsEnvironmentRequiringWeatherMasks()) {
        std::vector<CThingBuilding*> buildings;
        PThingSearchTools_ptr->GetAllLeafThings<CThingBuilding>(CThingFilter_None(), buildings);
        WeatherMaskingPrimitiveHandles.resize(buildings.size());
        for (long c1 = 0; c1 < static_cast<long>(buildings.size()); ++c1) {
            CDefPointer<const CThingBuildingDef> pdef;
            DefinitionManager->GetDef(buildings[c1]->GetDefGlobalIndex(), &pdef);
            if (pdef->HasWeatherMask) {
                if (buildings[c1]->GetPhysicsTC()->GetPhysicsMeshInfo()) {
                    C3DVector bounding_sphere_centre;
                    float bounding_sphere_radius = 0.0f;
                    buildings[c1]->GetPhysicsTC()->GetBoundingInfo(bounding_sphere_centre, bounding_sphere_radius);
                    const CRightHandedSet& rhset = buildings[c1]->GetPhysicsTC()->GetRHSet();
                    C3DVector up = rhset.GetUp();
                    C3DVector forward = rhset.GetForward();
                    C3DVector right = rhset.GetRight();
                    float scale = 1.0f;
                    const CThingPhysicsMeshInfo* pinfo = buildings[c1]->GetPhysicsTC()->GetPhysicsMeshInfo();
                    if (pinfo)
                        scale = pinfo->RenderSizeX;
                    CMatrix3x4 transform;
                    transform.SetTranslateOnly(buildings[c1]->GetPhysicsTC()->GetPosition());
                    transform.E11 = -right.X; transform.E12 = -right.Y; transform.E13 = -right.Z;
                    transform.E21 = -forward.X; transform.E22 = -forward.Y; transform.E23 = -forward.Z;
                    transform.E31 = up.X; transform.E32 = up.Y; transform.E33 = up.Z;
                    transform.PreScale(scale * 0.01f);
                    CMatrix3x4 inv_transform;
                    inv_transform.SetInverse(transform);
                    C3DBoundingBox bbox = buildings[c1]->GetPhysicsTC()->GetPhysicsMeshObject().GetMesh()->GetBoundingBox();
                    CEnginePrimitiveWeatherMask weather_mask_primitive(C3DBoundingSphere(bounding_sphere_centre, bounding_sphere_radius), bbox, inv_transform, scale, 0, (EEngineInterpolationSet)0);
                    info.Engine->SetPersistentPrimitive(WeatherMaskingPrimitiveHandles[c1], weather_mask_primitive, 0);
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
